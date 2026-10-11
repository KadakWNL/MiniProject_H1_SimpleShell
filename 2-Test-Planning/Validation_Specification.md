# Validation Specification — Simple Shell

## 1. Purpose

Validate that the documented requirements are correct, complete, consistent, feasible, measurable, and
testable before and during implementation, and define how functional, non-functional and security
validation will be carried out.

## 2. Validation Objectives

- Confirm the requirements reflect the intended shell functionality.
- Ensure each requirement is unambiguous and testable.
- Verify no requirement conflicts with another requirement.
- Confirm each FR has at least one acceptance criterion and test case.
- Confirm each NFR has a named verification procedure.
- Confirm every user-visible capability is represented in the use case model, and every use case
  traces back to a requirement.
- Confirm project scope is feasible for four students and the available Linux/C toolchain.

## 3. Actors and use cases

| Check | Method | Pass condition |
|---|---|---|
| Actors identified and named | Review `docs/Use_Case_Catalog.md` Section 1 | Number of actors is stated and each actor is named and classified |
| Use cases grouped by actor | Review `docs/Use_Case_Catalog.md` Section 2 | Every use case is assigned to an actor group |
| Use case diagram reflects the catalog | Compare `docs/Use_Case_Diagram.mmd` with the catalog | Diagram identifiers and relationships match the catalog exactly |
| Use cases trace to requirements | Review `docs/RTM.md` | Every use case is referenced by at least one requirement row |
| No orphan capabilities | Compare `help` output with the catalog | Every documented capability has a use case |

## 4. Requirements review

Each requirement is reviewed by at least two team members, and unresolved questions are recorded.

Additional checks:

- Every requirement carries a stable identifier and appears exactly once in the SRS.
- Every FR has an explicit, objectively checkable acceptance criterion.
- Every FR is mapped in the RTM to a use case, an implementation module, a Jira issue and a test.
- Documented limits in SRS Section 7.1 match the limits enforced in the parser.
- Any change to an agreed requirement is reflected in the SRS, the RTM and the test cases together.

## 5. Functional validation

Functional validation confirms that each FR produces its specified observable behaviour.

| ID | Validation activity | Pass condition |
|---|---|---|
| FVAL-01 | Run the automated shell suite (`make test`) | All asserted scenarios pass |
| FVAL-02 | Run the parser unit tests | All parser assertions pass |
| FVAL-03 | Confirm every FR in the RTM has at least one test case | No FR row has an empty test column |

Detailed scenarios are listed in `docs/Test_Cases.md` Section 1.

## 6. Non-functional validation

Non-functional validation confirms quality constraints rather than behaviour. Each NFR is verified by
the named procedure defined in `docs/Test_Cases.md` Section 2.

| NFR | Validation activity | Pass condition |
|---|---|---|
| NFR-01 | Repeated regression and stress runs | No crashes, no failing assertions |
| NFR-02 | Module and header separation review | Parsing, built-ins, execution and shell loop are separate modules |
| NFR-03 | `make asan-test` | ASan and UBSan report no errors or leaks |
| NFR-04 | Prompt and message review | Prompt and diagnostics are consistent and understandable |
| NFR-05 | Clean build on the target Linux system | Builds with the Makefile warning flags enabled |
| NFR-06 | CI runs on every push and pull request | CI job passes |
| NFR-07 | Source review plus malformed-input runs | No `system()` usage; malformed input is rejected safely |
| NFR-08 | Documentation review | Supported syntax, limits and testing steps match the implementation |

## 7. Security validation

Security validation focuses on the parser, because it is the component that processes untrusted
input.

| ID | Activity | Pass condition |
|---|---|---|
| SEC-01 | Malformed syntax: unterminated quotes, trailing backslash, leading/trailing pipe, non-final `&` | Each is rejected with an error and the shell stays usable |
| SEC-02 | Argument and segment exhaustion beyond the documented limits | Rejected cleanly with no overflow or leak |
| SEC-03 | Missing and unreadable files in redirections | Error reported, shell remains usable |
| SEC-04 | Non-existent command and non-executable targets | Error reported, shell remains usable |
| SEC-05 | Source review of external execution | `fork` + `execvp` used; no `system()`, no unintended second shell layer |
| SEC-06 | Sanitizer run of the malformed-input suite | No memory or undefined-behaviour findings |

