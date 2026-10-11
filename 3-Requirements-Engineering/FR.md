# Functional Requirements

Source of truth: [docs/SRS.md, Section 5](../docs/SRS.md).

| ID | Requirement | Acceptance summary |
|---|---|---|
| FR-01 | Interactive Input | Display a prompt, accept one command at a time, and continue until exit or EOF. |
| FR-02 | External Command Execution | Run valid external programs in a child process. |
| FR-03 | Built-in Commands | Support cd, pwd, echo, help and exit. |
| FR-04 | Command Parsing | Parse whitespace-separated args, quotes, escapes and supported operators. |
| FR-05 | I/O Redirection | Support input <, output overwrite > and append >> for built-ins and external commands. |
| FR-06 | Pipelines | Connect commands with |, including a built-in pipeline segment. |
| FR-07 | Background Execution | Commands ending in & run without waiting for completion. |
| FR-08 | Error Handling | Report invalid commands, syntax, file and syscall failures without unexpected shell termination. |
| FR-09 | Process Synchronization | Wait for foreground child processes and capture status. |
| FR-10 | Clean Termination | Exit on exit/EOF and release parser resources. |

For full wording and criteria, refer to the canonical SRS linked above.
