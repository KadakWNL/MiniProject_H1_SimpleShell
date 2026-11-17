# Code Coverage Report

## Recorded measurement

See [validation manifest](evidence/Validation_Manifest.md) for source SHA, environment, commands, and limitations.

| Production source | Covered | Instrumented | Coverage |
|---|---:|---:|---:|
| `src/builtins.c` | 63 | 80 | 78.75% |
| `src/executor.c` | 158 | 278 | 56.83% |
| `src/main.c` | 2 | 2 | 100.00% |
| `src/parser.c` | 257 | 321 | 80.06% |
| `src/shell.c` | 148 | 202 | 73.27% |
| **Total** | **628** | **883** | **71.12%** |

## Method and limits

`make coverage` rebuilds at `-O0 --coverage`, runs the complete Linux suite, and exports GCC gcov JSON with branch counts. Only `src/*.c` contributes to the denominator; test harnesses and external programs are excluded. Branch coverage means outgoing gcov branch edges taken at least once, not condition/MC/DC coverage. Forked children that exec or call `_exit` may not flush their counters; their behavior can be tested while gcov reports their code uncovered. Uncovered error paths include failures that require syscall/allocation fault injection. No 100% coverage claim is made.

Raw evidence: [coverage run](evidence/coverage/test.log), [gcov output](evidence/coverage/gcov.log), and compressed JSON files in `docs/evidence/coverage/`.
