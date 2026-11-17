# Branch Coverage Report

## Recorded measurement

See [validation manifest](evidence/Validation_Manifest.md) for source SHA, environment, commands, and limitations.

| Production source | Covered | Instrumented | Coverage |
|---|---:|---:|---:|
| `src/builtins.c` | 38 | 56 | 67.86% |
| `src/executor.c` | 139 | 226 | 61.50% |
| `src/main.c` | 0 | 0 | N/A |
| `src/parser.c` | 156 | 192 | 81.25% |
| `src/shell.c` | 66 | 102 | 64.71% |
| **Total** | **399** | **576** | **69.27%** |

## Method and limits

`make coverage` rebuilds at `-O0 --coverage`, runs the complete Linux suite, and exports GCC gcov JSON with branch counts. Only `src/*.c` contributes to the denominator; test harnesses and external programs are excluded. Branch coverage means outgoing gcov branch edges taken at least once, not condition/MC/DC coverage. Forked children that exec or call `_exit` may not flush their counters; their behavior can be tested while gcov reports their code uncovered. Uncovered error paths include failures that require syscall/allocation fault injection. No 100% coverage claim is made.

Raw evidence: [coverage run](evidence/coverage/test.log), [gcov output](evidence/coverage/gcov.log), and compressed JSON files in `docs/evidence/coverage/`.
