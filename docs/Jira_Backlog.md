# Backlog — Simple Shell

> **Note (Part-2 Deliverable):** The active Agile sprint backlog for Mini-Project Part-2 is tracked live on [GitHub Issues](https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell/issues) and detailed in [docs/Sprint_Backlog.md](Sprint_Backlog.md) with story point estimates and sprint assignments across the 4 team members.

Use the project key `SH` if syncing with a Jira project. The issue IDs below are logical IDs; Jira will generate its own numeric keys when the issues are created.

## Epic: Requirements & Planning

| Summary | Type | Priority | Acceptance |
|---|---|---|---|
| Problem statement and feasibility | Task | High | `docs/Problem_Statement.md` and `docs/Feasibility_Study.md` reviewed. |
| Prepare SRS | Task | High | FR/NFR list complete and testable. |
| Prepare RTM | Task | High | All FRs trace to use cases, code, tests. |
| Identify actors and use cases | Task | High | Actor list and use-case diagram reviewed. |
| Validation specification | Task | High | Requirements validation criteria complete. |

## Epic: Core Shell

| Summary | Type | Priority |
|---|---|---|
| Interactive command loop | Story | High |
| Implement `cd`, `pwd`, `echo`, `help`, `exit` | Story | High |
| Add command parser and quote handling | Story | High |
| Add external command execution | Story | High |

## Epic: Advanced Shell Features

| Summary | Type | Priority |
|---|---|---|
| Input/output redirection | Story | High |
| Pipelines | Story | High |
| Background execution | Story | Medium |
| Error handling and cleanup | Story | High |

## Epic: Testing and Quality

| Summary | Type | Priority |
|---|---|---|
| Automated shell test suite | Task | High |
| Sanitizer/memory safety validation | Task | High |
| Regression testing | Task | High |
| Final RTM status update | Task | Medium |

## Epic: Documentation & Release

| Summary | Type | Priority |
|---|---|---|
| README and installation guide | Task | Medium |
| Architecture documentation | Task | Medium |
| Final test report | Task | High |
| GitHub release/tag | Task | Medium |
| Demo preparation | Task | High |

## Suggested workflow

`TO DO -> IN PROGRESS -> CODE REVIEW -> DONE`

## Definition of Done

- Acceptance criteria satisfied.
- Code compiles with required warnings enabled.
- Relevant automated/manual tests pass.
- Pull request reviewed and merged.
- RTM/test evidence updated.
- Jira issue linked to the commit/PR.
