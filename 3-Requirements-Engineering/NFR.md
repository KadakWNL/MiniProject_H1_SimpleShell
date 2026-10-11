# Non-Functional Requirements

Source of truth: [docs/SRS.md, Section 6](../docs/SRS.md).

| ID | Requirement | Verification summary |
|---|---|---|
| NFR-01 | Reliability | Supported and malformed-input usage should not crash the shell. |
| NFR-02 | Maintainability | Parsing, built-ins, execution and shell-loop responsibilities are separated into modules and headers. |
| NFR-03 | Memory Safety | Parser-owned memory and file descriptors are released on normal/error paths; use sanitizers. |
| NFR-04 | Usability | Prompt and diagnostics are consistent and understandable. |
| NFR-05 | Portability | Target Linux with C11-capable GCC and POSIX APIs. |
| NFR-06 | Testability | Every FR maps to one or more test cases in the RTM. |
| NFR-07 | Security Hygiene | Avoid system() for external execution and validate parser/operator syntax before execution. |
| NFR-08 | Documentation | Document build, installation, usage, supported syntax, limits and tests. |

For full wording and acceptance context, refer to the canonical SRS linked above.
