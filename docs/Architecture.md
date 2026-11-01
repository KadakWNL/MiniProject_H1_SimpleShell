# Architecture and Design — Simple Shell

## 1. System Overview and High-Level Flow

Simple Shell is structured around a layered, modular architecture written in ISO C11 for Linux/POSIX platforms. The shell decouples user input acquisition, lexical tokenization/syntax validation, built-in execution, and external process orchestration into distinct modules with strict interface boundaries.

```text
                            +-----------------------------+
                            |       User Interaction      |
                            +-----------------------------+
                                           |
                                           v
                            +-----------------------------+
                            |   shell_loop() (shell.c)    | <---+ (Self-Pipe / SIGCHLD)
                            |   poll(stdin, sigchld_pipe) |     |
                            +-----------------------------+     |
                                           |                    |
                                           v                    |
                            +-----------------------------+     |
                            |   parse_line() (parser.c)   |     |
                            | Tokenize, Validate, AST     |     |
                            +-----------------------------+     |
                                           |                    |
                                           v                    |
                           /-------------------------------\    |
                          /  Pipeline Configuration Check   \   |
                         /-----------------------------------\  |
                                    |               |           |
             Single Built-in        |               | Pipelined / External /
             (Foreground Parent)    |               | Background Command
                                    v               v
            +---------------------------+   +-------------------------------+
            | run_single_parent_builtin |   |  execute_children()           |
            | - dup() save fds          |   |  - pipe() IPC establishment   |
            | - apply_redirection()     |   |  - fork() child processes     |
            | - run_builtin() (parent)  |   |  - child_exec()               |
            | - fflush() & restore fds  |   |    * dup2() pipe & files      |
            +---------------------------+   |    * execvp() / run_builtin() |
                                            |  - waitpid() foreground       |
                                            |  - asynchronous background    |
                                            +-------------------------------+
                                                            |
                                                            v
                                            +-------------------------------+
                                            | Kernel / POSIX Process Engine |
                                            | - SIGCHLD on state change     |
                                            | - Async write('C') to pipe    |
                                            +-------------------------------+
```

---

## 2. Module Responsibilities

The codebase is organized into five cohesive source modules with explicit header interfaces in `include/`:

| Module | Implementation | Header Interface | Primary Responsibility |
|---|---|---|---|
| **Entry Point** | `src/main.c` | None (`main`) | Application bootstrap; invokes `shell_loop()` and propagates the final exit code. |
| **Shell Loop & REPL** | `src/shell.c` | `include/shell.h` | Displays prompt on `stderr`, manages terminal I/O via `poll()`, configures `SIGCHLD` self-pipe notification, drains child events, dispatches pipelines, and manages REPL shutdown. |
| **Parser & AST** | `src/parser.c` | `include/parser.h` | Lexical analysis, quote preservation (`'...'`, `"..."`), escape processing (`\ `), operator pre-validation (`|`, `<`, `>`, `>>`, `&`), Abstract Syntax Tree (`struct Pipeline`, `struct Command`) construction, and heap memory cleanup (`free_pipeline()`). |
| **Built-in Commands** | `src/builtins.c` | `include/builtins.h` | Implements internal shell commands (`cd`, `pwd`, `echo`, `help`, `exit`), directory tracking (`OLDPWD`), argument validation, and execution context branching (`in_child` vs parent). |
| **Execution Engine** | `src/executor.c` | `include/executor.h` | External program launching (`fork()`, `execvp()`), standard stream redirection (`open()`, `dup2()`), multi-stage pipeline configuration (`pipe()`), foreground synchronization, and non-blocking zombie process harvesting (`reap_background()`). |

---

## 3. Asynchronous Background Reaping and Self-Pipe Architecture

A critical architectural enhancement in Simple Shell is its real-time, asynchronous background process reaping mechanism, implemented via the **self-pipe trick**:

### A. The Signal-Handling Limitation
In standard POSIX programming, invoking non-async-signal-safe functions (such as `printf()`, `malloc()`, `fflush()`, or complex state updates) inside a signal handler leads to undefined behavior, reentrancy deadlocks, and corrupted shell state. Conversely, relying solely on periodic reaping between command prompts leaves background processes as "zombie" processes until the user presses Enter, failing to provide immediate completion feedback during long idle periods.

