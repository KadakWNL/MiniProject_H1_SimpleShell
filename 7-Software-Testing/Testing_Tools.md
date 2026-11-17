# Testing Tools Used and Planned

| Tool | Purpose | Evidence state |
|---|---|---|
| GCC and Makefile warning flags | Compile C source and surface diagnostics (`-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes -std=c11`) | Build configuration active; verified warning-free across standard and sanitizer builds |
| GNU Make | Reproducible build/test/sanitizer targets (`all`, `test`, `asan-test`, `ci`, `clean`, `info`) | Makefile build/test targets; workflow step timeouts and executor harness alarm |
| Bash test harness | End-to-end shell scenarios including pipelines, built-ins, redirections, and error recovery | [Shell tests](../tests/test_shell.sh); results in the [test report](../docs/Test_Report.md) |
| Parser unit test | Parsing, quotes, operator diagnostics, limits, and memory lifecycle | [Parser tests](../tests/test_parser.c); results in the [test report](../docs/Test_Report.md) |
| Executor component test | Built-in/external pipeline stages, statuses, both-child waiting, EOF, descriptor stress, and reaping | [Executor tests](../tests/test_executor.c); results in the [test report](../docs/Test_Report.md) |
| Termination cleanup harness | `SIGTERM` foreground wait interruption and background descendant reaping (`make test` / `make asan-test`) | [Termination harness](../tests/test_termination.py); results in the [test report](../docs/Test_Report.md) |
| GitHub Actions | Automated CI pipeline triggered on PRs and commits targeting `main` with step timeouts | `.github/workflows/ci.yml` configured and active on `ubuntu-latest` (Refs Issue #8) |
| AddressSanitizer / UndefinedBehaviorSanitizer | Memory violation, leak detection, and undefined-behaviour checks via `make asan-test` | Automated in CI and local harness (`tests/test_sanitizers.sh`); zero diagnostics emitted across 182 checks |
| Valgrind | Memory and descriptor leak verification on unit harnesses and pipeline/background scenario | Target `make valgrind` active in CI; `0 errors from 0 contexts` |

## Bug-fix & CI Validation Process
Reproduce → isolate → add regression test → patch on feature branch → run focused tests → run regression/sanitizer checks (`make asan-test`) → open PR targeting `main` → GitHub Actions CI green → review and merge → record retest evidence.
