# Simple Shell

A lightweight Unix-like command-line interpreter written in C11 for Linux/POSIX environments.

---

## 1. Project Overview

Simple Shell is an educational Unix-like command-line interpreter developed by Team H1 as a Software Engineering mini-project. The project was designed and implemented to provide a modular shell built with POSIX process and I/O APIs that demonstrates core operating systems principles:

- Interactive Read-Eval-Print Loop (REPL) and robust terminal I/O.
- Process lifecycle management using `fork()`, `execvp()`, and `waitpid()`.
- Standard stream redirection (`<`, `>`, `>>`) via `open()` and `dup2()`.
- Inter-process communication pipelines (`|`) supporting multi-stage execution with deadlock avoidance.
- Asynchronous background process execution (`&`) with real-time zombie reaping via `SIGCHLD` notifications over a self-pipe.
- Lexical analysis, stateful quote processing (single and double quotes), escape handling, and pre-validation of shell operators without command or file side-effects on syntax errors.

### Implementation Environment
- **Standard:** ISO C11 (`-std=c11`) with POSIX.1-2008 APIs (`_POSIX_C_SOURCE 200809L`).
- **Platform:** Native Linux (x86_64) or compatible Linux VM/WSL2 environments.
- **Compiler:** GCC 11+ with strict warning flags (`-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes`).
- **Architecture:** Layered and modular architecture dividing responsibilities across five distinct C modules.

---

## 2. Prerequisites and Installation

Simple Shell runs as a native local Linux process and requires no external databases, servers, or cloud credentials.

### Required Dependencies
- **Operating System:** Linux (Ubuntu 22.04 LTS, Ubuntu 24.04 LTS, Debian, or compatible Linux distribution).
- **Compiler:** GCC supporting the C11 standard.
- **Build Tool:** GNU Make.
- **Shell / Harness:** GNU Bash (for running the integration test harness).
- **Version Control:** Git.

### Installation on Ubuntu / Debian

Update your package index and install the standard build essentials:

```bash
sudo apt update
sudo apt install build-essential git
```

*(For Fedora/RHEL, use `sudo dnf groupinstall "Development Tools" && sudo dnf install git`; for Arch Linux, use `sudo pacman -S base-devel git`.)*

---

## 3. Clone and Setup

Clone the repository and navigate to the project root directory:

```bash
git clone https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell.git
cd MiniProject_H1_SimpleShell
```

---

## 4. Build and Execute

Compile the project using the provided `Makefile`:

```bash
make
```

This compiles all modules with strict warning flags and produces the executable binary `./simple-shell`.

### Launching the Shell

```bash
./simple-shell
```

Upon startup, Simple Shell presents its interactive prompt on `stderr`:

```text
simple-shell$
```

The shell is now ready to accept interactive commands. Prompts and background completion diagnostics are written to standard error (`stderr`). Background launch messages (`[background pid <pid>]`) are written to standard output (`stdout`).

---

## 5. Features

- **Interactive Command Loop:** Displays `simple-shell$ ` prompt, processes user input line-by-line, recovers gracefully from errors, and terminates cleanly on `exit` or End-of-File (EOF / `Ctrl-D`).
- **Built-in Commands:**
  - `cd [DIR]`: Change working directory. Supports `cd` (to `HOME`), `cd <path>`, and `cd -` (toggling to `OLDPWD`). Runs in the parent process for a single foreground command; in a pipeline or background command, directory changes affect only the child.
  - `pwd`: Print the current working directory to standard output.
  - `echo [TEXT...]`: Print arguments separated by single spaces.
  - `help`: Display built-in command documentation and supported shell operators.
  - `exit`: Gracefully terminate the interactive shell session with exit status 0.