### B. The Self-Pipe Solution
Simple Shell resolves this using a dedicated unidirectional UNIX pipe (`sigchld_pipe[2]`) configured as follows:
1. **Pipe Initialization:** During startup (`initialize_child_notifications`), the shell opens `sigchld_pipe` and sets both read and write descriptors to non-blocking (`O_NONBLOCK`) and close-on-exec (`FD_CLOEXEC`).
2. **Signal Handler:** A POSIX signal handler (`handle_sigchld`) is registered for `SIGCHLD` via `sigaction()`. When any child process terminates, stops, or continues, the kernel delivers `SIGCHLD`. The handler executes only a single, async-signal-safe system call:
   ```c
   const char notification = 'C';
   (void)write(fd, &notification, sizeof(notification));
   ```
   No formatting, memory allocations, or `waitpid()` calls occur inside the signal context.
3. **I/O Multiplexing via `poll()`:** The interactive input reader (`read_command_line`) monitors two file descriptors simultaneously:
   - `descriptors[0].fd = STDIN_FILENO` (`POLLIN`)
   - `descriptors[1].fd = sigchld_pipe[0]` (`POLLIN`)
4. **Draining in Normal Execution Context:** When `poll()` detects activity on `descriptors[1]`, the shell invokes `process_child_notifications()`. This drains all accumulated notification bytes from the pipe and calls `reap_background()`.
5. **Non-blocking Zombie Harvesting:** `reap_background()` executes a loop around `waitpid(-1, &status, WNOHANG)`:
   - Reaps all terminated background children.
   - Emits asynchronous diagnostic messages to `stderr` (e.g. `[background pid 12345 exited 0]`).
   - Prevents zombie process accumulation.
6. **Idle Detection:** A self-pipe notification wakes `poll()`, allowing completed background processes to be reaped while the user is idle at the prompt. Handling occurs when the shell is scheduled; it is not a real-time latency guarantee.
7. **Foreground Concurrency Reaping:** If a background child finishes while the shell is blocked in `execute_children()` waiting for foreground pipeline stages, the `waitpid(-1, &status, 0)` loop detects that the reaped PID belongs to a background job, prints its termination message to `stderr`, and continues waiting for the foreground pipeline stages without interruption.

---

## 4. Process Execution Architecture

### Solitary Foreground Built-in Commands
Commands that mutate parent shell state (`cd`, `exit`) or inspect current state (`pwd`, `echo`, `help`) run directly within the parent process when executed as a single foreground command:
- Standard input/output are backed up using `dup()`.
- Redirections (`<`, `>`, `>>`) are applied using `apply_redirection()`.
- On redirection failure, saved descriptors are restored, and built-in execution is inhibited.
- `run_builtin(cmd, 0)` executes the command logic.
- Output buffer is committed with `fflush(stdout)`.
- Standard streams are restored via `dup2()` and the duplicate descriptors are closed.

### Pipelines and External Commands
Pipelines (`count >= 2`) or external commands execute in child processes spawned via `fork()`:
- **Pipes:** Consecutive command stages `i` and `i+1` are linked via `pipe(pipe_fd)`. The read-end of the previous stage (`previous_read`) supplies `STDIN_FILENO`, and the write-end of the current pipe (`pipe_fd[1]`) supplies `STDOUT_FILENO`.
- **Descriptor Hygiene:** Unused pipe ends are closed immediately in both child and parent processes. Intermediate file descriptors are closed as stages are launched, preventing file descriptor leaks and guaranteeing that EOF propagates cleanly to consumers.
- **Redirection Precedence:** In `child_exec()`, pipeline descriptors are duplicated first, followed by explicit command file redirections (`cmd->input_file`, `cmd->output_file`). File redirections override pipeline descriptors if both are specified.
- **Built-ins in Pipelines:** Built-in commands within pipelines execute in child processes (`run_builtin(cmd, 1)`), terminating with `_exit()`. This ensures that `cd` or `exit` inside a pipeline segment (e.g. `cd /tmp | cat` or `exit | cat`) does not alter the parent shell's working directory or terminate the parent REPL.
- **Foreground Synchronization:** The parent waits for all foreground child processes using `waitpid()`. The exit status of the rightmost command stage is returned as the pipeline result.
- **Background Execution:** If `pipeline->background` is set (`cmd &`), the parent prints `[background pid <pid>]` to `stdout`, flushes `stdout`, and returns `0` immediately without blocking.

---

## 5. Component Interfaces

The architecture defines four primary C interfaces:

### `include/shell.h`
- `int shell_loop(void)`: Main REPL entry point. Repeatedly presents prompt, polls input and notifications, parses commands, executes pipelines, and shuts down cleanly.

### `include/parser.h`
- `#define MAX_ARGS 128`: Maximum arguments after the command name (129 total `argv` entries, excluding the NULL terminator).
- `#define MAX_PIPE_SEGMENTS 32`: Maximum supported pipeline command stages.
- `int parse_line(const char *line, struct Pipeline *pipeline, char *error, size_t error_size)`: Tokenizes line, processes quotes/escapes, validates operator grammar, and populates AST.
- `void free_pipeline(struct Pipeline *pipeline)`: Frees all dynamic memory in a pipeline (argv strings, redirection paths).

