# Test Cases — Simple Shell

Test cases are split into **functional** tests (TC-xx) and **non-functional** verification
procedures (NFRV-xx). Both sets are referenced from `docs/RTM.md`.

## 1. Functional test cases

| ID | Scenario | Input / Action | Expected | Execution |
|---|---|---|---|---|
| TC-01 | Start/exit | `exit` | Shell terminates normally. | Automated * |
| TC-02 | External command | `ls -l` | Directory listing is produced. | Automated |
| TC-03 | Built-in echo | `echo hello` | `hello` printed. | Automated |
| TC-04 | Directory change | `cd /tmp` then `pwd` | `/tmp` printed. | Automated |
| TC-05 | Quoted argument | `echo "hello world"` | One argument printed with space preserved. | Automated |
| TC-06 | Escaped character | `echo hello\ world` | `hello world` printed as a single argument. | Manual |
| TC-07 | Input redirection | `cat < file` | File contents printed. | Automated |
| TC-08 | Output overwrite | `echo x > file` | File contains `x`. | Automated |
| TC-09 | Output append | `echo y >> file` | `y` appended. | Automated |
| TC-10 | EOF | Ctrl-D | Shell exits cleanly. | Automated * |
| TC-11 | Syntax error | `echo "unterminated` | Error shown on stderr; shell remains alive. | Automated |
| TC-12 | Two-command pipe | `printf "a\nb\n" \| wc -l` | `2` produced. | Automated |
| TC-13 | Background | `sleep 2 &` | Prompt returns without waiting; child is later reaped. | Manual |
| TC-14 | Invalid command | `not-a-real-command` | Error shown on stderr; shell remains alive. | Automated |
| TC-15 | Missing input file | `cat < missing` | Error shown on stderr; shell remains alive. | Manual |
| TC-16 | Foreground synchronization | `sleep 1` | Next prompt appears only after the process completes. | Manual |
| TC-17 | Stress/regression | Automated suite repeated; long pipeline such as `seq 10000 \| wc -l` | No crashes and no failing assertions. | Manual |
| TC-18 | Built-in help | `help` | Command list is printed, including the supported operators. | Automated |

\* TC-01 and TC-10 are exercised on every automated run — the harness sends `exit` and then closes
stdin — but neither is asserted as an independent scenario.

The `Execution` column reflects the cases asserted by `tests/test_shell.sh`. Cases marked Manual are
not yet automated; this gap is recorded in `docs/RTM.md` Section 4.

## 2. Non-functional verification procedures

Non-functional requirements are not verified by functional test cases. Each NFR in
`docs/SRS.md` maps to one of the procedures below.

| ID | NFR | Procedure | Pass condition |
|---|---|---|---|
| NFRV-01 | NFR-01 Reliability | Repeat `make test` several times; run long pipelines and repeated malformed input | No crashes, no failing assertions |
| NFRV-02 | NFR-02 Maintainability | Review module and header separation | Parsing, built-ins, execution and the shell loop are separate modules with headers |
| NFRV-03 | NFR-03 Memory safety | Run `make asan-test` | ASan and UBSan report no errors and no leaks |
| NFRV-04 | NFR-04 Usability | Review prompt and diagnostic output | Prompt and messages are consistent and understandable |
| NFRV-05 | NFR-05 Portability | Build on the target Linux system with GCC in C11 mode | Compiles with the Makefile warning flags enabled |
| NFRV-06 | NFR-06 Testability | Push a commit and confirm CI runs `make test` | CI job passes on every push and pull request |
| NFRV-07 | NFR-07 Security hygiene | Run the malformed-input cases; grep the source for `system(` | No `system()` usage; malformed input is rejected without unsafe behaviour |
| NFRV-08 | NFR-08 Documentation | Review docs against the supported syntax and the parser limits | Documented syntax and limits match the implementation |

## 3. How to run

```bash
make            # build
make test       # parser unit tests + shell integration tests
make asan-test  # rebuild under ASan/UBSan and run the same tests
```

Use `make asan-test`, not `make asan && make test`. `make asan` only produces a sanitized binary; the
following `make test` would rebuild without sanitizer flags, so nothing would actually be validated
under ASan or UBSan.

## 4. Known coverage gaps

- TC-06, TC-13, TC-15, TC-16 and TC-17 are documented but not automated.
- TC-13 (background) and TC-16 (foreground synchronisation) are the only tests backing FR-07 and
  FR-09, so those requirements currently have no automated regression protection.
- TC-16 is timing-dependent and should be automated only with an assertion that tolerates scheduler
  noise.
