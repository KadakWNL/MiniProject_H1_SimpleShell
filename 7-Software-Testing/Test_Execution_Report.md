# Test Report — Simple Shell

## 1. Status

**No validation run has been recorded yet.** This document specifies how to produce the evidence and
what a successful run must show. It deliberately contains no pass/fail claims until a real run is
performed and pasted below.

`docs/RTM.md` records implementation status only. Validation evidence becomes part of the submission
when Sections 3 and 4 are completed with actual output.

## 2. Target environment for validation

| Property | Required |
|---|---|
| OS | Linux, x86_64 (or the team's agreed target Linux VM) |
| Compiler | GCC, C11 mode |
| Build flags | `-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wstrict-prototypes` |
| Sanitizers | AddressSanitizer + UndefinedBehaviorSanitizer |
| Shell for the harness | bash |

Record the exact `gcc --version` and `uname -a` output with the evidence.

## 3. How to produce the evidence

```bash
make clean
make
make test
make asan-test
```

Use `make asan-test`. Running `make asan && make test` does not validate under sanitizers, because
the second `make test` rebuilds without the sanitizer flags.

Paste the complete terminal output below.

## 4. Expected results

The suite is expected to report:

- **Parser unit tests** — 9 assertions, ending with `Parser tests: 0 failed`
  (basic argument parsing, double-quoted argument, pipeline parsing, redirection parsing, append
  redirection parsing, unterminated quote rejected, leading pipe rejected, trailing pipe rejected,
  non-terminal background operator rejected)
- **Shell integration tests** — 11 scenarios, ending with `Total: 11 passed, 0 failed`
  (echo, quoted echo, external command, help, input redirection, output redirection, append
  redirection, pipeline, invalid command, `cd`, malformed input)
- **ASan/UBSan** — no sanitizer diagnostics and no leaks

Total expected: **20 checks, 0 failures**.

If the real run differs from this, the difference must be explained here rather than the expectation
being edited to match.

## 5. Recorded run

Paste the evidence for each phase below.

### 5.1 `make test`

```text
<not yet recorded>
```

### 5.2 `make asan-test`

```text
<not yet recorded>
```

### 5.3 Environment

```text
<not yet recorded>
```

## 6. Manual verification still outstanding

These scenarios are documented but not automated, and must be exercised manually before acceptance.

| Test | What to confirm |
|---|---|
| TC-06 | `echo hello\ world` prints a single argument `hello world` |
| TC-13 | `sleep 2 &` returns the prompt immediately; the child is later reaped |
| TC-15 | `cat < missing` reports an error and the shell stays usable |
| TC-16 | `sleep 1` returns the prompt only after the process completes |
| TC-17 | Long pipelines (`seq 10000 \| wc -l`) and repeated runs produce no crashes |

## 7. Limitations

This report will record local development validation. Final acceptance should also be run on the
team's agreed target Linux VM and against any instructor-provided test specification.
