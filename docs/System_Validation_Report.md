# System Validation Report - Simple Shell

Date: 3 November 2026. Re-verified: 17 November 2026 (integration onto the IEEE-restructured `main`). Production revision: `859b43ea8847b9abff28b9ea2c537b4c593deb5e`. Status: automated Linux system validation passed within the stated scope; manual acceptance pending. Environment and exact tested file hashes: [manifest](evidence/Validation_Manifest.md); integration re-run: [re-verification summary](evidence/verify-2026-11-17/summary.md).

## System and acceptance criteria

The system is the compiled shell with Linux processes, files, signals and standard utilities. Inputs enter stdin, results/errors are captured, and temporary filesystem/process state supplies independent oracles. The suite validates FR-01-FR-10 against [Test_Cases](Test_Cases.md) and the [RTM](RTM.md). Tests assert output, diagnostics, continued execution after errors, foreground waits, background return/reaping and resource lifecycle; no broad security certification is claimed.

## Results

| Scenario family | Standard | ASan/UBSan | Coverage build |
|---|---:|---:|---:|
| Bash shell integration | 70 passed, 0 failed | 70 passed, 0 failed | 70 passed, 0 failed |
| Python SIGTERM regressions | 4 passed, 0 failed | 4 passed, 0 failed | 4 passed, 0 failed |
| Total | 74 passed | 74 passed | 74 passed |

Coverage includes all five built-ins, external commands, quotes/escapes, `<`, `>`, `>>`, 2-32-stage pipelines, large streams, early consumer exit, invalid syntax with no process/file side effects, prompt/EOF/exit, missing command/file recovery and background SIGCHLD notifications. The Bash SIGTERM case verifies idle-shell cleanup; the Python cases check SIGTERM during foreground waiting and background descendant execution with bounded waits.

Raw evidence: [standard](evidence/standard.log), [sanitizers](evidence/sanitizers.log), [coverage build](evidence/coverage/test.log). A Valgrind shell smoke additionally exercises pipeline/background/exit and reports zero errors and zero live heap bytes: [log](evidence/valgrind.log). This smoke is not full Valgrind system coverage.

## Disposition and remaining acceptance

The first expanded Valgrind run hit the old 30-second harness watchdog (retained in evidence/valgrind-first-expanded-timeout.log); the final run uses a bounded 120-second watchdog. The final standard/sanitizer/system runs passed; prior timing failure/retest is preserved in [historical report](Test_Report_Pre_Audit.md). Tests run on one Linux/WSL environment in the existing checkout; fresh-clone verification of a final committed candidate remains for joint review. Automated EOF/input tests do not establish full interactive Ctrl-D/terminal-job-control behavior. Nonblocking timing checks remain environment-sensitive. Detached descendants, SIGKILL cleanup, injected syscall failures and a hostile executable are outside assurance.

Manual prompt/usability review, final two-minute outcome video, final release tag/freeze (#18), and both reviewers approving the same final commit (#30) remain open. The optional spillover Sprint 3 is allowed by the Part-3 handout for 2-6 November; original Sprint 1/2 story assignments and actual completion dates are preserved.

The four Python termination scenarios include two children that explicitly ignore SIGTERM before publishing readiness. They verify the grace period, bounded shutdown and no live fixture child afterward, exercising both foreground and background forced-cleanup paths.

## Expanded-fixture failure and retest record

The first expanded Valgrind run hit the 30-second watchdog; increasing the bound alone did not fully resolve the issue. A subsequent run showed the background fixture's signal could arrive before an instrumented child had exec'd sleep, allowing the finite-lived fixtures to expire and invalidating the out-of-order assertions. These failed logs are retained as `docs/evidence/valgrind-first-expanded-timeout.log` and `docs/evidence/valgrind-expanded-fixture-race.log`.

The fixture now waits for `/proc/<pid>/exe` to identify sleep before signaling, with a bounded readiness wait. This changes test coordination, not executor behavior. The executor watchdog remains bounded at 120 seconds for instrumented execution. A focused Valgrind retest passed, followed by the final complete standard/sanitizer/Valgrind/coverage rerun. The stdout restoration oracle additionally compares descriptor identities immediately after execute_pipeline, before the harness restores stdout itself. No production source was changed.
