# Executor coverage expansion

Local validation on 3 November 2026; production C/header revision remains `859b43ea8847b9abff28b9ea2c537b4c593deb5e`. Only test files changed for this expansion. Nothing was committed or pushed.

| Metric | Baseline | Expanded | Change |
|---|---:|---:|---:|
| Overall production lines | 628/883 (71.12%) | 671/883 (75.99%) | +43 lines, +4.87 percentage points |
| Overall branch edges | 364/576 (63.19%) | 399/576 (69.27%) | +35 edges, +6.08 percentage points |
| Executor lines | 158/278 (56.83%) | 201/278 (72.30%) | +43 lines, +15.47 percentage points |
| Executor branch edges | 104/226 (46.02%) | 139/226 (61.50%) | +35 edges, +15.48 percentage points |

## Behavioral additions

1. Parent builtin input/combined redirection and missing-input/failed-output behavior. A distinct stdin sentinel verifies restoration; the capture helper compares stdout descriptor identity immediately after executor return, before its own restoration.
2. Foreground/background children install SIG_IGN for SIGTERM before publishing readiness. Tests require the grace interval, bounded exit 143 and no live fixture afterward, reaching forced-cleanup branches.
3. A foreground SIGTERM exit maps to 128+signal. Background signal termination is reaped both through reap_background and during foreground execution.
4. Three background jobs complete out of order; tests verify the middle PID is reaped, other jobs stay alive, and remaining tracking survives cleanup. Fixture readiness waits for exec under /proc before signaling.
5. A real RLIMIT_NOFILE/EMFILE condition allows pipe #1 and its first child, then denies pipe #2. Assertions verify failure status, stable descriptor count, no remaining direct child and successful subsequent execution.

The executor harness increased from 29 to 45 checks, and Python termination scenarios from two to four. Each final standard, ASan/UBSan and coverage run passed 182 checks/scenarios. The full Valgrind target passed with zero reported errors. Instrumentation-related fixture failures and their fixes are recorded in the manifest with retained logs, not erased.

## Remaining limits

Child-side exec/_exit counters still may not be written. The instrumented denominator remains unchanged; no lines were excluded to raise the result. Additional dup/dup2/fork/wait/allocation failures, background capacity and interactive job-control paths are not exhaustively covered. Overall 80% line coverage would require another 36 of the current 883 instrumented lines; this is not a supplied course acceptance threshold.

Reports: [line coverage](Code_Coverage_Report.md), [branch coverage](Branch_Coverage_Report.md), [unit](Unit_Test_Report.md), [system](System_Validation_Report.md). Current raw evidence and exact hashes: [manifest](evidence/Validation_Manifest.md). Previous metrics and raw gcov JSON are retained under docs/evidence/baseline/.
