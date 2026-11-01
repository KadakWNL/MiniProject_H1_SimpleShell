# API and Component Interfaces

Simple Shell is a local C program and does not expose a web/REST API. “API” here refers to module-level C interfaces declared under `include/`.

| Module | Source | Header | Responsibility | Key Exposed Functions |
|---|---|---|---|---|
| **Shell Loop** | `src/shell.c` | `include/shell.h` | Prompt, input multiplexing via `poll()`, `SIGCHLD` self-pipe registration, and lifecycle orchestration | `int shell_loop(void);` |
| **Parser** | `src/parser.c` | `include/parser.h` | Lexing, quotes, escapes, operator pre-validation, AST allocation, and teardown | `int parse_line(const char *, struct Pipeline *, char *, size_t);`<br>`void free_pipeline(struct Pipeline *);` |
| **Built-ins** | `src/builtins.c` | `include/builtins.h` | Internal commands (`cd`, `pwd`, `echo`, `help`, `exit`) and context-aware execution | `int is_builtin(const char *);`<br>`int run_builtin(const struct Command *, int in_child);` |
| **Executor** | `src/executor.c` | `include/executor.h` | Process creation, redirection, multi-stage pipelines, foreground waiting, and background reaping | `int execute_pipeline(const struct Pipeline *);`<br>`void reap_background(void);` |

## Interaction Contract

1. **Input Acquisition & Event Multiplexing:** `shell_loop()` monitors `STDIN_FILENO` and an internal `SIGCHLD` notification pipe (`sigchld_pipe[0]`) via `poll()`. Terminated background jobs wake `poll()` and are harvested immediately by `reap_background()`.
2. **Lexical Parsing & Pre-Validation:** `parse_line()` transforms raw input into a `struct Pipeline` containing up to 32 `struct Command` segments. If any syntax error or limit violation is detected, an error diagnostic is written to the error buffer, no execution occurs, and any partially allocated tokens are released.
3. **Execution Dispatch:**
   - Single foreground built-in: Executed directly in the parent process (`run_single_parent_builtin`) with file descriptors saved and restored via `dup()` and `dup2()`.
   - External commands and multi-stage pipelines: Dispatched to `execute_children()`, which sets up unidirectional pipes (`pipe()`), spawns child processes (`fork()`), applies file redirections (`dup2()`), and invokes `execvp()` or `run_builtin(cmd, 1)`.
4. **Foreground Synchronization:** The parent waits for all foreground child processes using `waitpid()`, collecting the exit status of the rightmost stage.
5. **Memory & Descriptor Hygiene:** Parser-allocated memory is released with `free_pipeline()` at the end of every REPL iteration. All temporary and intermediate file descriptors are closed on both success and error paths.
