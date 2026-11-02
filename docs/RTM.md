# Requirements Traceability Matrix (RTM)

Maps every requirement to the use case that realises it, the source module that implements it, the
Jira issue that tracks it, and the test that verifies it.

Use case identifiers are defined in `docs/Use_Case_Catalog.md`. Verification identifiers for the
non-functional requirements are defined in `docs/Test_Cases.md`.

## 1. Functional requirements

| Req ID | Requirement | Use Case | Implementation | Test Cases | Coverage | Jira | Status |
|---|---|---|---|---|---|---|---|
| FR-01 | Interactive input | UC-01 | `src/shell.c` | TC-01, TC-10 | Automated | SH-REQ-01 | Implemented; accepted in Sprint 1 |
| FR-02 | External command execution | UC-03, UC-04 | `src/executor.c` | TC-02 | Automated | SH-REQ-02 | Implemented; accepted in Sprint 1 |
| FR-03 | Built-in commands | UC-05, UC-10 | `src/builtins.c`, `src/executor.c` | TC-03, TC-04, TC-18 | Automated | SH-REQ-03 | Implemented; accepted in Sprint 1 |
| FR-04 | Command parsing | UC-02 | `src/parser.c` | TC-05, TC-06, TC-11 | Automated | SH-REQ-04 | Implemented; operator pre-validation accepted (#11) |
| FR-05 | Input/output redirection | UC-06 | `src/parser.c`, `src/executor.c` | TC-07, TC-08, TC-09 | Automated | SH-REQ-05 | Implemented; accepted (#10) |
| FR-06 | Pipelines | UC-07 | `src/parser.c`, `src/executor.c` | TC-12 | Automated | SH-REQ-06 | Implemented; two-stage (#12) is merged; multi-stage (#13) expansion and deadlock prevention verified in this branch, PR pending |
| FR-07 | Background execution | UC-08 | `src/shell.c`, `src/parser.c`, `src/executor.c` | TC-13 | Automated | SH-REQ-07 | Implemented; background execution and asynchronous self-pipe reaping accepted (#14) |
| FR-08 | Error handling | UC-02, UC-09 | `src/shell.c`, `src/parser.c`, `src/executor.c`, `src/builtins.c` | TC-11, TC-14, TC-15 | Automated | SH-REQ-08 | Implemented; syntax verification accepted (#11) |
| FR-09 | Process synchronization | UC-03, UC-04 | `src/executor.c` | TC-16 | Automated | SH-REQ-09 | Implemented; foreground synchronization accepted (#12) |
| FR-10 | Clean termination | UC-10 | `src/shell.c`, `src/builtins.c` | TC-01, TC-10 | Automated | SH-REQ-10 | Implemented; accepted in Sprint 1 |

## 2. Non-functional requirements

Non-functional requirements are verified by the procedures NFRV-01 … NFRV-08 rather than by
functional test cases.

| Req ID | Requirement | Use Case | Verification | Method | Jira | Status |
|---|---|---|---|---|---|---|
| NFR-01 | Reliability | All | NFRV-01, TC-17 | Repeated stress and regression runs; ASan/UBSan build | SH-NFR-01 | Implemented & verified in this branch; regression PR (#16) pending merge |
| NFR-02 | Maintainability | N/A | NFRV-02 | Module/header separation review | SH-NFR-02 | Modular implementation accepted in Sprint 1 |
| NFR-03 | Memory safety | N/A | NFRV-03 | `make asan-test` with AddressSanitizer and UndefinedBehaviorSanitizer | SH-NFR-03 | Sanitizer checks pass; resource audit pending (#15) |
| NFR-04 | Usability | All | NFRV-04 | Prompt and diagnostic message review | SH-NFR-04 | Prompt and diagnostic tests pass; terminal review remains |
| NFR-05 | Portability | N/A | NFRV-05 | Build with GCC, C11, POSIX APIs on the target Linux system | SH-NFR-05 | Linux GCC C11 build and CI pass |
| NFR-06 | Testability | N/A | NFRV-06 | CI runs `make test` on every push and pull request | SH-NFR-06 | Automated CI active; executor and shell coverage active |
| NFR-07 | Security hygiene | N/A | NFRV-07 | Malformed-input suite; source check that `system()` is not used | SH-NFR-07 | Validation implemented and accepted (#11) |
| NFR-08 | Documentation | N/A | NFRV-08 | Documentation review against the supported syntax and limits | SH-NFR-08 | Documentation finalization and architecture sync in progress (#17) |

## 3. Traceability rules

1. Every requirement has a unique, stable identifier.
2. Every FR maps to a use case, an implementation module, a Jira issue, and at least one test case.
3. Every NFR maps to a named verification procedure rather than a functional test case.
4. Jira issue status and this matrix are updated after each accepted change.
5. A requirement is not considered **verified** until implementation, review, and recorded
   validation evidence all exist. `Status` in this table records **implementation** only; the
   recorded run evidence lives in `docs/Test_Report.md`.

## 4. Automated coverage and remaining gaps

[Parser tests](../tests/test_parser.c) check malformed operators, quoted literals,
rejected-input cleanup, and the 32/33-stage limit (63 checks). [Executor tests](../tests/test_executor.c)
check multi-stage pipeline status, both-stage and intermediate-stage completion,
parent-state isolation, deadlock prevention on high data volumes, child reaping,
and repeated 2-stage and 4-stage pipelines under a 64-descriptor limit (29 checks).
The [shell suite](../tests/test_shell.sh) checks syntax recovery without execution/file
side effects, multi-stage pipelines (3, 4, 5, and 32 stages), large data stream deadlock
prevention, regression stress loops, and asynchronous background process reaping (69 checks).
Results and verification evidence are recorded in [Test_Report.md](Test_Report.md).

The coverage encompasses Sprint 2 feature PRs and the multi-stage pipeline (#13) and full regression (#16) verification.

These are known and deliberately tracked prior to final freeze.

| Gap | Affected | Note |
|---|---|---|
| System-call failure paths are not injected by tests | FR-08, FR-09 | `fork`, `pipe`, and `waitpid` failures still require targeted fault-injection coverage. |
| Final release freeze and demo | NFR-08, Deliverables | Complete final 2-minute demo video and development freeze under #18. |
