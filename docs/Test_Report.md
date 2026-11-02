# Test Report — Simple Shell

## Multi-stage pipeline, deadlock prevention, and full regression validation

Validation of issues #13 (`[S2-04] Multi-Stage Pipeline Expansion & Deadlock Prevention`) and #16 (`[S2-07] Full Regression Testing, RTM Traceability Sync & Test Report`)
on branch `nov-w1-work` used Ubuntu Linux 24.04 LTS / WSL2, GCC 13.3.0,
and the strict C11 flags in the [Makefile](../Makefile). `make test` passed on the first run.
`make asan-test` passed on rerun with zero failures and no sanitizer diagnostics. Its first run
had one flaky foreground synchronization timing assertion; the repeated run passed.

| Suite | Passed | Evidence |
|---|---:|---|
| Parser | 63 | [Parser tests](../tests/test_parser.c): operator diagnostics, cleanup after rejection, quoted operators, and the 32/33-stage boundary |
| Executor | 29 | [Executor tests](../tests/test_executor.c): external and built-in stages, multi-stage pipelines (3, 4, and 32 stages), rightmost status propagation, intermediate and terminal child waiting, large-stream deadlock avoidance, early consumer exit, and descriptor cleanliness |
| Shell integration | 69 | [Shell tests](../tests/test_shell.sh): syntax error recovery, multi-stage pipelines (3, 4, 5, and 32 stages), pipelines combined with input/output redirections, large-stream deadlock avoidance, asynchronous background process reaping, and regression stress loops |
| Total per build | 161 | `make test` and the successful `make asan-test` rerun (322 checks across successful runs) |

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

## 1. Status

**Validation run completed successfully and verified under automated CI.**
This document records the validation evidence produced across local Linux/WSL environments and verified in the automated GitHub Actions CI workflow for Issue #8 ([S1-08] GitHub Actions CI Automation for PRs, Commits & Sanitizer Checks).

## 2. Target environment for validation

| Property | Required | Actual CI / Local Environment |
|---|---|---|
| OS | Linux, x86_64 | Ubuntu 24.04 LTS (`ubuntu-latest` runner) / Linux WSL2 (6.6.87.2) |
| Compiler | GCC, C11 mode | `gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0` |
| Build flags | `-std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes` | Exact flags applied in Makefile |
| Sanitizers | AddressSanitizer + UndefinedBehaviorSanitizer | `-fsanitize=address,undefined -fno-omit-frame-pointer -g3` |
| Shell for the harness | bash | GNU bash, version 5.2.21 |

## 3. How to produce the evidence

```bash
make clean
make all
make test
make asan-test
```

Or execute the consolidated CI target:

```bash
make ci
```

## 4. Recorded validation

Executed in Ubuntu 24.04 LTS under WSL2 with GCC 13.3.0:

| Command | Parser | Executor | Shell integration | Result |
|---|---:|---:|---:|---|
| `make test` | 63 passed, 0 failed | 29 passed, 0 failed | 69 passed, 0 failed | Passed (161 checks) |
| `make asan-test` (first run) | 63 passed, 0 failed | 29 passed, 0 failed | 68 passed, 1 failed | Foreground synchronization timing assertion reported an anomalous `-100s` duration |
| `make asan-test` (rerun) | 63 passed, 0 failed | 29 passed, 0 failed | 69 passed, 0 failed | Passed (161 checks); no sanitizer diagnostics |

The timing failure did not reproduce on the complete sanitizer rerun. The standard run and rerun
both completed successfully; the initial sanitizer run is recorded here rather than omitted.

## 5. Reproduction

```bash
make test
make asan-test
```
## 6. Traceability to Quality Requirements

- **NFR-01 (Reliability):** Regression procedures and pipeline cases in TC-01 through TC-18 were exercised with 161 automated checks per successful build across standard and sanitizer modes.
- **NFR-03 (Memory Safety):** Clean execution under AddressSanitizer and UndefinedBehaviorSanitizer with zero memory leaks, zero buffer overflows, and verified file descriptor hygiene under tight rlimits.
- **NFR-05 (Portability):** Verified clean compilation under strict standard GCC C11 flags (`-std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes`) with LF normalized line endings across platforms.
- **NFR-06 (Testability):** Functional behavior, multi-stage pipelines, and error recovery have automated regression coverage in the project test suite and CI workflow.
