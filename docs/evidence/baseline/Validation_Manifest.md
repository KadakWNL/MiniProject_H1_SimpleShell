# Validation manifest ? 3 November 2026

## Identity and environment

Production source revision: `859b43ea8847b9abff28b9ea2c537b4c593deb5e` (main, PR #32 merged). Production C files and headers are unchanged in this local submission candidate. The candidate adds documentation, evidence, and a coverage target/report generator; it has not been committed or pushed. Exact input hashes and tool versions are in [environment.txt](environment.txt). No future commit SHA or reviewer approval is fabricated.

Environment: Kali Linux under WSL2, Linux 6.6.87.2-microsoft-standard-WSL2 x86_64; GCC/gcov 16.2.0; Python 3.14.7. The full environment capture is authoritative for the remaining tools. Tests ran in the working checkout, not a fresh clone. Build variants ran sequentially, each with clean rebuilds.

## Executed commands and results

| Command | Exit | Evidence |
|---|---:|---|
| `make clean && make test` | 0 | [standard.log](standard.log) |
| `make asan-test` | 0 | [sanitizers.log](sanitizers.log) |
| `make clean && make valgrind` | 0 | [valgrind.log](valgrind.log) |
| `make coverage` | 0 | [command log](coverage-command.log), [tests](coverage/test.log), [gcov](coverage/gcov.log), compressed gcov JSON in coverage/ |

Standard, sanitizer and instrumented runs each passed 63 parser assertions, 29 executor checks, 70 shell scenarios and two Python termination scenarios (164 total). No failures were observed in these runs. Historical tests/retests are retained in [the earlier report](../Test_Report_Pre_Audit.md); they are not substituted for this run.

## Interpretation and assurance limits

No compiler warnings appeared in these runs. Sanitizers reported no errors in covered execution; Valgrind reported zero errors and zero bytes live at exit for the three top-level parser/executor/shell processes. Forked builtin children report inherited allocations as still reachable (for example 143 bytes in the shell smoke), with zero definitely/indirectly/possibly lost bytes. Still reachable is not the same as a lost allocation; the log is retained rather than claiming every child frees its inherited heap. These are observations, not proof of universal leak freedom. Valgrind does not run the full shell or SIGTERM suite, and external programs are not instrumented for coverage.

Gcov records instrumented production lines and branch edges only. Fork/exec/_exit children may not flush counters. Fault injection for allocation/pipe/fork/wait failures, detached process groups, interactive terminal job control, manual usability and instructor acceptance are not established by these runs. The harness accepts a descendant zombie as no longer running, so the termination regression does not claim the shell reaps grandchildren. See [resource audit](../Resource_Audit.md).

No final video, release tag, clean-clone verification at the final candidate, or joint sign-off was produced here. This package remains local per instruction.
