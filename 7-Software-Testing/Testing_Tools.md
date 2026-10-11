# Testing Tools Used and Planned

| Tool | Purpose | Evidence state |
|---|---|---|
| GCC and Makefile warning flags | Compile C source and surface diagnostics | Build configuration exists; capture output on tested commit |
| GNU Make | Reproducible build/test targets | Makefile present |
| Bash test harness | End-to-end shell scenarios | tests/test_shell.sh present |
| Parser unit test | Parsing and syntax rejection | tests/test_parser.c present |
| GitHub Actions | Run make and make test on pushes/PRs | Workflow exists; a recent run succeeded |
| AddressSanitizer / UndefinedBehaviorSanitizer | Memory and undefined-behaviour checks via make asan-test | Documented; record actual output |
| Valgrind (optional) | Secondary memory check | Record only if actually run |

## Bug-fix process
Reproduce → isolate → add regression test → patch on feature branch → run focused tests → run regression/sanitizer checks → open PR → review and merge → record retest evidence.