- **External Command Execution:** Searches system `PATH` and executes external binaries via `fork()` and `execvp()`.
- **Argument Parsing, Quotes & Escapes:**
  - Whitespace-delimited argument tokenization.
  - Single quotes (`'...'`): Preserve the literal value of all enclosed characters without escape interpretation.
  - Double quotes (`"..."`): Preserve whitespace and words while supporting embedded escaped quotes (`\"`) and backslashes (`\\`).
  - Backslash escaping (`\x`): Escapes spaces, quotes, and operator symbols outside of quotes.
- **Input Redirection (`<`):** Redirects standard input (`STDIN_FILENO`) from a specified file using `O_RDONLY`.
- **Output Overwrite Redirection (`>`):** Redirects standard output (`STDOUT_FILENO`) to a target file, creating it if needed or truncating existing content (`O_WRONLY | O_CREAT | O_TRUNC`).
- **Output Append Redirection (`>>`):** Redirects standard output to a file in append mode, preserving existing content (`O_WRONLY | O_CREAT | O_APPEND`).
- **Multi-Stage Pipelines (`|`):** Connects the standard output of each command stage to the standard input of the next stage using unidirectional pipes (`pipe()` and `dup2()`). Supports up to 32 concurrent pipeline stages and handles stream transfers larger than standard pipe buffers without deadlocking.
- **Background Execution (`&`):** Commands ending with `&` execute asynchronously in the background. The shell prints `[background pid <pid>]` and immediately returns control to the interactive prompt.
- **Asynchronous Background Zombie Reaping:** Uses a real-time `SIGCHLD` signal handler communicating via a self-pipe. The shell monitors standard input and child process notifications simultaneously using `poll()`, reaping finished background jobs and reporting exit status to `stderr` even while idle at the prompt or executing foreground commands.
- **Strict Operator Pre-Validation:** Lexical pre-validation rejects duplicate background operators (`&&`, `& &`), misplaced `&`, duplicate input or output redirections on one command, and dangling operators before command execution, ensuring that malformed command lines create no child processes and produce no spurious output files.

---

## 6. Usage Examples

### Navigation and Built-in Commands

```bash
simple-shell$ pwd
/home/user/MiniProject_H1_SimpleShell
simple-shell$ cd /tmp
simple-shell$ pwd
/tmp
simple-shell$ echo "Hello from Simple Shell"
Hello from Simple Shell
simple-shell$ help
Simple Shell - supported commands:
  cd [DIR]       Change current directory
  pwd            Print current directory
  echo [TEXT...] Print text
  help           Show this help message
  exit           Exit the shell

External commands, pipes (|), redirection (<, >, >>),
and background execution (&) are supported.
simple-shell$ date
Sat Oct 31 19:15:00 UTC 2026
```

### I/O Redirection

```bash
simple-shell$ echo "Hello" > output.txt
simple-shell$ echo "World" >> output.txt
simple-shell$ cat < output.txt
Hello
World
simple-shell$ echo "Overwrite" > output.txt
simple-shell$ cat < output.txt
Overwrite
```

### Command Pipelines

```bash
simple-shell$ printf "apple\nbanana\ncherry\n" | grep banana
banana
simple-shell$ printf "a\nb\nc\n" | wc -l
3
simple-shell$ echo "pipeline test" | cat | wc -w
2
```

### Background Execution

```bash
simple-shell$ sleep 2 &
[background pid 12345]
simple-shell$ echo "Immediate prompt return"
Immediate prompt return
[background pid 12345 exited 0]
```

### Clean Termination

```bash
simple-shell$ exit
```

Or press `Ctrl-D` (EOF) at an empty prompt to terminate cleanly.

---

## 7. Supported Syntax and Limitations

### Supported Syntax Grammar

