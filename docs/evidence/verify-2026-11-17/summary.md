# Re-verification — 17 November 2026

This folder records an independent re-execution of the expanded Part-3 test suite while the testing
package was integrated onto the IEEE-restructured `main`. It supplements, and does not replace, the
3 November 2026 evidence in `docs/evidence/`.

## Identity and environment

- Production source revision: `859b43ea8847b9abff28b9ea2c537b4c593deb5e` (`v1.0-final`). No file
  under `src/` or `include/` was modified by this integration.
- Test-only changes under re-verification: `tests/test_executor.c`, `tests/test_termination.py`,
  `Makefile` (coverage target), `tools/coverage_report.py`.
- Environment: Kali GNU/Linux Rolling under WSL2, Linux 6.6.87.2-microsoft-standard-WSL2, GCC/gcov
  16.2.0, GNU Make 4.4.1, GNU bash 5.3.9, Python 3.14.7, valgrind 3.27.1.

## Commands and results

| Command | Exit | Evidence | Result |
|---|---:|---|---|
| `make clean && make test` | 0 | [standard.log](standard.log) | 182 passed, 0 failed |
| `make asan-test` | 0 | [sanitizers.log](sanitizers.log) | 182 passed, 0 failed; no ASan/UBSan diagnostics |
| `make valgrind` | 0 | [valgrind.log](valgrind.log) | 0 errors from 0 contexts across all audited processes |
| `make coverage` | 0 | [coverage.log](coverage.log) | regenerated [code](../coverage/test.log) and [branch](../../Branch_Coverage_Report.md) reports |

## Suite breakdown

| Harness | Checks |
|---|---:|
| Parser (`tests/test_parser.c`) | 63 |
| Executor/component (`tests/test_executor.c`) | 45 |
| Shell integration (`tests/test_shell.sh`) | 70 |
| Termination cleanup (`tests/test_termination.py`) | 4 |
| **Total per build** | **182** |

## Coverage

- Line coverage: 671 / 883 = **75.99%**
- Branch coverage: 399 / 576 = **69.27%**

The figures reproduce the 3 November measurement exactly. Raw gcov JSON is in
`docs/evidence/coverage/`.

## Limitations

- This is a working-checkout run, not a fresh-clone run of the final candidate.
- Gcov counters in forked children that `exec` or call `_exit` may not flush.
- Fault injection for allocation, `fork`, `wait`, and other syscall failures remains partial.
- No final two-minute video, release re-tag, or joint sign-off is produced here.
- CI has not yet run this branch; CI validation of the expanded suite is pending a pull request.

## Line-ending note

The 3 November evidence hashed `tests/test_executor.c` with CRLF line endings. Git normalizes `*.c`
to LF (`.gitattributes`), so the committed file has the same content with a different SHA-256. Re-runs
above used the committed LF copy and produced the same 45 executor checks.
