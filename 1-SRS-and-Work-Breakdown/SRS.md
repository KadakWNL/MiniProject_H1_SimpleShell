# Software Requirements Specification (SRS) — Simple Shell

## 1. Purpose

This document defines the functional and non-functional requirements for Team H1's Simple Shell
project, and categorises them so that each requirement can be validated and traced.

## 2. Scope

The system is a lightweight Unix-like command-line interpreter for Linux. It reads commands
interactively, parses them, executes built-ins or external programs, supports basic I/O redirection
and pipelines, and reports errors.

## 3. Definitions

- **Shell:** command-line interpreter developed by the project.
- **Built-in:** command implemented inside the shell process.
- **External command:** executable launched as a child process.
- **Pipeline:** sequence of commands connected by `|`.
- **Foreground command:** shell waits for command completion.
- **Background command:** command launched without blocking the interactive prompt.
- **FR:** Functional Requirement — an observable behaviour of the shell.
- **NFR:** Non-Functional Requirement — a quality constraint the behaviour must satisfy.
- **RTM:** Requirements Traceability Matrix.
- **UC:** Use case, identified in `docs/Use_Case_Catalog.md`.

## 4. Requirements Categorization

Every requirement is assigned exactly one **category** and one **validation focus**. The validation
focus separates functional, security and non-functional validation.

### 4.1 By category

| Category | Definition | Count | Identifiers |
|---|---|---|---|
| Functional Requirement (FR) | An observable behaviour the shell must provide | 10 | FR-01 … FR-10 |
| Non-Functional Requirement (NFR) | A quality constraint on how the behaviour is delivered | 8 | NFR-01 … NFR-08 |
| **Total** | | **18** | |

### 4.2 By validation focus

| ID | Category | Validation focus | Verified by |
|---|---|---|---|
| FR-01 | Functional | Functional | TC-01, TC-10 |
| FR-02 | Functional | Functional | TC-02 |
| FR-03 | Functional | Functional | TC-03, TC-04, TC-18 |
| FR-04 | Functional | Functional | TC-05, TC-06, TC-11 |
| FR-05 | Functional | Functional | TC-07, TC-08, TC-09 |
| FR-06 | Functional | Functional | TC-12 |
| FR-07 | Functional | Functional | TC-13 |
| FR-08 | Functional | Functional **and** Security | TC-11, TC-14, TC-15 |
| FR-09 | Functional | Functional | TC-16 |
| FR-10 | Functional | Functional | TC-01, TC-10 |
| NFR-01 | Non-functional | Non-functional | NFRV-01, TC-17 |
| NFR-02 | Non-functional | Non-functional | NFRV-02 |
| NFR-03 | Non-functional | Non-functional | NFRV-03 |
| NFR-04 | Non-functional | Non-functional | NFRV-04 |
| NFR-05 | Non-functional | Non-functional | NFRV-05 |
| NFR-06 | Non-functional | Non-functional | NFRV-06 |
| NFR-07 | Non-functional | **Security** | NFRV-07 |
| NFR-08 | Non-functional | Non-functional | NFRV-08 |

### 4.3 Security-flavoured requirements

Three requirements carry an explicit security obligation and are validated as such:

| ID | Security obligation |
|---|---|
| FR-08 | Malformed syntax and failed system calls are reported without terminating the shell |
| NFR-03 | Parser-owned memory is released and file descriptors closed on normal and error paths |
| NFR-07 | External commands use `execvp` rather than `system()`; operator syntax is validated before execution |

### 4.4 Category labels

Requirements are grouped into two categories:

- **FR** — Functional Requirements, the observable behaviour of the shell.
- **NFR** — Non-Functional Requirements, the quality constraints on that behaviour. Requirements
  referred to as VFR's are captured in this category.

Validation is deliberately **not** modelled as a third requirement category. Whether a requirement is
validated for functional behaviour, for security, or for non-functional quality is recorded as its
**validation focus** in Section 4.2, which keeps each requirement in exactly one category while still
separating the three validation streams.

## 5. Functional Requirements

### FR-01 — Interactive Input
The shell shall display a prompt, accept one command line at a time, and continue until `exit` or end-of-file is received.

**Acceptance:** Given a valid command line, the shell reads it and returns to the prompt after processing it.