### `include/builtins.h`
- `int is_builtin(const char *name)`: Checks if a string matches `cd`, `pwd`, `echo`, `help`, or `exit`.
- `int run_builtin(const struct Command *cmd, int in_child)`: Executes built-in logic with context awareness.
- `#define SHELL_STATUS_EXIT 1000`: Sentinel status signaling parent loop termination.

### `include/executor.h`
- `int execute_pipeline(const struct Pipeline *pipeline)`: Dispatches single built-in or multi-stage children.
- `void reap_background(void)`: Harvests finished background children via non-blocking `waitpid()`.

---

## 6. Deviations from Phase 1 SRS/SAD

During Sprint 1 and Sprint 2 implementation and quality hardening, several architectural refinements were introduced to resolve POSIX edge cases, enhance concurrency, prevent resource leaks, and eliminate zombie process accumulation.

The table below catalogs all deviations between the Phase 1 Architectural Design / SRS expectations and the final merged implementation on `main`:

| Area | Phase 1 Design / Expectation | Actual Merged Implementation | Rationale and Engineering Impact | Status |
|---|---|---|---|---|
| **Input Loop & Event Multiplexing** | Standard blocking `getline(3)` or stream read on `stdin`. | `read_command_line()` using `poll()` on `STDIN_FILENO` and a self-pipe descriptor (`sigchld_pipe[0]`), reading characters into an expanding dynamic buffer. | **Architectural Refinement:** Allows the shell to be multiplexed between terminal user input and background child state changes. Eliminates blocking deadlocks and enables prompt wakeups when background tasks finish. | Implemented & verified (PR #28). FR-01 and FR-07 satisfied. |
| **Background Child Reaping** | Periodic reaping between prompts using `waitpid(-1, &status, WNOHANG)`. | Real-time asynchronous `SIGCHLD` handler writing to a non-blocking self-pipe, drained by `poll()` and reaped via `reap_background()`; concurrent reaping during foreground waits. | **Architectural Hardening:** Periodic-only reaping left zombie processes in the OS process table if the user remained idle at the prompt. The self-pipe trick ensures immediate zombie cleanup and instant feedback on `stderr` without risking signal-safety violations. | Implemented & verified (PR #28). FR-07 enhanced. |
| **Pipeline Scale & Capacity** | Two-stage pipelines (`cmd1 \| cmd2`) illustrated in early design diagrams and test cases (TC-12). | Generic multi-stage pipeline executor supporting up to 32 concurrent stages (`MAX_PIPE_SEGMENTS = 32`). | **Capability Extension:** Extended from two stages to arbitrary pipelines up to 32 stages. Concurrent child execution and immediate closing of unused pipe ends prevents pipe buffer exhaustion and deadlocks on large data transfers (`seq 20000 \| wc -l`). | Implemented & verified (PR #27). FR-06 expanded. |
| **Built-in Execution Isolation** | Initial high-level design described built-ins as executing within the shell process without distinguishing pipeline context. | Context-aware execution: single foreground built-in executes in parent (persisting `cd`/`exit`); built-ins in pipelines or background execute in child processes via `run_builtin(cmd, 1)` and `_exit()`. | **Architectural Refinement:** Conforms to POSIX pipeline isolation standards. Prevents child stages (e.g. `cd /tmp \| cat`) from inadvertently mutating parent working directory or terminating the parent interactive shell. | Implemented & verified (PR #27). FR-03 & FR-06 isolated. |
| **Operator Pre-Validation** | Lexical parsing during AST construction, with syntax errors and redirection handling deferred to execution time. | Strict pre-validation pass in `src/parser.c` verifying operator counts (`&&`, duplicate `<`/`>`, misplaced `&`, boundary limits) before execution. | **Security & Robustness Hardening:** Satisfies NFR-07 (Security Hygiene). Rejection of malformed syntax occurs before any child processes are forked or any output files are truncated/created (`touch must-not-exist \| cat > rejected > duplicate` is rejected before any files are created). | Implemented & verified (PR #27). FR-04 & FR-08 hardened. |

### Summary of Deviation Impact
None of the architectural deviations represent an abandonment or compromise of original requirements. Instead, all five deviations represent **engineering refinements and hardening** beyond the initial design sketches, using POSIX process and I/O APIs while preserving the limited shell syntax documented in the README. Full POSIX shell language compliance is not claimed; resource audit and regression acceptance remain tracked in issues #15 and #16.
