# Project Status — Simple Shell

- **Project:** Simple Shell
- **Team:** H1
- **Primary Language:** C (ISO C11)
- **Target Environment:** Linux / POSIX.1-2008
- **Repository:** https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell

---

## 1. Implementation Status

The following core and advanced features are merged into `main` and verified through automated CI:

- **Interactive Command Loop:** Read-Eval-Print Loop (REPL) displaying `simple-shell$ ` prompt on `stderr`, processing user input line-by-line, and gracefully handling EOF (`Ctrl-D`) and `exit`.
- **Built-in Commands:** Parent-process execution of `cd` (including `cd -` OLDPWD and `cd` to `HOME`), `pwd`, `echo`, `help`, and `exit` with standard output buffer flushing.
- **External Command Execution:** Process creation using `fork()`, standard executable searching and execution via `execvp()`, and parent process synchronization via `waitpid()`.
- **Lexical Analysis & Quoting:** Whitespace tokenization, literal single quotes (`'...'`), double quotes (`"..."`) with embedded quote and backslash escape handling, and unquoted backslash escapes (`\ `).
- **I/O Redirection:** Input redirection `<` (`O_RDONLY`), output overwrite redirection `>` (`O_WRONLY | O_CREAT | O_TRUNC`), and output append redirection `>>` (`O_WRONLY | O_CREAT | O_APPEND`) for both built-in and external commands.
- **Multi-Stage Pipelines:** Inter-process communication pipelines (`|`) supporting up to 32 concurrent stages with intermediate descriptor cleanup, deadlock avoidance on large data streams, and child-process isolation for built-ins in pipelines.
- **Background Execution:** Asynchronous command execution (`&`) returning control immediately to the prompt.
- **Asynchronous Zombie Reaping:** `SIGCHLD` signal handler writing to an internal self-pipe, multiplexed with standard input via `poll()`, reaping background children and reporting termination diagnostics in real time without blocking.
- **Operator Pre-Validation:** Syntax validation rejecting duplicate background operators (`&&`, `& &`), misplaced `&`, duplicate redirections, and limits before process spawning or file creation.

---

## 2. Sprint Status

