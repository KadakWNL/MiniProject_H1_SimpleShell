# Test Report — Simple Shell

## 1. Latest Release & CI Validation Summary (`v1.0-final`)

Following the merge of the resource-audit and termination-cleanup hardening in PR #32 (`main` commit `859b43e`, tagged `v1.0-final`), the full suite runs **four complementary validation harnesses totaling 164 automated checks per build** across `make test` and `make asan-test`, plus Valgrind leak/descriptor analysis via `make valgrind`.

| Suite / Harness | Passed | Failed | Evidence |
|---|---:|---:|---|
| Parser (`tests/test_parser.c`) | 63 | 0 | [Parser tests](../tests/test_parser.c): argument/quote tokenization, operator diagnostics, cleanup after rejection, quoted operators, and the 32/33-stage boundary |
| Executor (`tests/test_executor.c`) | 29 | 0 | [Executor tests](../tests/test_executor.c): external and built-in stages, multi-stage pipelines (3, 4, and 32 stages), rightmost status propagation, intermediate and terminal child waiting, large-stream deadlock avoidance, early consumer exit, and descriptor cleanliness |
| Shell integration (`tests/test_shell.sh`) | 70 | 0 | [Shell tests](../tests/test_shell.sh): syntax error recovery, multi-stage pipelines (3, 4, 5, and 32 stages), pipelines combined with input/output redirections, large-stream deadlock avoidance, asynchronous background reaping, `SIGTERM` cleanup, and regression stress loops |
| Termination cleanup (`tests/test_termination.py`) | 2 | 0 | [Termination harness](../tests/test_termination.py): `SIGTERM` interruption of foreground waits, descendant termination/reaping (`background=False` and `background=True`), and exit status `143` |
| **Total per build (`make test` / `make asan-test`)** | **164** | **0** | Verified on Ubuntu Linux in GitHub Actions [CI Run #77](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/actions/runs/43251661085) (`328` total checks across standard + ASan/UBSan runs) |
| Valgrind audit (`make valgrind`) | Passed | 0 | `valgrind 3.26.0`: `0 errors from 0 contexts` across `test_parser`, `test_executor`, and `echo audit | cat / sleep 30 & / exit` |

**Note on local Windows/MSYS2 runs:** On non-Linux hosts (Windows/MSYS2 emulation), `make asan-test` (`-lasan`/`-lubsan` unavailable), `make valgrind`, `tests/test_termination.py` (`/proc` and POSIX `SIGTERM` exit semantics), and `TC-13` integer-second timing under `fork()` overhead are Linux-specific; Linux (`ubuntu-latest` CI runner) is the target validation environment per SRS Section 2.4.

---

## 2. Historical Sprint 2 Validation (Multi-Stage Pipelines & Full Regression)

Historical validation of issues #13 (`[S2-04] Multi-Stage Pipeline Expansion & Deadlock Prevention`) and #16 (`[S2-07] Full Regression Testing, RTM Traceability Sync & Test Report`)
on branch `nov-w1-work` (prior to PR #32 adding the 70th integration case and the 2-check Python termination harness) used Ubuntu Linux 24.04 LTS / WSL2, GCC 13.3.0,
and the strict C11 flags in the [Makefile](../Makefile). `make test` passed on the first run.
`make asan-test` passed on rerun with zero failures and no sanitizer diagnostics. Its first run
had one flaky foreground synchronization timing assertion; the repeated run passed.

| Suite | Passed | Evidence |
|---|---:|---|
| Parser | 63 | [Parser tests](../tests/test_parser.c): operator diagnostics, cleanup after rejection, quoted operators, and the 32/33-stage boundary |
| Executor | 29 | [Executor tests](../tests/test_executor.c): external and built-in stages, multi-stage pipelines (3, 4, and 32 stages), rightmost status propagation, intermediate and terminal child waiting, large-stream deadlock avoidance, early consumer exit, and descriptor cleanliness |
| Shell integration | 69 | [Shell tests](../tests/test_shell.sh): syntax error recovery, multi-stage pipelines (3, 4, 5, and 32 stages), pipelines combined with input/output redirections, large-stream deadlock avoidance, asynchronous background process reaping, and regression stress loops |
| Historical total per build | 161 | `make test` and the successful `make asan-test` rerun (322 checks across successful runs) |

The executor harness tests multi-stage pipelines with 3, 4, and up to 32 concurrent stages.
It verifies concurrent execution on high data volumes (`seq 50000 | cat | wc -l`), ensuring
no pipe buffer deadlock occurs when data transfers exceed the 64KB OS kernel pipe capacity.
It also verifies early consumer exit handling (`yes | cat | head -n 1`), ensuring upstream
writers exit cleanly on SIGPIPE without deadlocks or hangs.
The executor harness also executes 100 2-stage and 50 4-stage pipelines under a soft file-descriptor
limit of 64, checks that the open-descriptor count is unchanged, and verifies
that no foreground children remain to reap. A 30-second alarm makes any pipeline
hang immediately fail the executor test process. Waiting for each stage (including intermediate
stages in multi-stage pipelines) is verified using file side effects independently of stdout capture.

The shell integration harness tests end-to-end multi-stage pipelines with built-in commands
and external utilities, pipelines combined with `<` input and `>` output redirections,
subshell built-in directory/exit isolation, and full automated regression stress cycles (TC-17).

## 3. Status

**Validation runs completed successfully and verified under automated CI.**
This document records the validation evidence produced across local Linux/WSL environments and verified in the automated GitHub Actions CI workflow for Issue #8 ([S1-08] GitHub Actions CI Automation for PRs, Commits & Sanitizer Checks), Resource Audit Issue #15 ([PR #32](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/32)), and release `v1.0-final`.

## 4. Target environment for validation

| Property | Required | Actual CI / Local Environment |
|---|---|---|
| OS | Linux, x86_64 | Ubuntu (`ubuntu-latest` CI runner) / Ubuntu 24.04 LTS WSL2 (6.6.87.2) |
| Compiler | GCC, C11 mode | GCC in `-std=c11` mode (`gcc 13.3.0` on Ubuntu 24.04 LTS) |
| Build flags | `-std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes` | Exact flags applied in Makefile |
| Sanitizers | AddressSanitizer + UndefinedBehaviorSanitizer | `-fsanitize=address,undefined -fno-omit-frame-pointer -g3` |
| Valgrind | Memory & leak analyzer | `valgrind 3.26.0` on `ubuntu-latest` CI runner |
| Shell & Python | bash, python3 | GNU bash 5.2.21, Python 3 |

## 5. How to produce the evidence

```bash
make clean
make all
make test
make asan-test
make valgrind
```

Or execute the consolidated CI target (builds standard and runs `make test` + `make asan-test`):

```bash
make ci
```

## 6. Recorded validation

### Historical run (Ubuntu 24.04 LTS WSL2, GCC 13.3.0 — Sprint 2 baseline before PR #32)

| Command | Parser | Executor | Shell integration | Result |
|---|---:|---:|---:|---|
| `make test` | 63 passed, 0 failed | 29 passed, 0 failed | 69 passed, 0 failed | Passed (161 checks) |
| `make asan-test` (first run) | 63 passed, 0 failed | 29 passed, 0 failed | 68 passed, 1 failed | Foreground synchronization timing assertion reported an anomalous `-100s` duration |
| `make asan-test` (rerun) | 63 passed, 0 failed | 29 passed, 0 failed | 69 passed, 0 failed | Passed (161 checks); no sanitizer diagnostics |

The timing failure did not reproduce on the complete sanitizer rerun and was later hardened deterministically via marker-file synchronization in PR #31 (`dc44a8b`). The historical initial sanitizer failure is retained here rather than omitted.

### Historical CI run (GitHub Actions `ubuntu-latest`, post-`v1.0-final` — CI Run #77)

| Command | Parser | Executor | Shell integration | Termination (`test_termination.py`) | Result |
|---|---:|---:|---:|---:|---|
| `make test` | 63 passed, 0 failed | 29 passed, 0 failed | 70 passed, 0 failed | 2 passed, 0 failed | Passed (164 checks) |
| `make asan-test` | 63 passed, 0 failed | 29 passed, 0 failed | 70 passed, 0 failed | 2 passed, 0 failed | Passed (164 checks); no sanitizer diagnostics |
| `make valgrind` | `0 errors` | `0 errors` | `0 errors` (`echo audit \| cat / sleep 30 & / exit`) | N/A (tested under ASan/UBSan) | Passed (0 leaks, 0 errors across all contexts) |

### Confirmed PR #34 CI run (GitHub Actions `ubuntu-latest`, PR #34 head `1c23b8b1` — CI Run #82)

[CI Run #82](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/actions/runs/44011706095) completed successfully on the PR #34 head branch before merging into `main`:
- `make test`: 63 parser + 29 executor + 70 shell integration + 2 termination (`test_termination.py`) = 164 passed, 0 failed.
- `make asan-test`: 164 passed, 0 failed (zero ASan/UBSan diagnostics).
- `make valgrind`: 0 errors from 0 contexts, 0 bytes definitely lost.

### Latest Main CI run (GitHub Actions `ubuntu-latest`, current `main` `a5bf25ec` — CI Run #83)

Executed on Ubuntu 26.04.1 LTS (`gcc 15.2.0`, kernel `7.0.0-1019-azure`) in [CI Run #83](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/actions/runs/44012102281):

| Command | Parser | Executor | Shell integration | Termination (`test_termination.py`) | Result |
|---|---:|---:|---:|---:|---|
| `make test` | 63 passed, 0 failed | 29 passed, 0 failed | 70 passed, 0 failed | 2 passed, 0 failed | Passed (164 checks) |
| `make asan-test` | 63 passed, 0 failed | 29 passed, 0 failed | 70 passed, 0 failed | 2 passed, 0 failed | Passed (164 checks); zero sanitizer diagnostics |
| `make valgrind` | `0 errors` | `0 errors` | `0 errors` (`echo audit \| cat / sleep 30 & / exit`) | N/A (tested under ASan/UBSan) | Passed (167 Valgrind reports with 0 errors; 164 leak summaries with 0 bytes definitely lost) |

## 7. Reproduction

```bash
make test
make asan-test
make valgrind
```

## 8. Traceability to Quality Requirements

- **NFR-01 (Reliability):** Regression procedures and pipeline cases in TC-01 through TC-18 were exercised with 164 automated checks per successful build (161 historical Sprint 2 baseline + 3 termination/SIGTERM checks added in PR #32) across standard and sanitizer modes.
- **NFR-03 (Memory Safety):** Clean execution under AddressSanitizer, UndefinedBehaviorSanitizer, and Valgrind with zero memory leaks, zero buffer overflows, and verified file descriptor hygiene under tight rlimits.
- **NFR-05 (Portability):** Verified clean compilation under strict standard GCC C11 flags (`-std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes`) with LF normalized line endings across platforms.
- **NFR-06 (Testability):** Functional behavior, multi-stage pipelines, error recovery, and signal-driven termination cleanup have automated regression coverage in the project test suite and CI workflow.

## 9. Part-3 expanded testing (test-only change)

Part 3 adds lifecycle and resource-failure coverage without changing production C code, which remains
at `v1.0-final` (`859b43e`). The expanded executor and termination harnesses raise the suite from 164
to **182 checks per build** (parser 63, executor 45, shell 70, termination 4); test cases TC-19, TC-20,
and TC-21 record the new scenarios in [Test_Cases.md](Test_Cases.md).

| Report | Result | Location |
|---|---|---|
| Unit Test Report | 108 passed (parser 63, executor 45), standard, sanitizer, and coverage builds | [Unit_Test_Report.md](Unit_Test_Report.md) |
| System Validation Report | 74 passed (shell 70, termination 4) | [System_Validation_Report.md](System_Validation_Report.md) |
| Code Coverage Report | 671 / 883 lines = 75.99% | [Code_Coverage_Report.md](Code_Coverage_Report.md) |
| Branch Coverage Report | 399 / 576 branch edges = 69.27% | [Branch_Coverage_Report.md](Branch_Coverage_Report.md) |

The suite was re-executed on 17 November 2026 after integration onto the IEEE-restructured `main`:
`make test` and `make asan-test` each passed 182 checks, `make valgrind` reported 0 errors, and
`make coverage` reproduced the percentages above. Raw logs are in
[`docs/evidence/verify-2026-11-17/`](evidence/verify-2026-11-17/summary.md). CI validation of the
expanded suite is pending a pull request; this section does not claim a CI run for it.

The first CI run for the Part-3 pull request exposed one flaky assertion rather than a product
failure: TC-13 compared a 2-second background `sleep` against a strict 2-second wall-clock bound, and
sanitizer startup pushed it to 2 seconds. The check now launches a 30-second background job and
asserts that the prompt returns within 5 seconds, which keeps the non-blocking assertion while
removing the dependence on sanitizer startup time.