Penetration-style testing is **planned only**. See Section 8.

## 8. Penetration testing plan

No penetration test is performed against the built shell at this stage. What follows defines the
authorisation, scope and method for a later execution phase.

### 8.1 Authorisation and scope

- **Authorisation:** local robustness testing of this project's own binary by the team members,
  on development machines only.
- **In scope:** the `parse_line` entry point, the interactive command loop, and the parser limits in
  SRS Section 7.1.
- **Out of scope:** any external or third-party system, any networked service (the shell has none),
  privilege escalation, persistence, and any activity that would degrade a shared or production
  system.

### 8.2 Threat model

The primary untrusted input is the command line supplied by the user or piped to standard input. The
parser is therefore the main attack surface; the executor is secondary, since it forwards validated
tokens to `fork`/`execvp`.

### 8.3 Planned techniques for the execution phase

| ID | Technique | Target | Expected safe outcome |
|---|---|---|---|
| PEN-01 | Fuzzing with random and structured byte sequences | `parse_line` | Rejection with an error, never a crash |
| PEN-02 | Argument count beyond `MAX_ARGS` | `parse_line` | Clean syntax error, no buffer overrun |
| PEN-03 | Pipeline length beyond `MAX_PIPE_SEGMENTS` | `parse_line` | Clean syntax error, no overflow |
| PEN-04 | Very long input lines and deep quote nesting | Lexer | No excessive allocation, no crash |
| PEN-05 | File-descriptor exhaustion via repeated redirections | `executor` | Descriptor cleanup, no leak |
| PEN-06 | Large numbers of background children | `executor` reap loop | No descriptor or process leak |
| PEN-07 | Control characters and high-bit bytes in arguments | Lexer | Treated as literal data |
| PEN-08 | Review for shell-metacharacter re-entry | Source | No `system()`; arguments passed as an argv array |

### 8.4 Planned tooling

- A fuzzing harness targeting `parse_line` (AFL++ or libFuzzer), added as a separate test target.
- AddressSanitizer and UndefinedBehaviorSanitizer builds via the existing `make asan-test`.
- Valgrind as a secondary memory check if sanitizers are unavailable.

### 8.5 Explicitly out of scope and prohibited

- Testing against any system not owned by the team.
- Any denial-of-service attempt against shared infrastructure.
- Social engineering, credential testing, or physical attacks.
- Publishing or sharing exploit details outside the course submission.

### 8.6 Scope of this plan

This document is the deliverable. Execution begins in a later phase and results are recorded as a
separate security test report.

## 9. Requirement quality checklist

| Check | Pass condition |
|---|---|
| Unique ID | Each requirement has one stable ID. |
| Categorized | Each requirement has one category and one validation focus. |
| Atomic | Requirement expresses one primary behavior. |
| Clear | No ambiguous terms. |
| Feasible | Implementable with agreed resources. |
| Measurable | A numeric target or threshold is given where applicable. |
| Testable | Acceptance can be objectively verified. |
| Traceable | Mapped to use case/design/test/Jira. |
| Consistent | No contradiction with another requirement. |

## 10. Traceability of this specification

| This document | Related document |
|---|---|
| Section 3 actor and use case checks | `docs/Use_Case_Catalog.md` |
| Section 5 functional validation | `docs/Test_Cases.md` Section 1 |
| Section 6 non-functional validation | `docs/Test_Cases.md` Section 2 |
| Section 7 security validation | `docs/Test_Cases.md` Section 1 |
| Section 8 penetration test plan | Not executed in this phase |
| Requirement quality checklist | `docs/SRS.md`, `docs/RTM.md` |

## 11. Approval record

- Prepared by: Team H1
- Reviewers: Team members
- Final reviewer: Project guide/instructor
- Status: Ready for team/instructor review