### Sprint 1 (19th Oct – 23rd Oct 2026) — Completed
- **Goal:** Working foreground shell with built-ins, external commands, parser, automated CI, and Sprint 1 demo video.
- **Velocity:** 32 Story Points (100% completed).
- **Status:** Done. All 9 user stories (#2, #3, #19, #4, #5, #6, #7, #8, #9) closed; demo video committed at `docs/demos/simple-shell-sprint1-demo.mp4`.

### Sprint 2 (26th Oct – 30th Oct 2026) — In Progress
- **Goal:** Advanced features (redirection, pipelines, background execution, self-pipe reaping), resource audit, full regression, documentation finalization, and 2-minute final demo video / freeze.
- **Velocity:** 34 Story Points planned.
  - **Completed & Merged (21 pts):**
    - Issue #10 (`[S2-01] I/O Redirection Support`, 5 pts): Closed via PR #25.
    - Issue #11 (`[S2-02] Advanced Operator Pre-Validation`, 3 pts): Closed via PR #27.
    - Issue #12 (`[S2-03] Two-Stage Pipeline Implementation`, 5 pts): Closed via PR #27.
    - Issue #14 (`[S2-05] Background Execution & Reaping`, 5 pts): Closed via PR #28.
    - Issue #17 (`[S2-08] Documentation Finalization, Architecture Sync & Deviation Log`, 3 pts): Closed via PR #29.
  - **Completed in this branch; pending PR merge (8 pts):** Issues #13 and #16.
  - **In Progress / Remaining (5 pts):**
    - Issue #15 (`[S2-06] Comprehensive Resource Audit`, 3 pts, @Aqual456): Sanitizers and descriptor checks active; final audit report pending.
    - Issue #18 (`[S2-09] 2-Minute Final Demo Video, Release Tag & Freeze`, 2 pts, @sudeeksh02): Pending final release video deliverable.

---

## 3. GitHub Issue & Pull Request Status

### Live Issue Status

| Issue | Title | Assignee | State | Resolution |
|---|---|---|:---:|---|
| [#2](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/2) | [S1-01] Modular Architecture & C11 Build Setup | @ShrikarDesai07 | Closed | Merged via PR #22 |
| [#3](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/3) | [S1-02] Interactive Command Loop & Prompt Display | @ShrikarDesai07 | Closed | Merged via PR #22 |
| [#19](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/19) | [S1-03] Command Line Tokenizer & Quote Parsing | @KadakWNL | Closed | Merged via PR #20 |
| [#4](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/4) | [S1-04] Error Diagnostics & Malformed Syntax | @KadakWNL | Closed | Merged via PR #20 |
| [#5](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/5) | [S1-05] External Command Execution & Foreground Wait | @Aqual456 | Closed | Merged via PR #26 |
| [#6](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/6) | [S1-06] Parser Memory Lifecycle & FD Hygiene | @Aqual456 | Closed | Merged via PR #26 |
| [#7](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/7) | [S1-07] Built-in Commands Implementation | @sudeeksh02 | Closed | Merged via PR #21 |
| [#8](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/8) | [S1-08] GitHub Actions CI Automation | @sudeeksh02 | Closed | Merged via PR #23 |
| [#9](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/9) | [S1-09] Sprint 1 Validation & Demo Video | @ShrikarDesai07 | Closed | Merged via PR #24 |
| [#10](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/10) | [S2-01] I/O Redirection Support (<, >, >>) | @ShrikarDesai07 | Closed | Merged via PR #25 |
| [#11](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/11) | [S2-02] Advanced Operator Pre-Validation | @KadakWNL | Closed | Merged via PR #27 |
| [#12](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/12) | [S2-03] Two-Stage Pipeline Implementation | @KadakWNL | Closed | Merged via PR #27 |
| [#13](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/13) | [S2-04] Multi-Stage Pipeline Expansion | @sudeeksh02 | In PR | Multi-stage pipeline and deadlock prevention suite verified |
| [#14](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/14) | [S2-05] Background Execution & Asynchronous Reaping | @Aqual456 | Closed | Merged via PR #28 |
| [#15](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/15) | [S2-06] Comprehensive Resource Audit | @Aqual456 | **Open** | Active in CI; final audit report pending |
| [#16](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/16) | [S2-07] Full Regression Testing & RTM Sync | @sudeeksh02 | In PR | Full regression verified (161 checks/build), RTM synced, and Test Report updated |
| [#17](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/17) | [S2-08] Documentation Finalization & Architecture Sync | @ShrikarDesai07 | Closed | Merged via PR #29 |
| [#18](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/18) | [S2-09] 2-Minute Final Demo Video & Freeze | @sudeeksh02 | **Open** | Pending final release |

### Pull Request Status
- **Merged Pull Requests:** PRs #20, #21, #22, #23, #24, #25, #26, #27, #28, and #29 are merged into `main`.
- **Release Tags:** Final release acceptance and development freeze are tracked in [issue #18](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/18); check the live issue and repository tags for their current state.

---

## 4. Documentation Deliverables Status

| Deliverable | Location | Status | Notes |
|---|---|:---:|---|
| **Root README** | `README.md` | **Finalized** | Comprehensive evaluator guide with setup, features, examples, limits, test guide, and structure. |
| **System Architecture** | `docs/Architecture.md` | **Finalized** | Detailed execution flow, module interfaces, self-pipe asynchronous reaping, and Phase 1 deviation log. |
| **Requirements Specification** | `docs/SRS.md` | **Complete** | All 10 FRs and 8 NFRs defined with categorized acceptance criteria. |
| **Traceability Matrix** | `docs/RTM.md` | **Updated** | Bi-directional mappings from requirements to modules, tests, and Jira keys. |
| **Test Cases & Procedures** | `docs/Test_Cases.md` | **Complete** | Functional test cases (TC-01..18) and non-functional procedures (NFRV-01..08). |
| **Test Execution Report** | `docs/Test_Report.md` | **Complete** | Recorded runs across unit, executor, and shell integration test suites. |
| **Project Status** | `docs/Project_Status.md` | **Finalized** | Synchronized with live GitHub repository issues, PRs, and sprint velocity. |
| **Sprint Backlog** | `docs/Sprint_Backlog.md` | **Synchronized** | Reflects Sprint 1 completion and current Sprint 2 progress. |
| **Architectural Diagrams** | `4-Architectural-Design/` | **Synchronized** | `Architecture.puml` updated to reflect self-pipe and multi-stage execution model. |

---

## 5. Remaining Work Prior to Project Freeze

1. **Complete Resource Audit (Issue #15):** Finalize Valgrind and descriptor leak analysis records.
2. **Final 2-Minute Demo Video & Tag (Issue #18):** Record and link the final 2-minute demonstration video, cut the release tag on `main`, complete final acceptance, and freeze development.
