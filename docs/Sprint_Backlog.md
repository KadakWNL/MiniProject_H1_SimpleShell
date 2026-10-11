# Sprint Backlog & Work Distribution — Simple Shell

**Repository:** [ShrikarDesai07/MiniProject_H1_SimpleShell](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell)  
**Deliverable:** Software Engineering Mini-Project Part-2 (Backlog Creation)  
**ETA:** 12th October 2026  
**Status:** Published to GitHub Issues & Board  

---

## 1. Team Members & Roles

| Member | GitHub Username | Role | Sprint 1 Points | Sprint 2 Points | Total Points |
|---|---|---|:---:|:---:|:---:|
| **Shrikar Desai** | [@ShrikarDesai07](https://github.com/ShrikarDesai07) | Repo Owner / Core Shell / Redirection | 8 | 8 | **16** |
| **Shreyas Kulkarni** | [@KadakWNL](https://github.com/KadakWNL) | Parser / Pipelines Lead | 8 | 8 | **16** |
| **Sujal Yadavi** | [@Aqual456](https://github.com/Aqual456) | Process Execution / Background Jobs | 8 | 8 | **16** |
| **Sudeeksh Nayak** | [@sudeeksh02](https://github.com/sudeeksh02) | Built-ins / CI / QA & Validation | 8 | 10 | **18** |
| **Total** | | | **32** | **34** | **66** |

---

## 2. Sprint 1 Backlog (19th Oct – 23rd Oct 2026)
**Sprint Goal:** Deliver a working foreground shell (REPL loop, parser with quotes/escapes, external command execution, all 5 built-in commands, error recovery, and clean exit) with automated CI and a 1-minute video demo.

| Issue | Title | Epic | Requirements | Points | Assignee | Status |
|---|---|---|---|:---:|---|:---:|
| [#2](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/2) | [S1-01] Modular Architecture, Source Structure & C11 Build Setup | Core Shell | NFR-02, NFR-05 | 3 | @ShrikarDesai07 | Ready |
| [#3](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/3) | [S1-02] Interactive Command Loop, Prompt Display & Clean Termination | Core Shell | FR-01, FR-10, NFR-04 | 3 | @ShrikarDesai07 | Ready |
| [#19](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/19) | [S1-03] Command Line Tokenizer, Quote Parsing & Escape Handling | Core Shell | FR-04, NFR-07 | 5 | @KadakWNL | Ready |
| [#4](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/4) | [S1-04] Error Diagnostics, Malformed Syntax Handling & Safe Recovery | Core Shell | FR-08, NFR-01, NFR-04 | 3 | @KadakWNL | Ready |
| [#5](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/5) | [S1-05] External Command Execution via Fork/Execvp & Foreground Waiting | Core Shell | FR-02, FR-09, NFR-07 | 5 | @Aqual456 | Ready |
| [#6](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/6) | [S1-06] Parser Memory Lifecycle, File Descriptor Hygiene & Teardown | Core Shell | FR-10, NFR-03 | 3 | @Aqual456 | Ready |
| [#7](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/7) | [S1-07] Built-in Commands Implementation: cd, pwd, echo, help, exit | Core Shell | FR-03 | 5 | @sudeeksh02 | Ready |
| [#8](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/8) | [S1-08] GitHub Actions CI Automation for PRs, Commits & Sanitizer Checks | Testing & Quality | NFR-05, NFR-06 | 3 | @sudeeksh02 | Ready |
| [#9](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/9) | [S1-09] Sprint 1 Validation, Fresh Clone Verification & 1-Minute Demo Video | Documentation & Release | NFR-08, Deliverable | 2 | @ShrikarDesai07 | Ready |

**Sprint 1 Velocity:** 32 Story Points

---

## 3. Sprint 2 Backlog (26th Oct – 30th Oct 2026)
**Sprint Goal:** Complete advanced features (I/O redirection `<`, `>`, `>>`, pipelines `|`, background execution `&`), full regression suite, 2-minute video demo, documentation sync, and development freeze.

| Issue | Title | Epic | Requirements | Points | Assignee | Status |
|---|---|---|---|:---:|---|:---:|
| [#10](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/10) | [S2-01] I/O Redirection Support (<, >, >>) for Built-in & External Commands | Advanced Features | FR-05, FR-08, NFR-03 | 5 | @ShrikarDesai07 | Backlog |
| [#11](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/11) | [S2-02] Advanced Operator Pre-Validation & Strict Syntax Verification | Advanced Features | FR-04, FR-08, NFR-07 | 3 | @KadakWNL | Backlog |
| [#12](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/12) | [S2-03] Two-Stage Pipeline Implementation Connecting Commands and Built-ins | Advanced Features | FR-06, FR-09, NFR-03 | 5 | @KadakWNL | Backlog |
| [#13](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/13) | [S2-04] Multi-Stage Pipeline Expansion & Deadlock Prevention | Advanced Features | FR-05, FR-06, FR-09 | 3 | @sudeeksh02 | Backlog |
| [#14](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/14) | [S2-05] Background Command Execution (&) & Asynchronous Zombie Reaping | Advanced Features | FR-07, FR-09 | 5 | @Aqual456 | Backlog |
| [#15](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/15) | [S2-06] Comprehensive Resource Audit: Sanitizers, Descriptors & Stress Safety | Testing & Quality | FR-10, NFR-01, NFR-03 | 3 | @Aqual456 | Backlog |
| [#16](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/16) | [S2-07] Full Regression Testing, RTM Traceability Sync & Test Report | Testing & Quality | FR-01–10, NFR-01, NFR-04–07 | 5 | @sudeeksh02 | Backlog |
| [#17](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/17) | [S2-08] Documentation Finalization, Architecture Sync & Deviation Log | Documentation & Release | NFR-02, NFR-08 | 3 | @ShrikarDesai07 | Backlog |
| [#18](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues/18) | [S2-09] 2-Minute Final Demo Video, Release Tag & Development Freeze | Documentation & Release | Deliverable 3.b, 3.d | 2 | @sudeeksh02 | Backlog |

**Sprint 2 Velocity:** 34 Story Points

---

## 4. Definition of Done (DoD)
A story is considered **Done** only when:
1. Acceptance criteria defined in the story are completely satisfied.
2. Code adheres to C11 standard and compiles warning-free with `gcc -Wall -Wextra -std=c11`.
3. Automated test suite passes (`make test` and `make asan-test`).
4. PR is created by the assigned team member from their branch (`feat/...`).
5. PR is reviewed by peers and approved/merged by repo owners.
6. Associated issue is closed with commit/PR link.
