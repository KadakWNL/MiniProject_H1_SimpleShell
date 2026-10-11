# Project Status

**Project:** Simple Shell
**Team:** H1
**Primary language:** C
**Target:** Linux
**Last updated:** 2026-09-29

## Phase 1 (Requirements stage) status

| Deliverable | File | Status |
|---|---|---|
| Problem statement / feasibility | `Problem_Statement.md`, `Feasibility_Study.md` | Prepared |
| SRS and requirement categorization | `SRS.md` | Prepared |
| RTM | `RTM.md` | Prepared |
| Actors identified and named | `Use_Case_Catalog.md` §1 | Prepared — 2 actors |
| Use cases grouped by actor | `Use_Case_Catalog.md` §2 | Prepared |
| Use case diagram | `Use_Case_Diagram.mmd` / `.png` | Prepared |
| Validation specification | `Validation_Specification.md` | Prepared, including penetration test plan (planning only) |

Deliverable-to-file mapping is in `docs/Phase_1_Checklist.md` Section 1.

## Implementation baseline

- Modular C implementation in `src/` with headers in `include/`.
- Automated parser unit tests and shell integration tests in `tests/`.
- GitHub Actions CI workflow running `make test` on every push and pull request.
- Jira backlog seed in `docs/Jira_Backlog.md`.

## Validation state

- No recorded validation run. See `docs/Test_Report.md` Section 1.
- Automated coverage gaps are recorded in `docs/RTM.md` Section 4 and `docs/Test_Cases.md` Section 4.
  TC-13 (background) and TC-16 (foreground synchronisation) are the only coverage for FR-07 and
  FR-09 and are not yet automated.

## Phase 2 (Agile Development stage) status

| Deliverable | Description / Location | Status | ETA |
|---|---|---|---|
| Backlog creation in GitHub | Issues #2 to #19 mapped to FR/NFRs | Published | 12th Oct 2026 |
| Story Points assignment | Fibonacci points assigned across all stories (66 pts total) | Completed | 12th Oct 2026 |
| Team story assignments | Distributed equally across 4 team members | Completed | 12th Oct 2026 |
| Two Sprint plans | Sprint 1 (Oct 19–23) and Sprint 2 (Oct 26–30) documented in `docs/Sprint_Backlog.md` | Completed | 12th Oct 2026 |
| Sprint dry run | Practice PRs and CI validation | Scheduled | 12th–16th Oct 2026 |
| Sprint 1 execution | Working foreground shell & 1-min demo video | Scheduled | 19th–23rd Oct 2026 |
| Sprint 2 execution | Advanced features, 2-min demo video & freeze | Scheduled | 26th–30th Oct 2026 |

## Open items

1. Run `make test` and `make asan-test` on the target Linux system and paste the output into
   `Test_Report.md` Sections 5.1–5.3.
2. Link the published GitHub Issues into the GitHub Project board (Projects v2) under the repo.
3. Conduct Sprint dry run during Oct 12–16 before starting Sprint 1 development on Oct 19.
