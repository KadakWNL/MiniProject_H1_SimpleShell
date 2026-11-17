# Sprint 1 Demo & Verification Documentation

## Purpose
This document records the Sprint 1 working-product demonstration and clean-clone verification for the **Simple Shell** project in accordance with GitHub Issue [#9](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/9) (`[S1-09] Sprint 1 Validation, Fresh Clone Verification & 1-Minute Demo Video`).

## Video Demonstration
- **Video file:** [`simple-shell-sprint1-demo.mp4`](simple-shell-sprint1-demo.mp4)
- **Path:** `docs/demos/simple-shell-sprint1-demo.mp4`
- **Target Duration:** ~60 seconds (1 minute)

---

## Demonstration Script & Command Sequence

The demonstration follows the required sequence and timings:

| Timing | Phase | Command / Action | Expected Terminal Behavior |
|---|---|---|---|
| **00:00 – 00:08** | **Launch** | `make`<br>`./simple-shell` | Compiles the shell with zero warnings and displays the interactive prompt `simple-shell$ `. |
| **00:08 – 00:23** | **Built-in Navigation** | `pwd`<br>`cd /tmp`<br>`pwd` | Prints the initial directory, navigates to `/tmp`, and prints the updated working directory to prove successful directory change. |
| **00:23 – 00:38** | **Quoted Input & External Commands** | `echo "Hello from Simple Shell"`<br>`date`<br>`ls` | Prints the quoted string preserving spaces, then runs external commands `date` and `ls` with correct standard output. |
| **00:38 – 00:50** | **Error Handling & Recovery** | `simple_shell_invalid_command_123`<br>`echo "recovered and running"` | Produces an error diagnostic on `stderr`, smoothly redisplays the prompt, and executes the subsequent command without crashing or terminating. |
| **00:50 – 01:00** | **Help & Clean Exit** | `help`<br>`exit` | Displays the built-in help summary with supported syntax, then cleanly terminates the interactive loop with exit code `0`. |

---

## Clean-Clone Verification Details

- **Repository:** `https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell.git`
- **Base Branch:** `main`
- **Tested Commit SHA:** `199c52aaa99804f0fd6681f85178d20a0fdd3cc7`
- **Target Binary:** `simple-shell`

### Build & Test Results
```bash
# 1. Clean build
$ make clean && make
rm -rf build simple-shell simple-shell.exe
mkdir -p build
gcc -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes -g -Iinclude -c src/builtins.c -o build/builtins.o
gcc -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes -g -Iinclude -c src/executor.c -o build/executor.o
gcc -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes -g -Iinclude -c src/main.c -o build/main.o
gcc -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes -g -Iinclude -c src/parser.c -o build/parser.o
gcc -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes -g -Iinclude -c src/shell.c -o build/shell.o
gcc -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes -g -Iinclude build/builtins.o build/executor.o build/main.o build/parser.o build/shell.o -o simple-shell
# Exit code: 0 (0 warnings produced)

# 2. Automated test suite
$ make test
./build/test_parser
# Parser tests: 18 passed, 0 failed
bash tests/test_shell.sh ./simple-shell
# Shell tests: 18 passed, 0 failed
Total: 18 passed, 0 failed (exit code: 0)
```

### Environment Notes & Limitations
- Clean-clone build and test suite pass 100% on GCC C11.
- Local verification executed using the MSYS2 POSIX compatibility environment (GCC 15.2.0, GNU Make 4.4.1).
- Native Linux execution and memory safety checks (`make test`, CI workflow) are verified on Ubuntu (`ubuntu-latest`) via GitHub Actions.

## Final Release Demonstration - Issue #18

- **Video:** [`se-compressed.mp4`](se-compressed.mp4), supplied for final submission; duration 1:58.63.
- **Source release:** `v1.0-final` is already published on `main`.
- **Test evidence:** the project release documentation records 164 checks passed, 0 failed.