| Operator / Syntax | Description | Example |
|---|---|---|
| `arg1 arg2` | Whitespace-delimited command arguments | `ls -la /tmp` |
| `'literal'` | Single quotes (literal text, no escape processing) | `echo 'a | b & < >'` |
| `"quoted"` | Double quotes (preserves spaces, supports `\"` and `\\`) | `echo "Hello \"World\""` |
| `\ ` | Backslash escape (preserves literal character outside quotes) | `echo hello\ world` |
| `< file` | Input redirection (read from file) | `cat < input.txt` |
| `> file` | Output overwrite redirection (truncates or creates file) | `date > stamp.txt` |
| `>> file` | Output append redirection (appends or creates file) | `echo "log entry" >> app.log` |
| `\|` | Command pipeline (connects stdout to stdin across stages) | `cat data.txt \| grep key \| wc -l` |
| `&` | Asynchronous background execution (must be last token) | `sleep 5 &` |

### Enforced Limits and Rejected Syntax

The following limits are strictly enforced by the parser and reported with helpful diagnostics on `stderr`:

| Limit / Condition | Enforced Value | Parser Error Diagnostic |
|---|---|---|
| Maximum arguments per command | 128 (excluding binary name & sentinel) | `shell: syntax error: too many arguments or memory allocation failure` |
| Maximum pipeline segments | 32 command stages | `shell: syntax error: pipeline is too long` |
| Multiple input redirections | Rejected | `shell: syntax error: multiple input redirections are not supported` |
| Multiple output redirections | Rejected | `shell: syntax error: multiple output redirections are not supported` |
| Multiple background operators | Rejected | `shell: syntax error: multiple background operators are not supported` |
| Misplaced background operator | Rejected | `shell: syntax error: background operator must be at the end` |
| Unterminated quotation | Rejected | `shell: syntax error: unterminated quote` |
| Trailing escape character | Rejected | `shell: syntax error: trailing escape character` |
| Missing redirection target | Rejected | `shell: syntax error: redirection requires a file name` |
| Empty pipe segment | Rejected | `shell: syntax error: empty command before pipe` |

### Scope and Exclusions
The following shell capabilities are outside the scope of this mini-project release:
- Full POSIX shell scripting (e.g. `if`, `while`, `for`, `case` loops and functions).
- Variable expansion (`$VAR`, `$?`, `$!`) and arithmetic evaluation (`$((...))`).
- Pathname expansion / wildcard globbing (`*`, `?`, `[...]`).
- Command substitution (`$(cmd)` or `` `cmd` ``).
- Command aliases, shell history manipulation, and user configuration files (`.shellrc`).
- Terminal interactive job control signals (`Ctrl-Z`, `fg`, `bg`).

---

## 8. Testing and Sanitizers

Simple Shell includes a comprehensive, automated test suite covering unit, component, integration, and memory sanitizer validation.

### Running the Test Suites

To compile the project and execute all unit and integration test suites:

```bash
make test
```

To rebuild the project under AddressSanitizer (ASan) and UndefinedBehaviorSanitizer (UBSan) and execute the full test suite:

```bash
make asan-test
```

To clean all build artifacts and compiled binaries:

```bash
make clean
```

### Executed Test Targets

The `make test` and `make asan-test` targets execute three complementary validation suites:
1. **Parser Unit Tests (`tests/test_parser.c`):** 63 assertions verifying argument tokenization, quote and escape processing, boundary limits (32 vs 33 pipeline stages, 128 vs 129 arguments after the command name), rejected operator combinations, and leak-free memory lifecycle under repeated allocation/free cycles.
2. **Direct Executor Tests (`tests/test_executor.c`):** 29 component tests verifying external and built-in pipeline execution, multi-stage pipelines (3, 4, and 32 stages), rightmost command exit status capture, waiting for producer, intermediate, and consumer stages independently, pipe capacity deadlock prevention (`seq 50000 | cat | wc -l`), early consumer termination without deadlock, parent state isolation (`cd` in pipeline), foreground child reaping, and 100 2-stage and 50 4-stage pipeline descriptor stress testing under a restricted 64-descriptor `rlimit`.
3. **Shell Integration Tests (`tests/test_shell.sh`):** 66 end-to-end integration scenarios verifying clean termination (`exit` and EOF), stderr prompt output, built-in execution (`cd`, `pwd`, `echo`, `help`), redirection truncation and append, multi-stage pipelines (3, 4, 5, and 32 stages), pipelines combined with input/output redirections, syntax error recovery without command side-effects or file creation, foreground process synchronization, and non-blocking background execution with real-time asynchronous self-pipe reaping (158 total checks executed per build).

