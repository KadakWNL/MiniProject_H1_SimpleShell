# Validation manifest - 3 November 2026

## Identity and environment

Production source revision: `859b43ea8847b9abff28b9ea2c537b4c593deb5e` (main, PR #32 merged). Production C files and headers are unchanged by this candidate. The candidate adds expanded executor/termination tests, documentation, evidence, and a coverage target/report generator; it is prepared on branch `part-3/integrate-testing-package` and has not been merged to `main`. Exact input hashes and tool versions are in [environment.txt](environment.txt). No future commit SHA or reviewer approval is fabricated.

Environment: Kali Linux under WSL2, Linux 6.6.87.2-microsoft-standard-WSL2 x86_64; GCC/gcov 16.2.0; Python 3.14.7. The full environment capture is authoritative for the remaining tools. Tests ran in the working checkout, not a fresh clone. Build variants ran sequentially, each with clean rebuilds.

## Executed commands and results

| Command | Exit | Evidence |
|---|---:|---|
| `make clean && make test` | 0 | [standard.log](standard.log) |
| `make asan-test` | 0 | [sanitizers.log](sanitizers.log) |
| `make clean && make valgrind` | 0 | [valgrind.log](valgrind.log) |
| `make coverage` | 0 | [command log](coverage-command.log), [tests](coverage/test.log), [gcov](coverage/gcov.log), compressed gcov JSON in coverage/ |

Standard, sanitizer and instrumented runs each passed 63 parser assertions, 45 executor checks, 70 shell scenarios and four Python termination scenarios (182 total). These are the final successful reruns. An initial expanded Valgrind run hit the prior 30-second executor watchdog; [the failed log](valgrind-first-expanded-timeout.log) is preserved. The harness now has a bounded 120-second watchdog, and the complete sequence was repeated. Historical tests/retests are retained in [the earlier report](../Test_Report_Pre_Audit.md); they are not substituted for this run.

## Interpretation and assurance limits

No compiler warnings appeared in these runs. Sanitizers reported no errors in covered execution; Valgrind reported zero errors and zero bytes live at exit for the three top-level parser/executor/shell processes. Forked builtin children report inherited allocations as still reachable (for example 143 bytes in the shell smoke), with zero definitely/indirectly/possibly lost bytes. Still reachable is not the same as a lost allocation; the log is retained rather than claiming every child frees its inherited heap. These are observations, not proof of universal leak freedom. Valgrind does not run the full shell or SIGTERM suite, and external programs are not instrumented for coverage.

Gcov records instrumented production lines and branch edges only. Fork/exec/_exit children may not flush counters. Fault injection for allocation/fork/wait failures, detached process groups, interactive terminal job control, manual usability and instructor acceptance are not established by these runs. The harness accepts a descendant zombie as no longer running, so the termination regression does not claim the shell reaps grandchildren. See [resource audit](../Resource_Audit.md).

No final two-minute video, clean-clone verification at the final candidate, or joint sign-off was produced here. The `v1.0-final` release tag predates the documentation restructure and the expanded test suite.

## Integration re-verification (17 November 2026)

After the test-only changes were applied onto the IEEE-restructured `main`, the complete sequence was
re-run: `make test` and `make asan-test` each passed 182 checks, `make valgrind` reported zero errors,
and `make coverage` reproduced 75.99% line and 69.27% branch coverage. The new logs and a summary are
in [verify-2026-11-17/](verify-2026-11-17/summary.md). The 3 November logs above remain the original
record. Git normalizes `*.c` to LF, so the committed `tests/test_executor.c` differs from the
3 November hash only in line endings.

## Before/after evidence

Original logs and coverage JSON/reports are preserved under baseline/. Baseline: 628/883 lines (71.12%), 364/576 outgoing branch edges (63.19%), with 29 executor checks and two termination cases. The expanded suite has 45 executor checks and four termination cases. Production code is unchanged; refreshed environment.txt records the exact locally modified test hashes.

## Expanded-fixture failure and retest record

The first expanded Valgrind run hit the 30-second watchdog; increasing the bound alone did not fully resolve the issue. A subsequent run showed the background fixture's signal could arrive before an instrumented child had exec'd sleep, allowing the finite-lived fixtures to expire and invalidating the out-of-order assertions. These failed logs are retained as `docs/evidence/valgrind-first-expanded-timeout.log` and `docs/evidence/valgrind-expanded-fixture-race.log`.

The fixture now waits for `/proc/<pid>/exe` to identify sleep before signaling, with a bounded readiness wait. This changes test coordination, not executor behavior. The executor watchdog remains bounded at 120 seconds for instrumented execution. A focused Valgrind retest passed, followed by the final complete standard/sanitizer/Valgrind/coverage rerun. The stdout restoration oracle additionally compares descriptor identities immediately after execute_pipeline, before the harness restores stdout itself. No production source was changed.

Expanded executor results: 75.99% overall lines and 69.27% branch edges; executor 72.30% lines and 61.50% branch edges. [Before/after and five test groups](../Executor_Coverage_Review.md).
