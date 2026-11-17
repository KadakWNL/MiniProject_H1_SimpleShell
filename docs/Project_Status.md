# Project Status — Simple Shell

- **Project:** Simple Shell
- **Team:** H1
- **Primary Language:** C (ISO C11)
- **Target Environment:** Linux / POSIX.1-2008
- **Repository:** https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell
- **Status date:** 12 November 2026
- **Part-3 testing integration:** 17 November 2026 (test-only; production code unchanged)

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

Development is feature-complete and tagged `v1.0-final` (12 November 2026). The final demonstration
evidence remains tracked in issue #18; the joint review and sign-off is tracked in issue #30.

---

## 2. Sprint Status

### Sprint 1 (19th Oct – 23rd Oct 2026) — Completed
- **Goal:** Working foreground shell with built-ins, external commands, parser, automated CI, and Sprint 1 demo video.
- **Velocity:** 32 Story Points (100% completed).
- **Status:** Done. All 9 user stories (#2, #3, #19, #4, #5, #6, #7, #8, #9) closed; demo video committed at `docs/demos/simple-shell-sprint1-demo.mp4` (58.1 s).

### Sprint 2 (26th Oct – 30th Oct 2026) — Completed
- **Goal:** Advanced features (redirection, pipelines, background execution, self-pipe reaping), resource audit, full regression, documentation finalization, and the final demo/release package.
- **Velocity:** 34 Story Points planned; 32 points of development completed.
  - Issue #10 (`[S2-01]` I/O Redirection, 5 pts): closed via PR #25.
  - Issue #11 (`[S2-02]` Operator Pre-Validation, 3 pts): closed via PR #27.
  - Issue #12 (`[S2-03]` Two-Stage Pipeline, 5 pts): closed via PR #27.
  - Issue #13 (`[S2-04]` Multi-Stage Pipeline Expansion, 3 pts): closed via PR #31.
  - Issue #14 (`[S2-05]` Background Execution & Reaping, 5 pts): closed via PR #28.
  - Issue #15 (`[S2-06]` Resource Audit, 3 pts): closed via PR #32.
  - Issue #16 (`[S2-07]` Full Regression & RTM Sync, 5 pts): closed via PR #31.
  - Issue #17 (`[S2-08]` Documentation Finalization, 3 pts): closed via PR #29.
  - Issue #18 (`[S2-09]` Final Demo Video & Release, 2 pts): carried as a final check; video and release acceptance remain open.

### Sprint 3 (2nd Nov – 6th Nov 2026) — Completed and closed
- **Spillover:** #13, #15, and #16 were accepted and closed between 2 and 3 November 2026, ahead of the 6 November due date.
- **Milestone:** closed on 9 November 2026 with 3 closed / 0 open issues.
- **Final checks:** the remaining release and review work is tracked outside the sprint as #18 and #30.

---

## 3. GitHub Issue & Pull Request Status

### Live Issue Status

| Issue | Title | Assignee | State | Resolution |
|---|---|---|---|:---:|---|
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
| [#13](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/13) | [S2-04] Multi-Stage Pipeline Expansion | @sudeeksh02 | Closed | Merged via PR #31 |
| [#14](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/14) | [S2-05] Background Execution & Asynchronous Reaping | @Aqual456 | Closed | Merged via PR #28 |
| [#15](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/15) | [S2-06] Comprehensive Resource Audit | @Aqual456 | Closed | Merged via PR #32 |
| [#16](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/16) | [S2-07] Full Regression Testing & RTM Sync | @sudeeksh02 | Closed | Merged via PR #31 |
| [#17](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/17) | [S2-08] Documentation Finalization & Architecture Sync | @ShrikarDesai07 | Closed | Merged via PR #29 |
| [#18](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/18) | [FinalChecks] 2-Minute Final Demo Video, Release Tag & Development Freeze | @sudeeksh02 | **Open** | Final demo and release acceptance |
| [#30](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/30) | [FinalChecks] Joint final project review and sign-off | @KadakWNL, @ShrikarDesai07 | **Open** | Joint final review |

### Pull Request Status
- **Merged Pull Requests:** PRs #20, #21, #22, #23, #24, #25, #26, #27, #28, #29, #31, and #32 are merged into `main`.
- **Release Tags:** `v1.0-final` was tagged on `main` at commit `859b43e` on 12 November 2026 and
  published as a GitHub release. Final demonstration acceptance remains tracked in
  [issue #18](https://github.com/SE-H1-SimpleShell/MiniProject_H1_SimpleShell/issues/18).
- **Documentation restructure:** the IEEE-format SRS, Test Plan, and SAD restructure was merged to
  `main` via PR #33 on 12 November 2026.
- **Part-3 testing package:** the expanded executor and termination harnesses, the four Part-3
  reports, the coverage target, and the retained evidence are prepared on branch
  `part-3/integrate-testing-package` for review before submission.

---

## 4. Documentation Deliverables Status

| Deliverable | Location | Status | Notes |
|---|---|:---:|---|
| **Root README** | `README.md` | **Finalized** | Evaluator guide with setup, features, examples, limits, test guide, and structure. |
| **Requirements Specification** | `docs/SRS.md`, `1-SRS-and-Work-Breakdown/SRS.md` | **Finalized (IEEE 29148)** | 10 FRs, 8 NFRs, 3 security objectives, 4 security requirements, use case diagram. |
| **Software Architecture & Design** | `docs/Architecture.md`, `4-Architectural-Design/Architecture.md` | **Finalized (IEEE 42010/1016)** | Stakeholders, views, security architecture, two sequence diagrams, error-handling protocols, deviation log. |
| **Test Plan** | `2-Test-Planning/Test_Plan.md` | **Finalized (IEEE 829/29119-3)** | Sections 3–5 complete; security validation in Section 5.1. |
| **Traceability Matrix** | `docs/RTM.md` | **Updated** | FR/NFR to use case, module, test, and tracking issue; SR/SO mapping note. |
| **Test Cases & Procedures** | `docs/Test_Cases.md` | **Complete** | Functional test cases (TC-01..18) and non-functional procedures (NFRV-01..08). |
| **Test Execution Report** | `docs/Test_Report.md` | **Complete** | Recorded runs across parser, executor, shell integration, and termination cleanup suites (164 checks per build + Valgrind) with historical baseline retained; Part-3 expanded suite recorded separately (182 checks). |
| **Unit Test Report** | `docs/Unit_Test_Report.md` | **Complete (Part-3)** | Parser and executor/component results across standard, sanitizer, and coverage builds. |
| **System Validation Report** | `docs/System_Validation_Report.md` | **Complete (Part-3)** | End-to-end shell and SIGTERM lifecycle validation with disposition and limitations. |
| **Code Coverage Report** | `docs/Code_Coverage_Report.md` | **Complete (Part-3)** | 75.99% production line coverage with gcov evidence. |
| **Branch Coverage Report** | `docs/Branch_Coverage_Report.md` | **Complete (Part-3)** | 69.27% outgoing branch-edge coverage with gcov evidence. |
| **Validation Specification** | `docs/Validation_Specification.md` | **Complete** | Security validation SEC-01..06 and penetration-test plan. |
| **Project Status** | `docs/Project_Status.md` | **Finalized** | Synchronized with live issues, PRs, and sprint status. |
| **Sprint Backlog** | `docs/Sprint_Backlog.md` | **Synchronized** | Reflects completed Sprint 1/2/3 work and remaining final checks. |
| **Architectural Diagrams** | `4-Architectural-Design/` | **Synchronized** | Component, use case, and two sequence diagrams with PlantUML sources. |
| **GitHub & Demo Evidence** | `5-GitHub-Jira-Evidence/screenshots/`, `9-Demo/screenshots/` | **Captured** | Repository, project table/board, sprint views, PRs, CI, release, and Sprint 1 output frames. |

---

## 5. Remaining Work Prior to Final Submission

1. **Final 2-Minute Demo Video (Issue #18):** record and link the final demonstration; complete release acceptance.
2. **Joint Final Review (Issue #30):** both reviewers sign off on the same final commit after #18 is closed.
3. **Review the Part-3 testing branch:** approve and merge `part-3/integrate-testing-package` so the four reports and the 182-check suite land on `main`.
4. **Refresh the demo evidence index:** attach the final recording and remaining screenshots.
