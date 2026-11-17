# Final Project Audit, Acceptance Review and Joint Sign-off Report

- **Project:** Simple Shell (Team H1)
- **Language / Standard:** ISO C11 (`gcc -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes`)
- **Target OS:** Linux / POSIX.1-2008 (tested on Ubuntu 26.04.1 LTS x86_64)
- **Repository:** https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell
- **Candidate Commit SHA for Acceptance:** `10794b271f332316a2a60645597e015e15d48074`
- **Review Date:** 17 November 2026
- **Tracking Issue:** [#30 — Joint final project review and sign-off](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/30)

---

## 1. Executive Summary

Team H1 has completed all planned functional requirements (FR-01 through FR-10), non-functional requirements (NFR-01 through NFR-08), security objectives/requirements (SO-01..03, SR-01..04), sprint deliverables across Sprint 1, Sprint 2, and Sprint 3 spillover, and final release deliverables.

This comprehensive audit independently verified:
1. Every merged Pull Request (PRs #20 through #35) on the `main` branch.
2. Full closure of all prerequisite user stories and final check issues (#13, #15, #16, and #18).
3. Fresh-clone build and execution on Windows/MSYS2 and native Ubuntu Linux via GitHub Actions [CI Run #87](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/actions/runs/44015713358).
4. All four automated test suites (**182 checks per build** across `make test` and `make asan-test`, with zero failures and zero sanitizer diagnostics, plus zero errors under `make valgrind`).
5. Direct inspection of both video demonstration files (`docs/demos/simple-shell-sprint1-demo.mp4` and `docs/demos/se-compressed.mp4`) and all deliverable documentation directories.
6. Commit tree consistency between release tag `v1.0-final` (`859b43ea8847b9abff28b9ea2c537b4c593deb5e`) and current `main` (`10794b271f332316a2a60645597e015e15d48074`), confirming zero production C changes post-tag; the Part-3 expansion changes tests and the Makefile only.

Issue #30 is in the final sign-off stage, requiring explicit personal sign-off from both assigned reviewers (`@KadakWNL` and `@ShrikarDesai07`) against commit `10794b271f332316a2a60645597e015e15d48074`.

---

## 2. Accepted Main Commit SHA

- **Current `main` Head SHA:** `10794b271f332316a2a60645597e015e15d48074`
- **Release Baseline Tag (`v1.0-final`):** `859b43ea8847b9abff28b9ea2c537b4c593deb5e`
- **Difference:** No production C changes. Post-tag changes are documentation, diagrams, PDFs, screenshots, demo evidence, and the Part-3 test/coverage expansion (tests and Makefile only).
- **Finding:** **Passed**

---

## 3. Merged Pull Requests Reviewed

All 15 pull requests merged into `main` were independently verified against repository git history and GitHub PR records:

| PR | Issue | Title | Author | Merged At | Merge Commit | Verification Status |
|---|---|---|---|---|---|:---:|
| [#20](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/20) | #19, #4 | Tokenizer, quote parsing, error handling & test suite | @KadakWNL | 2026-10-23 | `9fb60e9` | Passed |
| [#21](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/21) | #7 | Implement built-in commands (cd, pwd, echo, help, exit) | @sudeeksh02 | 2026-10-23 | `199c52a` | Passed |
| [#22](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/22) | #2, #3 | Modular Architecture & REPL Command Loop Setup | @ShrikarDesai07 | 2026-10-23 | `4ffe6b7` | Passed |
| [#23](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/23) | #8 | GitHub Actions CI Automation | @sudeeksh02 | 2026-10-23 | `8bab63d` | Passed |
| [#24](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/24) | #9 | Sprint 1 Validation, Fresh Clone Verification & 1-Minute Demo Video | @ShrikarDesai07 | 2026-10-23 | `341dd55` | Passed |
| [#25](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/25) | #10 | S2-01 I/O Redirection Support (<, >, >>) | @ShrikarDesai07 | 2026-10-28 | `a3a1569` | Passed |
| [#26](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/26) | #5, #6 | External command execution, foreground wait & parser memory teardown | @Aqual456 | 2026-10-28 | `69f0383` | Passed |
| [#27](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/27) | #11, #12 | Validate operators and verify two-stage pipelines | @KadakWNL | 2026-10-28 | `15e8e0e` | Passed |
| [#28](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/28) | #14 | Background command execution and self-pipe zombie reaping | @Aqual456 | 2026-10-28 | `abad16e` | Passed |
| [#29](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/29) | #17 | Documentation Finalization, Architecture Sync & Deviation Log | @ShrikarDesai07 | 2026-10-29 | `34006de` | Passed |
| [#31](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/31) | #13, #16 | Multi-Stage Pipeline, Deadlock Prevention & Regression Testing | @sudeeksh02 | 2026-11-02 | `ab345627` | Passed |
| [#32](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/32) | #15 | Complete comprehensive resource audit | @Aqual456 | 2026-11-03 | `859b43ea` | Passed |
| [#33](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/33) | N/A | IEEE restructure, security design, UML diagrams, and final evidence | @KadakWNL | 2026-11-12 | `fae11022` | Passed |
| [#34](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/34) | #18 | Submit final two-minute demo for Issue #18 | @sudeeksh02 | 2026-11-17 | `a5bf25ec` | Passed |
| [#35](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/pull/35) | N/A (Part-3) | Part-3 expanded suite, coverage reports, and evidence | @KadakWNL | 2026-11-17 | `10794b2` | Passed |

---

## 4. Prerequisite Issue Status

All prerequisites specified in Issue #30 have been completed and verified on GitHub:

- **Issue #13 (`[S2-04]` Multi-Stage Pipeline Expansion & Deadlock Prevention):** Closed on 2 November 2026 via PR #31 (`ab345627`). Verified with 32-stage pipeline tests and deadlock prevention transfers. **Status: Passed**
- **Issue #15 (`[S2-06]` Comprehensive Resource Audit):** Closed on 3 November 2026 via PR #32 (`859b43ea`). Verified with AddressSanitizer, UndefinedBehaviorSanitizer, Valgrind, and file descriptor hygiene tests. **Status: Passed**
- **Issue #16 (`[S2-07]` Full Regression Testing, RTM Traceability Sync & Test Report):** Closed on 2 November 2026 via PR #31 (`ab345627`). Verified with automated regression test runs and RTM sync. **Status: Passed**
- **Issue #18 (`[FinalChecks]` 2-Minute Final Demo Video, Release Tag & Development Freeze):** Closed on 17 November 2026 via PR #34 (`a5bf25ec`). Verified with `docs/demos/se-compressed.mp4` committed on `main` and release `v1.0-final` tagged. **Status: Passed**
- **Issue #30 (`[FinalChecks]` Joint final project review and sign-off):** **Open** (currently in review awaiting joint sign-off).

---

## 5. Fresh-Clone Environment & Results

### 5.1 Local Emulation Environment (Windows / MSYS2)
- **Host:** Windows 11 x86_64
- **Toolchain:** MSYS2 GCC 15.2.0, GNU Make 4.4.1, GNU bash 5.2.21
- **Fresh Clone Directory:** Isolated temporary clone at `scratch/fresh_clone` checked out to `10794b271f332316a2a60645597e015e15d48074`.
- **Build Result (`make clean && make all`):** Passed cleanly, zero compiler warnings.
- **Harness Results (MSYS2):**
  - `./build/test_parser`: **63 passed, 0 failed**
  - `bash tests/test_shell.sh ./simple-shell`: **70 passed, 0 failed**
- **Known Platform Constraints:**
  - The expanded executor harness (`/proc` readiness, `RLIMIT_NOFILE`, `waitid`) and the termination harness are Linux-specific; they are validated on CI, not on MSYS2.
  - `make asan-test` requires Linux `libasan`/`libubsan` runtime (unsupported in MSYS2 GCC).
  - `make valgrind` requires native Linux Valgrind binary (unsupported on Windows).
  - `git diff --check` and `git status --short`: clean working tree.
- **Finding:** **Passed (with documented platform limitations)**

### 5.2 Native Linux CI Environment (GitHub Actions `ubuntu-latest`)
- **OS:** Ubuntu 26.04.1 LTS
- **Kernel:** `Linux runnervmccnms 7.0.0-1019-azure #20-Ubuntu SMP PREEMPT Mon Oct 19 22:41:16 UTC 2026 x86_64 GNU/Linux`
- **Compiler:** `gcc (Ubuntu 15.2.0-16ubuntu1) 15.2.0`
- **CI Run:** [Actions Run #87 (ID: 44015713358)](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/actions/runs/44015713358)
- **Commit Tested:** `10794b271f332316a2a60645597e015e15d48074` (current `main` head)
- **Overall Result:** **Success (all steps)**
- **Working Tree Cleanliness:** Clean working tree confirmed via `git status --porcelain`.
- **Finding:** **Passed**

---

## 6. Test Breakdown and Totals

The project includes four distinct automated test harnesses executed on every build:

| Test Harness | File | Checks | Checks Passed | Checks Failed | Status |
|---|---|---:|---:|---:|:---:|
| 1. Parser Unit Tests | `tests/test_parser.c` | 63 | 63 | 0 | Passed |
| 2. Executor Tests | `tests/test_executor.c` | 45 | 45 | 0 | Passed |
| 3. Shell Integration Tests | `tests/test_shell.sh` | 70 | 70 | 0 | Passed |
| 4. Termination Cleanup | `tests/test_termination.py` | 4 | 4 | 0 | Passed |
| **Total per build (`make test`)** | | **182** | **182** | **0** | **Passed** |
| **Total under ASan/UBSan (`make asan-test`)** | | **182** | **182** | **0** | **Passed** |
| **Consolidated Automated Executions per CI Run** | | **364** | **364** | **0** | **Passed** |

---

## 7. Sanitizer and Valgrind Results

### 7.1 AddressSanitizer & UndefinedBehaviorSanitizer (`make asan-test`)
- **Compiler Flags:** `-fsanitize=address,undefined -fno-omit-frame-pointer -g3`
- **Executed In:** GitHub Actions CI Run #83 (Ubuntu 26.04.1 LTS)
- **Scope:** Complete rerun of all 182 checks across parser, executor, shell integration, and termination harnesses.
- **Diagnostics:** Zero memory errors, zero out-of-bounds accesses, zero use-after-free, zero undefined behavior reports.
- **Finding:** **Passed**

### 7.2 Valgrind Resource & Leak Audit (`make valgrind`)
- **Tool Version:** `valgrind 3.26.0`
- **Flags:** `--leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1`
- **Targets Audited:**
  1. `./build/test_parser`
  2. `./build/test_executor` (including multi-stage pipelines and 150 stress pipelines under `ulimit -n 64`)
  3. Interactive shell session: `printf 'echo audit | cat\nsleep 30 &\nexit\n' | ./simple-shell`
- **Results:**
  - Total `ERROR SUMMARY` reports: 169 (zero errors across all contexts in the 17 November re-run)
  - Total `definitely lost`: **0 bytes in 0 blocks** across all Valgrind contexts
  - Total `indirectly lost`: **0 bytes in 0 blocks**
  - Total `possibly lost`: **0 bytes in 0 blocks**
  - Open file descriptors at test completion: normal standard streams (`0`, `1`, `2`) only.
- **Finding:** **Passed**

---

## 8. Requirements Traceability Audit (FR & NFR)

### 8.1 Functional Requirements (FR-01 to FR-10)

| ID | Requirement | SRS Ref | Implementation | Verification | CI Evidence | Status |
|---|---|---|---|---|---|:---:|
| **FR-01** | Interactive Input | SRS §3.2 | `src/shell.c` | TC-01, TC-10 | Run #87 Step 5 & 6 | Passed |
| **FR-02** | External Execution | SRS §3.2 | `src/executor.c` | TC-02 | Run #87 Step 5 & 6 | Passed |
| **FR-03** | Built-in Commands | SRS §3.2 | `src/builtins.c` | TC-03, TC-04, TC-18 | Run #87 Step 5 & 6 | Passed |
| **FR-04** | Command Parsing | SRS §3.2 | `src/parser.c` | TC-05, TC-06, TC-11 | Run #87 Step 5 & 6 | Passed |
| **FR-05** | I/O Redirection | SRS §3.2 | `src/parser.c`, `src/executor.c` | TC-07, TC-08, TC-09 | Run #87 Step 5 & 6 | Passed |
| **FR-06** | Pipelines (up to 32) | SRS §3.2 | `src/parser.c`, `src/executor.c` | TC-12 | Run #87 Step 5 & 6 | Passed |
| **FR-07** | Background Execution | SRS §3.2 | `src/shell.c`, `src/executor.c` | TC-13, TC-19 | Run #87 Step 5 & 6 | Passed |
| **FR-08** | Error Handling | SRS §3.2 | All modules | TC-11, TC-14, TC-15, TC-21 | Run #87 Step 5 & 6 | Passed |
| **FR-09** | Process Sync | SRS §3.2 | `src/executor.c` | TC-16 | Run #87 Step 5 & 6 | Passed |
| **FR-10** | Clean Termination | SRS §3.2 | `src/shell.c`, `src/builtins.c` | TC-01, TC-10, TC-19, TC-20 | Run #87 Step 5 & 6 | Passed |

### 8.2 Non-Functional Requirements (NFR-01 to NFR-08)

| ID | Requirement | SRS Ref | Verification Procedure | Method / Evidence | Status |
|---|---|---|---|---|:---:|
| **NFR-01** | Reliability | SRS §3.3 | NFRV-01, TC-17 | 182 automated checks, 20-cycle pipeline stress loops | Passed |
| **NFR-02** | Maintainability | SRS §3.3 | NFRV-02 | Modular separation (`src/`, `include/`), no cyclic includes | Passed |
| **NFR-03** | Memory Safety | SRS §3.3 | NFRV-03, TC-21 | Clean ASan/UBSan run and zero Valgrind leaks | Passed |
| **NFR-04** | Usability | SRS §3.3 | NFRV-04 | Standard prompt `simple-shell$ `, standard diagnostics on stderr | Passed |
| **NFR-05** | Portability | SRS §3.3 | NFRV-05 | Strict ISO C11 flags on Linux/POSIX.1-2008, MSYS2 compatibility | Passed |
| **NFR-06** | Testability | SRS §3.3 | NFRV-06 | 4 automated test suites running under CI on every push/PR | Passed |
| **NFR-07** | Security Hygiene | SRS §3.3 | NFRV-07 | Zero `system()` calls, `execvp` array execution, operator validation | Passed |
| **NFR-08** | Documentation | SRS §3.3 | NFRV-08 | IEEE 29148 SRS, IEEE 829 Test Plan, IEEE 42010/1016 SAD, 0 broken links | Passed |

### 8.3 Security Objectives & Requirements (SO-01..03, SR-01..04)
- **SO-01 — Prevent command injection and unintended interpretation / SR-01, SR-02:** No `system()`; commands run via `execvp(argv[0], argv)`; operator syntax is validated before any `fork` or file side effect. **Status: Passed**
- **SO-02 — Preserve memory and resource integrity under hostile input / SR-03:** Parser memory and descriptors are released on all paths; ASan/UBSan clean; `MAX_ARGS = 128` and `MAX_PIPE_SEGMENTS = 32`. **Status: Passed**
- **SO-03 — Fail safely and remain available / SR-04:** Errors go to stderr and the REPL continues; `SIGTERM` teardown and `SIGCHLD` self-pipe reaping do not depend on unsafe signal-context work. **Status: Passed**

---

## 9. Functional Feature Review

Detailed inspection of C source files (`src/*.c`, `include/*.h`) and corresponding test harnesses confirms:

1. **Interactive loop & EOF:** `shell_loop()` in `src/shell.c` correctly displays `simple-shell$ ` on stderr, reads input via `poll()` multiplexing, handles `Ctrl-D` / EOF by returning clean exit status 0.
2. **Built-in commands:** `cd`, `pwd`, `echo`, `help`, and `exit` in `src/builtins.c` operate in the parent process for standalone invocations and support `~` (`HOME`) and `-` (`OLDPWD`).
3. **External command execution:** `execute_pipeline()` / `execute_children()` in `src/executor.c` forks, executes via `execvp()`, handles missing commands with 127, and reaps foreground status via `waitpid()`.
4. **Quoting & backslash escaping:** `src/parser.c` preserves literal content inside `'...'`, handles backslash escapes inside `"..."`, and treats quoted operators (`"|"`, `">"`, `"<"`, `"&"`) as literal arguments.
5. **Syntax validation & safe error recovery:** Parser rejects duplicate operators (`||`, `&&`, `< <`, `> >`) and dangling operators without executing commands or mutating files; `shell_loop()` recovers smoothly.
6. **Input redirection (`<`):** `apply_redirection()` opens `O_RDONLY` and redirects via `dup2()`.
7. **Output overwrite redirection (`>`):** Opens `O_WRONLY | O_CREAT | O_TRUNC` with `0666` mode and redirects stdout.
8. **Output append redirection (`>>`):** Opens `O_WRONLY | O_CREAT | O_APPEND` and appends output.
9. **Multi-stage pipelines:** `execute_pipeline()` creates intermediate pipes up to 32 stages, running all stages concurrently.
10. **Deadlock prevention:** Closes all unused pipe ends in parent and children immediately after forking; verified with large transfers (`seq 50000 | cat | wc -l`).
11. **Foreground synchronization:** Parent waits for all foreground children and propagates the exit code of the rightmost stage.
12. **Background execution (`&`):** Trailing `&` bypasses foreground waiting, adds PID to background job table, and returns prompt immediately.
13. **Asynchronous zombie reaping:** `SIGCHLD` handler writes token to internal self-pipe; `shell_loop()` drains pipe and calls `reap_background()`, printing notifications to stderr without interrupting active input.
14. **SIGTERM cleanup:** `handle_sigterm()` triggers `cleanup_background_jobs()`, terminating all registered background child processes.
15. **Memory and descriptor cleanup:** `free_pipeline()` frees AST memory; parent builtin redirection restores original file descriptors; descriptor counts remain constant under stress.

- **Finding:** **Passed**

---

## 10. Demo Deliverables Verification

Both required video recordings exist on the `main` branch, were directly inspected, and match documented durations and scripts:

### 10.1 Sprint 1 Working-Product Demonstration
- **File:** `docs/demos/simple-shell-sprint1-demo.mp4`
- **Size:** 715,828 bytes (0.68 MB)
- **Duration:** 58.10 seconds (conforms to ~1-minute requirement)
- **Stream Format:** H.264 video, 954x1020, 30 fps
- **Features Demonstrated:** Launch, `pwd`, `cd /tmp`, quoted `echo`, `date`, `ls`, error diagnostic and recovery, `help`, `exit`.
- **Documentation:** `docs/demos/README.md`, `9-Demo/Sprint_1_Demo_Script.md`, output screenshots in `9-Demo/screenshots/`.
- **Finding:** **Passed**

### 10.2 Final Sprint 2 Working-Product Demonstration
- **File:** `docs/demos/se-compressed.mp4`
- **Size:** 18,962,552 bytes (18.08 MB)
- **Duration:** 118.63 seconds (1:58.63, strictly conforms to < 2-minute requirement)
- **Stream Format:** H.264 video, 1920x1020, 30 fps
- **Features Demonstrated:** Launch and prompt, built-in commands (`help`, `pwd`), quoted input, redirection overwrite and append (`>demo.txt`, `>>demo.txt`, `< demo.txt`), background execution (`sleep 5 &`) with immediate prompt return and asynchronous reap notification, syntax error recovery, full automated test suites (`test_parser` 0 failed, `test_executor` 0 failed, `test_shell.sh` 70 passed, `test_termination.py` 2 passed), and clean exit. The recording predates the Part-3 expansion; the current suite is 182 checks (executor 45, termination 4).
- **Documentation:** `9-Demo/Final_Demo_Script.md`, `9-Demo/Demo_Evidence_Index.md`.
- **Finding:** **Passed**

---

## 11. Release Tag Verification

- **Release Tag:** `v1.0-final`
- **Release URL:** https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/releases/tag/v1.0-final
- **Tagged Commit:** `859b43ea8847b9abff28b9ea2c537b4c593deb5e`
- **Current `main` Head:** `10794b271f332316a2a60645597e015e15d48074`
- **Commit Graph & Diff Analysis:**
  Between commit `859b43ea` and commit `10794b27`, 111 files were changed with 23,479 insertions and 469 deletions. Changes are documentation, diagrams, PDFs, screenshots, demo evidence, and the Part-3 test/coverage expansion:
  - Zero changes in `src/` (0 diff lines)
  - Zero changes in `include/` (0 diff lines)
  - `tests/`: 3 files changed, 206 insertions, 9 deletions (Part-3 expansion)
  - `Makefile`: 10 insertions, 1 deletion (coverage target)
  - Zero changes in `.github/workflows/ci.yml` (0 diff lines)
- **Interpretation & Decision:**
  The tag `v1.0-final` correctly designates the frozen production baseline. Subsequent commits add documentation, demo evidence, and test/coverage work only. If coursework evaluation requires the tag to point to the exact final commit, both reviewers must agree before creating a post-submission tag.
- **Finding:** **Passed**

---

## 12. Documentation and Coursework Deliverable Review

All coursework deliverable folders and documentation files are present, up to date, and verified:

1. `1-SRS-and-Work-Breakdown/`: Contains IEEE 29148 formatted `SRS.md`, `SRS.pdf`, and use-case diagrams. **Passed**
2. `2-Test-Planning/`: Contains IEEE 829 / 29119-3 `Test_Plan.md`, `Test_Plan.pdf`, `Test_Cases.md`, `Validation_Specification.md`. **Passed**
3. `3-Requirements-Engineering/`: Contains `FR.md`, `NFR.md`, and `RTM.md` pointing to canonical sources. **Passed**
4. `4-Architectural-Design/`: Contains IEEE 42010/1016 `Architecture.md`, `SAD.pdf`, PlantUML source files, and rendered SVG/PNG sequence/component diagrams. **Passed**
5. `5-GitHub-Jira-Evidence/`: Contains real screenshot captures of repository, project boards, sprint views, PR approvals, CI runs, and release. **Passed**
6. `6-Copilot-Evidence/`: Contains scaffold and checklist noting zero unverified AI code generation was claimed. **Passed**
7. `7-Software-Testing/`: Contains `Testing_Tools.md`, `Test_Execution_Report.md`, and test README. **Passed**
8. `8-Deployment/`: Contains environment setup, build instructions, and execution guides. **Passed**
9. `9-Demo/`: Contains demo scripts, evidence index, screenshots, and links to MP4 recordings. **Passed**
10. `docs/`: Contains the Part-3 Unit Test Report, System Validation Report, Code Coverage Report, Branch Coverage Report, and retained evidence logs. **Passed**
- **Link Hygiene:** 58 deliverable markdown files scanned (excluding retained evidence/baseline copies); **0 broken relative links**.
- **Finding:** **Passed**

---

## 13. Known Limitations and Unresolved Findings

The following items are recorded transparently as known project limitations:

1. **System-Call Failure Path Injection:**
   - **Classification:** **Known limitation**
   - Low-level system call failures (`fork()`, `pipe()`, `waitpid()` returning `-1` due to resource exhaustion) are handled in the code with defensive diagnostics, but have not undergone automated fault-injection testing.
2. **Penetration Testing:**
   - **Classification:** **Known limitation**
   - Penetration testing is documented in `docs/Validation_Specification.md` Section 7 as a planned post-release activity, not an executed test suite. No penetration test execution is claimed.
3. **Local Platform Constraints:**
   - **Classification:** **Known limitation**
   - Windows/MSYS2 emulation cannot execute the Linux-specific `tests/test_termination.py` (`/proc` check), `make asan-test` (`-lasan` missing), or `make valgrind`. Full validation is achieved on Ubuntu Linux 26.04.1 LTS via GitHub Actions.
4. **Release-Blocking Defects:**
   - **Classification:** **None** (zero open release-blocking issues or PRs).
5. **Interactive interrupt (SIGINT):**
   - **Classification:** **Known limitation**
   - Ctrl+C is not handled and terminates the shell. `SIGCHLD` and `SIGTERM` are handled; clean termination is `exit`, Ctrl+D, or SIGTERM.
6. **Coverage:**
   - **Classification:** **Known limitation**
   - Production coverage is 75.99% of lines and 69.27% of branch edges. Uncovered paths are mainly syscall/allocation failure injection and forked children whose gcov counters do not flush.

---

## 14. Final Reviewer Sign-off

Joint sign-off on Issue #30 requires explicit approval from both assigned reviewers on the same candidate commit SHA:

### Accepted Final Candidate Commit
`10794b271f332316a2a60645597e015e15d48074`

This candidate contains the frozen production code plus the Part-3 test/coverage suite. PR #36 is documentation-only; if the final tag must include it, record the #36 merge commit after merge.

### Sign-off Checklist

| Reviewer | Role | Status | Date | Notes / Approval Reference |
|---|---|:---:|:---:|---|
| **Shreyas Kulkarni (`@KadakWNL`)** | Repository Owner / Parser & Pipelines Lead | **Pending** | — | Awaiting explicit personal sign-off on Issue #30 |
| **Shrikar Desai (`@ShrikarDesai07`)** | Reviewer | **Pending** | — | Awaiting explicit personal sign-off on Issue #30 |

> **Note on Issue #30 Closure:** Per strict project policy, neither reviewer may sign off on behalf of the other, nor may automated agents sign off on their behalf. Issue #30 remains **OPEN** until both `@KadakWNL` and `@ShrikarDesai07` personally post their approving sign-off comments referencing commit `10794b271f332316a2a60645597e015e15d48074`.
