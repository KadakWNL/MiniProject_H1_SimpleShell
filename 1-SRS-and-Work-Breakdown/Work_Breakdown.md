# Work Breakdown Structure

| WBS | Work package | Activities | Related issues |
|---|---|---|---|
| 1.0 | Requirements and planning | Problem analysis, feasibility, SRS, FR/NFR, RTM, use cases and validation specification | Existing Phase 1 artifacts |
| 2.0 | Architecture and design | Module boundaries, component/interface model, sequence/use-case diagrams and justification | #2, #17 |
| 3.0 | Core shell foundation | C11 build structure, prompt/input loop, clean exit and dispatch | #2, #3 |
| 4.0 | Parsing and execution | Tokenizer, quotes/escapes, syntax errors, external execution and foreground waiting | #4, #5, #19 |
| 5.0 | Built-ins and shell operators | Built-ins, redirection, pipelines and background execution | #7, #10–#14 |
| 6.0 | QA and automation | CI, parser/integration tests, sanitizer checks, resource audit and regression/RTM updates | #6, #8, #15, #16 |
| 7.0 | Release and demo | Fresh-clone validation, documentation sync, demos and development freeze | #9, #17, #18 |

## Sprint allocation
- **Sprint 1 (19–23 October 2026):** core shell, tokenizer, built-ins, external execution, errors, cleanup, CI and one-minute demo.
- **Sprint 2 (26–30 October 2026):** redirection, syntax validation, pipelines, background processing, resource audit, regression evidence, final documentation and two-minute demo.

This is a planning decomposition, not evidence that all planned work is complete. Existing GitHub issues remain authoritative for acceptance criteria, assignees and estimates.
