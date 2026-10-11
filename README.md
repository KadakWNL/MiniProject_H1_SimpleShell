# Simple Shell

A lightweight Unix-like command-line interpreter written in C for Linux.

## Features

- Interactive prompt
- Built-ins: `cd`, `pwd`, `echo`, `help`, `exit`
- External command execution using `fork`/`execvp`
- Single-quoted, double-quoted and escaped arguments
- Input redirection: `<`
- Output redirection: `>`
- Append redirection: `>>`
- Pipelines: `|`
- Background execution: `&`
- Error handling for common parser/process/file failures

Redirection, pipelines and background execution apply to built-ins as well as external programs.

## Build

```bash
make
```

## Run

```bash
./simple-shell
```

Example:

```text
simple-shell$ pwd
/home/student/simple-shell
simple-shell$ echo "hello world"
hello world
simple-shell$ echo 'single quoted'
single quoted
simple-shell$ printf "a\nb\n" | wc -l
2
simple-shell$ exit
```

## Supported syntax

| Element | Syntax |
|---|---|
| Arguments | Whitespace separated |
| Single quotes | `'text'` — no escape processing inside |
| Double quotes | `"text"` — backslash escapes `"`, `\`, `$`, backtick and newline |
| Escapes | `\c` outside quotes yields a literal `c` |
| Input redirection | `cmd < file` |
| Output redirection | `cmd > file` |
| Append redirection | `cmd >> file` |
| Pipeline | `cmd \| cmd` |
| Background | `cmd &` |

## Limits and rejected syntax

These are enforced by the parser and reported as syntax errors.

| Limit | Value |
|---|---|
| Maximum arguments per command | 128 |
| Maximum pipeline segments | 32 |
| Multiple input redirections on one command | Rejected |
| Multiple output redirections on one command | Rejected |
| Multiple `&` operators | Rejected |
| `&` in a non-final position | Rejected |
| Unterminated quote | Rejected |
| Trailing backslash | Rejected |

## Test

```bash
make test
```

For sanitizer validation:

```bash
make asan-test
```

## Project documentation

Requirements stage (Phase 1):

- `docs/Problem_Statement.md` — problem analysis, stakeholders, risks
- `docs/Feasibility_Study.md` — technical, operational, economic, schedule, resource, security
- `docs/SRS.md` — functional and non-functional requirements, categorization, limits
- `docs/RTM.md` — requirements traceability matrix
- `docs/Use_Case_Catalog.md` — actors, use cases grouped by actor, relationship semantics
- `docs/Use_Case_Diagram.mmd` — use case diagram source (Mermaid)
- `docs/Use_Case_Diagram.png` — rendered use case diagram
- `docs/Validation_Specification.md` — validation criteria and penetration test plan
- `docs/Phase_1_Checklist.md` — deliverable index mapping requirements to files

Supporting:

- `docs/Architecture.md` — modules and design decisions
- `docs/Test_Cases.md` — functional test cases and non-functional verification procedures
- `docs/Test_Report.md` — recorded validation evidence
- `docs/Sprint_Backlog.md` — Sprint 1 & 2 backlog, story points, and team allocations (Part-2)
- `docs/Jira_Backlog.md` — planned issue breakdown
- `docs/Project_Status.md` — current state and next controlled step

## Development workflow

1. Create/update a Jira issue.
2. Create a feature branch using the issue key.
3. Implement the smallest complete change.
4. Add/update tests.
5. Run `make test`.
6. Open a pull request for review.
7. Merge only after tests and review pass.
8. Update the RTM and Jira status.

## Out of scope for the initial release

Full shell scripting, variable expansion, globbing, command substitution, aliases, job-control signals, and complete POSIX grammar.
