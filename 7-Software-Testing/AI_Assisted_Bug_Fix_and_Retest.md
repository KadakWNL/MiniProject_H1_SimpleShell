# AI-Assisted Bug Fix, Patch and Retest Record

Complete only after a real defect has been reproduced and fixed. Do not invent bugs or claim tests passed without running them.

## 1. Defect
- GitHub issue:
- Requirement / test ID:
- File and function:
- Commit SHA:
- Date and environment:

## 2. Reproduction before fix
- Input/command:
- Expected result:
- Actual result:
- Steps:
- Evidence link:

## 3. Root cause
Describe the observed cause; distinguish confirmed findings from hypotheses.

## 4. AI assistance
- Tool used:
- Prompt/task:
- Suggestion accepted or modified:
- Human review:

## 5. Patch
- Branch:
- Commit / PR:
- Files changed:
- Why the change addresses the cause:
- Regression test added:

## 6. Retest after fix
| Check | Command/input | Actual result | Pass/Fail | Evidence |
|---|---|---|---|---|
| Targeted regression | | | | |
| make test | | | | |
| make asan-test (if applicable) | | | | |

## 7. Conclusion
- Fix confirmed:
- Remaining limitation:
- Reviewer/merge reference:
