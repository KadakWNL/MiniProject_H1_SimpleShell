# Phase 1 Completion Checklist

The Requirements stage package, with each deliverable mapped to the file that satisfies it.

## 1. Deliverable index



| # | Deliverable | Satisfied by | Status |
|---|---|---|---|

| 1 | Problem statement Analysis / Feasibility Testing | `docs/Problem_Statement.md`, `docs/Feasibility_Study.md` | Complete |

| 2 | SRS List | `docs/SRS.md` Section 5 (FR) and Section 6 (NFR) | Complete |
| 3 | Categorize requirements into FR's and VFR's | `docs/SRS.md` Section 4 — FR and NFR categories with a validation focus per requirement | Complete |
| 4 | Prepare RTM table for the requirements | `docs/RTM.md` | Complete |
| 5 | Identify the number of actors and name the actors | `docs/Use_Case_Catalog.md` Section 1 — **2 actors** | Complete |

| 6 | Group the use cases based on actors | `docs/Use_Case_Catalog.md` Section 2 | Complete |
| 7 | Draw use case diagram on actors | `docs/Use_Case_Diagram.mmd`, rendered to `docs/Use_Case_Diagram.png` | Complete |
| — | Validation Specification | `docs/Validation_Specification.md` | Complete |




## 2. Supporting documents

| Document | Role |
|---|---|

| `docs/Architecture.md` | Module responsibilities and design decisions |
| `docs/Test_Cases.md` | Functional test cases and non-functional verification procedures |
| `docs/Test_Report.md` | Recorded validation evidence |
| `docs/Jira_Backlog.md` | Planned issue breakdown |
| `docs/Project_Status.md` | Current state and the next controlled step |



## 3. Actor identification summary



All ten use cases are initiated by A1. A2 supplies the POSIX services those use cases depend on and
is associated with them through labelled service relationships.





## 4. Student learning evidence

| Learning outcome | Evidence |
|---|---|
| Customer/user perspective | `Problem_Statement.md` Section 5 names the primary user; `Use_Case_Catalog.md` derives every use case from that user's goals |
| Use case development | Ten use cases with triggers, actor grouping and relationship semantics, in `Use_Case_Catalog.md` and the use case diagram |
| Building measurable and testable requirements | Every FR in `SRS.md` has an explicit acceptance criterion; `SRS.md` Section 7.1 states numeric parser limits |
| Testability of requirements | Every FR maps to a test case and every NFR to a verification procedure in `RTM.md` |
| Functional validation | `Validation_Specification.md` Section 5, backed by `Test_Cases.md` Section 1 |
| Non-functional validation | `Validation_Specification.md` Section 6, backed by `Test_Cases.md` Section 2 |
| Security validation | `Validation_Specification.md` Section 7, covering malformed input, resource limits and avoidance of `system()` |
| Penetration testing | **Planning only.** `Validation_Specification.md` Section 8 defines scope, threat model, eight planned techniques, tooling, and explicit prohibitions. No penetration test is executed in this phase. |




## 5. Open items




1. **Attach validation evidence.** `Test_Report.md` currently describes how to produce the evidence
   rather than recording an actual run.
2. **Close the automated coverage gaps** listed in `RTM.md` Section 4 and `Test_Cases.md` Section 4.







#Formatted checklist.md