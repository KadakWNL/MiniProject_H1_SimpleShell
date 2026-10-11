# Test Plan

## Objectives
Validate Simple Shell against its functional requirements and non-functional constraints. Record reproducible results and never mark an expected result as passed before it has actually been run.

## Scope
**In scope:** interactive input/termination; built-ins; external commands; whitespace, quotes, escapes and operators; redirection; pipelines; background execution; malformed input; missing commands/files; process/resource handling; CI and sanitizer checks.

**Out of scope:** full POSIX shell scripting, variable expansion, globbing, command substitution, aliases, full job control, remote services, and tests against systems not owned by the team.

## Test levels

| Level | Method | Evidence |
|---|---|---|
| Unit/component | Parser and targeted module tests | Actual parser output |
| Integration | Shell command scenarios through the harness | Actual integration-test output |
| Robustness/security | Malformed input, missing files, limits and repeated runs | Inputs, outputs and sanitizer diagnostics |
| System/acceptance | Fresh clone, build and command demonstration | Test report and demo evidence |
| CI | GitHub Actions on push and pull request | Workflow URL and status |

## Environment
Linux (x86_64 or agreed team VM), GCC in C11 mode, GNU Make, Bash, GitHub Actions, and AddressSanitizer/UndefinedBehaviorSanitizer for the sanitizer target. Record OS/compiler versions, commit SHA, commands and complete output.

## Procedure
From a clean checkout, run:

    make clean
    make
    make test
    make asan-test

Also exercise the documented manual cases: escaped spaces, background prompt return/reaping, missing input files, foreground waiting and long-pipeline stress. Store actual output in the execution report.

## Entry criteria
- Source, headers and tests are checked out.
- GCC, Make and Bash are available.
- Target commit and expected results are identified.

## Exit criteria
- Automated tests are run and outcomes recorded.
- Failures and deviations are explained and fixes re-tested.
- Manual coverage gaps are exercised or documented as unresolved.
- Sanitizer output and test report reflect real evidence.

## Defect lifecycle
Reproduce → link to requirement/test and issue → patch on feature branch → add regression test → run focused and regression tests → open PR → review/CI → merge → record retest evidence.

Detailed mappings remain in docs/RTM.md, docs/Test_Cases.md and docs/Validation_Specification.md.