### FR-02 — External Command Execution
The shell shall execute valid external programs by creating a child process and invoking the selected executable.

**Acceptance:** `ls`, `date`, `printf`, and similar available programs execute successfully when invoked with valid arguments.

### FR-03 — Built-in Commands
The shell shall support `cd`, `pwd`, `echo`, `help`, and `exit`.

**Acceptance:** Each command performs its documented operation and reports invalid arguments appropriately.

### FR-04 — Command Parsing
The shell shall parse whitespace-separated arguments, single- and double-quoted strings, backslash escaping, and the supported operators `<`, `>`, `>>`, `|`, `&`.

**Acceptance:** `echo "hello world"` produces one argument containing `hello world`.

### FR-05 — Input/Output Redirection
The shell shall support input `<`, overwrite output `>`, and append output `>>`, for both built-in and external commands.

**Acceptance:** `echo hi > f` and `cat < f` behave as specified for built-ins and for external programs.

### FR-06 — Pipelines
The shell shall connect multiple commands using `|`, including pipelines in which a segment is a built-in.

**Acceptance:** `printf "a\nb\n" | wc -l` produces the expected line count.

### FR-07 — Background Execution
The shell shall support commands ending with `&` without waiting for completion.

**Acceptance:** `sleep 2 &` returns the prompt immediately and the process is eventually reaped.

### FR-08 — Error Handling
The shell shall provide useful errors for invalid commands, malformed syntax, inaccessible files, and system-call failures without terminating the shell unexpectedly.

**Acceptance:** An invalid command returns to the prompt and the shell remains usable.

### FR-09 — Process Synchronization
The shell shall wait for foreground child processes and capture their completion status.

**Acceptance:** A foreground command does not return control to the next prompt until the command completes.

### FR-10 — Clean Termination
The shell shall terminate on `exit` and handle end-of-file without leaking allocated parser resources.

**Acceptance:** Both `exit` and Ctrl-D/EOF end the interactive loop cleanly.

## 6. Non-Functional Requirements

### NFR-01 — Reliability
The shell shall not crash during supported normal usage or malformed-input tests.

### NFR-02 — Maintainability
Parsing, built-ins, execution, and shell-loop responsibilities shall be separated into source modules with headers.

### NFR-03 — Memory Safety
The project shall free parser-owned heap memory and close file descriptors on normal and error paths. Sanitizer runs shall be part of quality validation.

### NFR-04 — Usability
The prompt and error messages shall be consistent and understandable.

### NFR-05 — Portability
The supported development target is Linux with a C11-capable GCC toolchain and POSIX APIs.

### NFR-06 — Testability
Every functional requirement shall map to one or more test cases in the RTM.

### NFR-07 — Security Hygiene
The implementation shall avoid `system()` for external command execution and shall validate parser/operator syntax before execution.

### NFR-08 — Documentation
Build, installation, usage, supported syntax, documented limits, and testing instructions shall be documented.

## 7. Constraints

- Linux is the official target environment.
- The shell is not intended to be full POSIX-shell compliant.
- Advanced job control, shell variables, glob expansion, command substitution, and scripting are outside the initial scope.

### 7.1 Documented implementation limits

These limits are enforced by the parser and are part of the documented behaviour:

| Limit | Value | Behaviour when exceeded |
|---|---|---|
| Maximum arguments per command | 128 | Syntax error: `too many arguments or memory allocation failure` |
| Maximum pipeline segments | 32 | Syntax error: `pipeline is too long` |
| Multiple input redirections on one command | not supported | Syntax error: `multiple input redirections are not supported` |
| Multiple output redirections on one command | not supported | Syntax error: `multiple output redirections are not supported` |
| Multiple background operators | not supported | Syntax error: `multiple background operators are not supported` |
| `&` in a non-final position | not supported | Syntax error: `background operator must be at the end` |
| Unterminated quote | not supported | Syntax error: `unterminated quote` |
| Trailing backslash | not supported | Syntax error: `trailing escape character` |

## 8. Assumptions

- GCC and POSIX APIs are available.
- The user has access to normal Linux executables for functional testing.
- Four team members collaborate through GitHub and Jira.

## 9. Acceptance Summary

The release is accepted when all in-scope FRs are implemented, mapped in the RTM, covered by tests,
reviewed, documented, and the final test suite passes on the agreed Linux environment.
