# Unit Test Report - Simple Shell

Date: 3 November 2026. Re-verified: 17 November 2026 (integration onto the IEEE-restructured `main`). Status: locally executed, pending peer review. Production revision: `859b43ea8847b9abff28b9ea2c537b4c593deb5e`. Exact input hashes, environment, commands and scope: [validation manifest](evidence/Validation_Manifest.md); integration re-run: [re-verification summary](evidence/verify-2026-11-17/summary.md).

## Scope and method

Parser unit tests exercise tokenization, quotes/escapes, operator validation, allocation ownership on rejection, and argument/stage limits. Direct executor tests are component tests using real processes/files, not mocked unit tests. They exercise built-ins/external pipelines, redirection, rightmost status, parent-state isolation, concurrent transfer, early consumer exit, intermediate/terminal waits, child reaping and descriptor stability.

## Results

| Harness | Standard | ASan/UBSan | Coverage build |
|---|---:|---:|---:|
| Parser | 63 passed, 0 failed | 63 passed, 0 failed | 63 passed, 0 failed |
| Executor/component | 45 passed, 0 failed | 45 passed, 0 failed | 45 passed, 0 failed |
| Total | 108 passed | 108 passed | 108 passed |

Evidence: [standard](evidence/standard.log), [sanitizers](evidence/sanitizers.log), [instrumented](evidence/coverage/test.log). `make valgrind` additionally ran both harnesses with zero reported memory errors; see [Valgrind log](evidence/valgrind.log). Compiler warning flags were enabled; no compiler warnings were observed.

## Defects, retests and limits

The first expanded Valgrind run hit the former 30-second executor watchdog. The harness watchdog is now 120 seconds to accommodate instrumented execution; the failed log is retained in evidence/valgrind-first-expanded-timeout.log. The complete sequence was rerun after that adjustment. PR #32's previously fixed SIGTERM and unused-function findings are traced in Resource_Audit.md and the merged history. The current run is a post-fix regression, not a claim to have repeated the original red/green experiment. Earlier anomalous timing/retest evidence remains in [historical report](Test_Report_Pre_Audit.md).

The descriptor-exhaustion regression induces a real second-pipe EMFILE failure after a child has started and verifies cleanup/recovery. Other syscall/allocation errors are not injected exhaustively; child coverage is limited by gcov flushing. Structural coverage is separately measured in [Code Coverage](Code_Coverage_Report.md) and [Branch Coverage](Branch_Coverage_Report.md). Functional test success does not imply 100% coverage. Requirement mapping: [RTM](RTM.md); procedures: [Test Plan](../2-Test-Planning/Test_Plan.md). No release approval is inferred.

## Expanded executor cases

Sixteen added assertions cover builtin input and combined redirection, input/output restoration after success and failure, 128+signal status, three background jobs reaped out of order, concurrent foreground/background reaping, final tracking cleanup, and real EMFILE partial-pipeline cleanup/descriptor stability/recovery. Tests use actual files, processes and RLIMIT_NOFILE rather than mocked return values. Production C code is unchanged.

## Expanded-fixture failure and retest record

The first expanded Valgrind run hit the 30-second watchdog; increasing the bound alone did not fully resolve the issue. A subsequent run showed the background fixture's signal could arrive before an instrumented child had exec'd sleep, allowing the finite-lived fixtures to expire and invalidating the out-of-order assertions. These failed logs are retained as `docs/evidence/valgrind-first-expanded-timeout.log` and `docs/evidence/valgrind-expanded-fixture-race.log`.

The fixture now waits for `/proc/<pid>/exe` to identify sleep before signaling, with a bounded readiness wait. This changes test coordination, not executor behavior. The executor watchdog remains bounded at 120 seconds for instrumented execution. A focused Valgrind retest passed, followed by the final complete standard/sanitizer/Valgrind/coverage rerun. The stdout restoration oracle additionally compares descriptor identities immediately after execute_pipeline, before the harness restores stdout itself. No production source was changed.