---

## 9. Project Structure

```text
MiniProject_H1_SimpleShell/
├── src/                               # C source implementation
│   ├── main.c                         # Program entry point
│   ├── shell.c                        # Interactive REPL, self-pipe, poll() loop, and orchestration
│   ├── parser.c                       # Lexical analysis, quote/escape handling, AST construction
│   ├── builtins.c                     # Built-in commands (cd, pwd, echo, help, exit)
│   └── executor.c                     # Process spawning, redirection, pipelines, waiting, reaping
├── include/                           # C header interfaces
│   ├── shell.h                        # REPL declarations
│   ├── parser.h                       # AST structs, limits, parser interface
│   ├── builtins.h                     # Built-in declarations and status codes
│   └── executor.h                     # Executor interface and reaping functions
├── tests/                             # Automated test suites
│   ├── test_parser.c                  # Parser unit tests
│   ├── test_executor.c                # Executor component tests
│   ├── test_shell.sh                  # Shell end-to-end integration test harness
│   └── test_sanitizers.sh             # Sanitizer build and verification harness
├── docs/                              # Canonical project documentation
│   ├── Architecture.md                # System architecture, process model, self-pipe, deviations
│   ├── SRS.md                         # Software Requirements Specification
│   ├── RTM.md                         # Requirements Traceability Matrix
│   ├── Test_Cases.md                  # Test case catalog and verification procedures
│   ├── Test_Report.md                 # Recorded validation evidence and CI run logs
│   ├── Project_Status.md              # Live project status and deliverable tracking
│   ├── Sprint_Backlog.md              # Sprint 1 and Sprint 2 backlog and story point allocation
│   └── demos/                         # Demo video and verification evidence
├── 1-SRS-and-Work-Breakdown/          # Coursework deliverable folders
├── 2-Test-Planning/
├── 3-Requirements-Engineering/
├── 4-Architectural-Design/            # Architecture diagrams, plantuml source, selection rationale
├── 5-GitHub-Jira-Evidence/
├── 6-Copilot-Evidence/
├── 7-Software-Testing/
├── 8-Deployment/
├── 9-Demo/
├── .github/workflows/                 # GitHub Actions CI automation
│   └── ci.yml                         # Build, test, and sanitizer workflow definition
├── Makefile                           # Build configuration and test targets
├── CONTRIBUTING.md                    # Git workflow, PR conventions, review requirements
└── README.md                          # Project overview and evaluator guide
```

For detailed technical design specifications, refer to [docs/Architecture.md](docs/Architecture.md). For complete requirements traceability and test reports, refer to [docs/RTM.md](docs/RTM.md) and [docs/Test_Report.md](docs/Test_Report.md).

---

## 10. Mini-Project Submission Metadata

- **Mini-Project:** Simple Shell — Unix Command-Line Interpreter
- **Team:** H1
- **Team Members:**
  - Shrikar Desai ([@ShrikarDesai07](https://github.com/ShrikarDesai07))
  - Shreyas Kulkarni ([@KadakWNL](https://github.com/KadakWNL))
  - Sujal Yadavi ([@Aqual456](https://github.com/Aqual456))
  - Sudeeksh Nayak ([@sudeeksh02](https://github.com/sudeeksh02))
- **Language:** C (C11 standard)
- **Target OS:** Linux / POSIX.1-2008
- **Repository:** https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell
- **Project Board:** https://github.com/orgs/SE-H1-SimpleShell/projects/2
