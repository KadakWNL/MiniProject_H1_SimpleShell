#!/usr/bin/env bash
set -euo pipefail

SHELL_BIN=${1:-./simple-shell}
PASS=0
FAIL=0
TMP_DIR=$(mktemp -d /tmp/simple-shell-test-XXXXXX)
trap 'rm -rf "$TMP_DIR"' EXIT

run_test() {
    local name=$1
    local input=$2
    local expected=$3
    local actual
    actual=$(printf '%s' "$input" | "$SHELL_BIN" 2>/dev/null)
    if [[ "$actual" == "$expected" ]]; then
        printf 'PASS: %s\n' "$name"
        PASS=$((PASS + 1))
    else
        printf 'FAIL: %s\n' "$name"
        printf '  expected: %q\n' "$expected"
        printf '  actual:   %q\n' "$actual"
        FAIL=$((FAIL + 1))
    fi
}

# Verify that a bad command line produces a diagnostic on stderr and that the
# shell survives it and keeps executing following commands.
run_recovery_test() {
    local name=$1
    local input=$2
    local expected_stdout=$3
    local expected_stderr=$4
    local actual_stdout actual_stderr
    actual_stdout=$(printf '%s' "$input" | "$SHELL_BIN" 2>"$TMP_DIR/stderr.txt")
    actual_stderr=$(cat "$TMP_DIR/stderr.txt")
    if [[ "$actual_stdout" == "$expected_stdout" && "$actual_stderr" == *"$expected_stderr"* ]]; then
        printf 'PASS: %s\n' "$name"
        PASS=$((PASS + 1))
    else
        printf 'FAIL: %s\n' "$name"
        printf '  expected stdout: %q\n' "$expected_stdout"
        printf '  actual stdout:   %q\n' "$actual_stdout"
        printf '  expected stderr to contain: %q\n' "$expected_stderr"
        printf '  actual stderr:   %q\n' "$actual_stderr"
        FAIL=$((FAIL + 1))
    fi
}

# Verify termination status, stdout, and clean exit code.
run_status_test() {
    local name=$1
    local input=$2
    local expected_exit=$3
    local expected_stdout=${4:-}
    local actual_stdout
    local actual_exit=0
    actual_stdout=$(printf '%s' "$input" | "$SHELL_BIN" 2>/dev/null) || actual_exit=$?
    if [[ "$actual_exit" -eq "$expected_exit" && "$actual_stdout" == "$expected_stdout" ]]; then
        printf 'PASS: %s\n' "$name"
        PASS=$((PASS + 1))
    else
        printf 'FAIL: %s\n' "$name"
        printf '  expected exit: %d, stdout: %q\n' "$expected_exit" "$expected_stdout"
        printf '  actual exit:   %d, stdout: %q\n' "$actual_exit" "$actual_stdout"
        FAIL=$((FAIL + 1))
    fi
}

# Verify that prompt is emitted to stderr and command stdout remains clean.
run_prompt_test() {
    local name=$1
    local input=$2
    local expected_prompt=$3
    local actual_stdout actual_stderr
    actual_stdout=$(printf '%s' "$input" | "$SHELL_BIN" 2>"$TMP_DIR/stderr.txt") || true
    actual_stderr=$(cat "$TMP_DIR/stderr.txt")
    if [[ "$actual_stderr" == *"$expected_prompt"* && "$actual_stdout" != *"$expected_prompt"* ]]; then
        printf 'PASS: %s\n' "$name"
        PASS=$((PASS + 1))
    else
        printf 'FAIL: %s\n' "$name"
        printf '  expected stderr to contain: %q\n' "$expected_prompt"
        printf '  actual stderr:              %q\n' "$actual_stderr"
        printf '  actual stdout:              %q\n' "$actual_stdout"
        FAIL=$((FAIL + 1))
    fi
}

# --- Functional tests ---

# TC-01: Clean termination with exit
run_status_test "clean termination with exit" $'exit\n' 0 ""

# TC-10: Clean termination on EOF without exit
run_status_test "clean termination on EOF" "" 0 ""
run_status_test "clean termination on EOF after command" $'echo eof-test\n' 0 "eof-test"

# Prompt output on standard error without polluting standard output
run_prompt_test "prompt on stderr and clean stdout" $'echo prompt-check\nexit\n' "simple-shell$ "

# Blank and whitespace-only lines are ignored without executing commands
run_status_test "blank and whitespace-only input" $'\n   \n\t  \n' 0 ""

# Continued command processing after blank lines
run_status_test "continued command processing after blank lines" \
    $'\n   \necho resumed\n   \n\techo running\nexit\n' 0 $'resumed\nrunning'

# TC-03: Built-in echo
run_test "echo builtin" $'echo hello\nexit\n' "hello"

# Multi-argument echo
run_test "echo multiple arguments" $'echo one two three four five\nexit\n' "one two three four five"

# TC-05: Quoted argument
run_test "quoted echo" $'echo "hello world"\nexit\n' "hello world"

# TC-06: Escaped character (previously manual)
run_test "escaped space" $'echo hello\\ world\nexit\n' "hello world"

# TC-02: External command execution
run_test "external command" $'printf hi\nexit\n' "hi"
run_test "external command with formatted arguments" $'printf "%s %s" foo bar\nexit\n' "foo bar"

# TC-18: Built-in help
run_test "help builtin" $'help\nexit\n' $'Simple Shell - supported commands:\n  cd [DIR]       Change current directory\n  pwd            Print current directory\n  echo [TEXT...] Print text\n  help           Show this help message\n  exit           Exit the shell\n\nExternal commands, pipes (|), redirection (<, >, >>),\nand background execution (&) are supported.'

# TC-07: Input redirection
printf 'redirection-input\n' > "$TMP_DIR/in.txt"
input=$(printf 'cat < %s/in.txt\nexit\n' "$TMP_DIR")
run_test "input redirection" "$input" "redirection-input"

# External command combined input and output redirection
printf 'combined-in-out\n' > "$TMP_DIR/in_both.txt"
input=$(printf 'cat < %s/in_both.txt > %s/out_both.txt\ncat %s/out_both.txt\nexit\n' "$TMP_DIR" "$TMP_DIR" "$TMP_DIR")
run_test "combined input and output redirection" "$input" "combined-in-out"

# TC-08: Output overwrite (built-in)
input=$(printf 'echo output > %s/out.txt\ncat %s/out.txt\nexit\n' "$TMP_DIR" "$TMP_DIR")
run_test "output redirection" "$input" "output"

# Output overwrite (external command)
input=$(printf 'printf "external-out\\n" > %s/ext_out.txt\ncat %s/ext_out.txt\nexit\n' "$TMP_DIR" "$TMP_DIR")
run_test "external command output redirection" "$input" "external-out"

# Truncation: overwrite pre-existing longer content with shorter content (built-in)
printf 'Initial very long line of text that should be completely truncated\n' > "$TMP_DIR/trunc.txt"
input=$(printf 'echo short > %s/trunc.txt\ncat %s/trunc.txt\nexit\n' "$TMP_DIR" "$TMP_DIR")
run_test "built-in output redirection truncates existing file" "$input" "short"

# Truncation: overwrite pre-existing longer content with shorter content (external command)
printf 'Initial very long line of text for external truncation verification\n' > "$TMP_DIR/trunc_ext.txt"
input=$(printf 'printf "shorter\\n" > %s/trunc_ext.txt\ncat %s/trunc_ext.txt\nexit\n' "$TMP_DIR" "$TMP_DIR")
run_test "external command output redirection truncates existing file" "$input" "shorter"

# TC-09: Output append (built-in)
input=$(printf 'echo second >> %s/out.txt\ncat %s/out.txt\nexit\n' "$TMP_DIR" "$TMP_DIR")
run_test "append redirection" "$input" $'output\nsecond'

# Output append (external command)
printf 'line1\n' > "$TMP_DIR/ext_app.txt"
input=$(printf 'printf "line2\\n" >> %s/ext_app.txt\ncat %s/ext_app.txt\nexit\n' "$TMP_DIR" "$TMP_DIR")
run_test "external command append redirection" "$input" $'line1\nline2'

# Redirection isolation: verify redirected output does not leak to shell standard output
input=$(printf 'echo hidden > %s/silent_builtin.txt\nexit\n' "$TMP_DIR")
run_test "built-in output redirection emits no stdout" "$input" ""
input=$(printf 'printf hidden > %s/silent_ext.txt\nexit\n' "$TMP_DIR")
run_test "external output redirection emits no stdout" "$input" ""

# Standard output restoration for subsequent commands
input=$(printf 'echo to_file > %s/f1.txt\necho normal-stdout\nexit\n' "$TMP_DIR")
run_test "subsequent command stdout restored after builtin redirection" "$input" "normal-stdout"
input=$(printf 'printf to_file > %s/f2.txt\nprintf normal-stdout\nexit\n' "$TMP_DIR")
run_test "subsequent command stdout restored after external redirection" "$input" "normal-stdout"

# Built-in command redirection to file
input=$(printf 'help > %s/help.txt\ncat %s/help.txt\nexit\n' "$TMP_DIR" "$TMP_DIR")
run_test "built-in output redirection to file" "$input" $'Simple Shell - supported commands:\n  cd [DIR]       Change current directory\n  pwd            Print current directory\n  echo [TEXT...] Print text\n  help           Show this help message\n  exit           Exit the shell\n\nExternal commands, pipes (|), redirection (<, >, >>),\nand background execution (&) are supported.'

# Stress test: repeated redirections to verify file descriptor hygiene (no leak/exhaustion)
stress_input=""
for i in $(seq 1 50); do
    stress_input+="echo val$i > $TMP_DIR/stress.txt"$'\n'
    stress_input+="cat < $TMP_DIR/stress.txt >> $TMP_DIR/stress_acc.txt"$'\n'
done
stress_input+="echo stress-complete"$'\n'"exit"$'\n'
run_test "repeated redirection stress (50 cycles) fd hygiene" "$stress_input" "stress-complete"

# TC-12: Pipeline execution
run_test "pipeline" $'printf "a\\nb\\nc\\n" | wc -l\nexit\n' "3"

# Multi-stage pipeline execution (3 stages)
run_test "three-stage pipeline" $'printf "alpha\\nbeta\\ngamma\\n" | grep -v beta | wc -l\nexit\n' "2"

# Multi-stage pipeline execution (4 stages)
run_test "four-stage pipeline" $'printf "delta\\nbeta\\nalpha\\ngamma\\n" | grep -v beta | sort | head -n 2\nexit\n' $'alpha\ndelta'

# Multi-stage pipeline execution (5 stages)
run_test "five-stage pipeline" $'printf "5\\n4\\n3\\n2\\n1\\n" | cat | sort -n | cat | wc -l\nexit\n' "5"

# Multi-stage pipeline with built-in commands (middle/head/tail)
run_test "multi-stage pipeline with builtin producer" $'echo multi-stage-echo | cat | cat\nexit\n' "multi-stage-echo"

# Multi-stage pipeline combining input and output redirection
printf "item1\nitem2\nitem3\nitem4\n" > "$TMP_DIR/ms_in.txt"
input=$(printf 'cat < %s/ms_in.txt | grep -v item2 | wc -l > %s/ms_out.txt\ncat %s/ms_out.txt\nexit\n' "$TMP_DIR" "$TMP_DIR" "$TMP_DIR")
run_test "multi-stage pipeline with input and output redirection" "$input" "3"

# TC-17: Multi-stage pipeline deadlock prevention with large data stream (> 64KB pipe capacity)
run_test "multi-stage pipeline deadlock prevention large transfer" $'seq 30000 | cat | cat | wc -l\nexit\n' "30000"

# Multi-stage pipeline early consumer exit terminates without hang (SIGPIPE handling)
run_test "multi-stage pipeline early consumer exit without deadlock" $'seq 50000 | cat | cat | head -n 3 | wc -l\nexit\n' "3"

# Multi-stage pipeline built-in state isolation
orig_pwd=$(pwd)
actual_pwd=$(printf 'cd /tmp | cat | cat\npwd\nexit\n' | "$SHELL_BIN" 2>/dev/null)
if [[ "$actual_pwd" == "$orig_pwd" ]]; then
    printf 'PASS: multi-stage pipeline cd does not mutate parent directory\n'
    PASS=$((PASS + 1))
else
    printf 'FAIL: multi-stage pipeline cd mutated parent directory (actual=%q, expected=%q)\n' "$actual_pwd" "$orig_pwd"
    FAIL=$((FAIL + 1))
fi

run_test "multi-stage pipeline exit does not terminate parent" $'exit | cat | cat\necho alive-after-multistage-exit\nexit\n' "alive-after-multistage-exit"

# Pipeline throughput under AddressSanitizer
run_test "empty stream pipeline" $'cat /dev/null | wc -c\nexit\n' "0"

# TC-14: Invalid command handling (diagnostic + prompt recovery)
run_recovery_test "invalid command recovery" \
    $'definitely-not-a-command\necho alive\nexit\n' "alive" "definitely-not-a-command:"

# TC-04: Directory change (cd & pwd)
actual=$(printf 'cd /tmp\npwd\nexit\n' | "$SHELL_BIN" 2>/dev/null)
if [[ "$actual" == "/tmp" ]]; then
    printf 'PASS: cd builtin\n'; PASS=$((PASS + 1))
else
    printf 'FAIL: cd builtin (actual=%q)\n' "$actual"; FAIL=$((FAIL + 1))
fi

# Directory change with cd - (OLDPWD)
mkdir -p "$TMP_DIR/cd_d1" "$TMP_DIR/cd_d2"
actual=$(printf 'cd '"$TMP_DIR"'/cd_d1\ncd '"$TMP_DIR"'/cd_d2\ncd -\nexit\n' | "$SHELL_BIN" 2>/dev/null)
if [[ "$actual" == *"$TMP_DIR/cd_d1"* ]]; then
    printf 'PASS: cd - toggle to OLDPWD\n'; PASS=$((PASS + 1))
else
    printf 'FAIL: cd - toggle to OLDPWD (actual=%q)\n' "$actual"; FAIL=$((FAIL + 1))
fi

# TC-11: Syntax / malformed input error recovery
run_recovery_test "unterminated double quote recovery" \
    $'echo "unterminated\necho alive\nexit\n' "alive" "shell: syntax error:"
run_recovery_test "unterminated single quote recovery" \
    $'echo \'unterminated\necho alive\nexit\n' "alive" "shell: syntax error:"
run_recovery_test "trailing backslash recovery" \
    $'echo trailing\\\necho alive\nexit\n' "alive" "shell: syntax error:"
run_recovery_test "dangling pipe recovery" \
    $'echo hi |\necho alive\nexit\n' "alive" "shell: syntax error:"
run_recovery_test "leading pipe recovery" \
    $'| echo hi\necho alive\nexit\n' "alive" "shell: syntax error:"
run_recovery_test "dangling redirection recovery" \
    $'cat <\necho alive\nexit\n' "alive" "shell: syntax error:"
run_recovery_test "redirection without command recovery" \
    $'< in.txt\necho alive\nexit\n' "alive" "shell: syntax error:"
run_recovery_test "recovery across blank lines and malformed syntax" \
    $'\n  \necho "unterminated\n\n  \necho recovered\nexit\n' "recovered" "shell: syntax error:"

# TC-15: Inaccessible / missing input file recovery
input=$(printf 'cat < %s/nonexistent_missing.txt\necho alive\nexit\n' "$TMP_DIR")
run_recovery_test "missing input file diagnostic and recovery" \
    "$input" "alive" "shell: cannot open"

# Failed external command output redirection recovery
input=$(printf 'printf fail > %s/nonexistent_dir/err.txt\necho alive\nexit\n' "$TMP_DIR")
run_recovery_test "failed external output redirection recovery" \
    "$input" "alive" "shell: cannot open"

# Failed built-in output redirection: diagnostic emitted and built-in execution inhibited
actual_stdout=$(printf 'cd '"$TMP_DIR"'/cd_d1\ncd '"$TMP_DIR"'/cd_d2 > '"$TMP_DIR"'/nonexistent_dir/err.txt\npwd\nexit\n' | "$SHELL_BIN" 2>"$TMP_DIR/cd_err.txt")
actual_stderr=$(cat "$TMP_DIR/cd_err.txt")
if [[ "$actual_stdout" == *"$TMP_DIR/cd_d1"* && "$actual_stderr" == *"shell: cannot open"* ]]; then
    printf 'PASS: failed output redirection inhibits built-in execution\n'
    PASS=$((PASS + 1))
else
    printf 'FAIL: failed output redirection inhibits built-in execution\n'
    printf '  actual stdout: %q\n' "$actual_stdout"
    printf '  actual stderr: %q\n' "$actual_stderr"
    FAIL=$((FAIL + 1))
fi

# TC-16: Foreground process synchronization (previously manual)
start_fg=$(date +%s)
printf 'sleep 1\nexit\n' | "$SHELL_BIN" >/dev/null 2>&1
dur_fg=$(( $(date +%s) - start_fg ))
if [[ $dur_fg -ge 1 ]]; then
    printf 'PASS: foreground process synchronization\n'; PASS=$((PASS + 1))
else
    printf 'FAIL: foreground process synchronization (waited %ds, expected >= 1s)\n' "$dur_fg"; FAIL=$((FAIL + 1))
fi

# TC-13: Background non-blocking execution (previously manual)
start_bg=$(date +%s)
printf 'sleep 2 &\nexit\n' | "$SHELL_BIN" >/dev/null 2>&1
dur_bg=$(( $(date +%s) - start_bg ))
if [[ $dur_bg -lt 2 ]]; then
    printf 'PASS: background execution non-blocking\n'; PASS=$((PASS + 1))
else
    printf 'FAIL: background execution non-blocking (waited %ds, expected < 2s)\n' "$dur_bg"; FAIL=$((FAIL + 1))
fi

# TC-17: Background child is asynchronously reaped while shell is idle
fifo="$TMP_DIR/bg_reap_input.fifo"
bg_stdout="$TMP_DIR/bg_reap_stdout.txt"
bg_stderr="$TMP_DIR/bg_reap_stderr.txt"

mkfifo "$fifo"

"$SHELL_BIN" < "$fifo" > "$bg_stdout" 2> "$bg_stderr" &
shell_pid=$!

# Open FIFO read/write so the shell can start without blocking.
exec 3<> "$fifo"

# Launch a background process and keep the shell idle.
printf 'sleep 1 &\n' >&3

bg_pid=""
for attempt in {1..30}; do
    bg_pid=$(sed -n 's/^\[background pid \([0-9][0-9]*\)\]$/\1/p' "$bg_stdout" | head -n 1)
    [[ -n "$bg_pid" ]] && break
    sleep 0.1
done

# Both the initial prompt and the returned prompt must appear before sleep exits.
prompt_count=$(grep -oF 'simple-shell$ ' "$bg_stderr" | wc -l || true)
if [[ "$prompt_count" -ge 2 ]]; then
    printf 'PASS: background launch immediately returns prompt\n'; PASS=$((PASS + 1))
else
    printf 'FAIL: background launch immediately returns prompt\n'; FAIL=$((FAIL + 1))
fi

if [[ -z "$bg_pid" ]]; then
    printf 'FAIL: asynchronous background reaping (could not capture background PID)\n'
    FAIL=$((FAIL + 1))
else
    # Do not send another command. The shell must reap the child while idle.
    sleep 2

    process_state=$(ps -o stat= -p "$bg_pid" 2>/dev/null || true)

    if [[ -z "$process_state" ]]; then
        printf 'PASS: background child asynchronously reaped while shell is idle\n'
        PASS=$((PASS + 1))
    else
        printf 'FAIL: background child still exists after completion (state: %s)\n' "$process_state"
        FAIL=$((FAIL + 1))
    fi
fi

# Shut down the shell cleanly.
printf 'exit\n' >&3
exec 3>&-
wait "$shell_pid" || true
# Reaping must continue while a foreground pipeline is running.
"$SHELL_BIN" < "$fifo" > "$bg_stdout" 2> "$bg_stderr" &
shell_pid=$!
exec 3<> "$fifo"
printf 'sleep 1 &\nsleep 4 | sleep 4\nexit\n' >&3
bg_pid=""
for attempt in {1..30}; do
    bg_pid=$(sed -n 's/^\[background pid \([0-9][0-9]*\)\]$/\1/p' "$bg_stdout" | head -n 1)
    [[ -n "$bg_pid" ]] && break
    sleep 0.1
done
sleep 2
process_state=$(ps -o stat= -p "${bg_pid:-0}" 2>/dev/null || true)
if [[ -n "$bg_pid" && -z "$process_state" ]] && kill -0 "$shell_pid" 2>/dev/null; then
    printf 'PASS: background child reaped during foreground pipeline\n'; PASS=$((PASS + 1))
else
    printf 'FAIL: background child reaped during foreground pipeline (state: %s)\n' "$process_state"; FAIL=$((FAIL + 1))
fi
exec 3>&-
wait "$shell_pid" || true
# A rejected line must neither launch an earlier stage nor create redirect files.
for invalid in \
    "touch $TMP_DIR/must-not-exist | cat > $TMP_DIR/rejected-out > $TMP_DIR/rejected-other" \
    "touch $TMP_DIR/must-not-exist | cat < a < b" \
    "touch $TMP_DIR/must-not-exist |" \
    "touch $TMP_DIR/must-not-exist || cat" \
    "touch $TMP_DIR/must-not-exist & echo unexpected" \
    "touch $TMP_DIR/must-not-exist &&" \
    "touch $TMP_DIR/must-not-exist & &"; do
    run_recovery_test "reject before execution: $invalid" \
        "$invalid"$'\necho recovered\nexit\n' "recovered" "shell: syntax error:"
done
if [[ ! -e "$TMP_DIR/must-not-exist" && ! -e "$TMP_DIR/rejected-out" && ! -e "$TMP_DIR/rejected-other" ]]; then
    printf 'PASS: malformed operators cause no command or redirection side effects\n'
    PASS=$((PASS + 1))
else
    printf 'FAIL: malformed operators caused side effects\n'
    FAIL=$((FAIL + 1))
fi

# Check the full REPL boundary as well as the parser boundary.
max_pipeline='echo boundary'
for ((stage = 1; stage < 32; ++stage)); do
    max_pipeline+=' | cat'
done
run_test "32-stage pipeline executes" "$max_pipeline"$'\nexit\n' "boundary"
run_recovery_test "33-stage pipeline rejected before execution" \
    "$max_pipeline | cat"$'\necho recovered\nexit\n' "recovered" "pipeline is too long"
run_test "quoted operators pass through builtin pipeline" \
    $'echo "|" "&" "<" ">" ">>" | cat\nexit\n' '| & < > >>'

# TC-17: Full regression stress sequence: repeated multi-stage pipelines and redirections
stress_suite_ok=1
for i in {1..20}; do
    out=$(printf 'echo round-%d | cat | cat\nexit\n' "$i" | "$SHELL_BIN" 2>/dev/null)
    if [[ "$out" != "round-$i" ]]; then
        stress_suite_ok=0
        break
    fi
done
if [[ $stress_suite_ok -eq 1 ]]; then
    printf 'PASS: TC-17 multi-stage pipeline regression stress loop (20 cycles)\n'
    PASS=$((PASS + 1))
else
    printf 'FAIL: TC-17 multi-stage pipeline regression stress loop failed\n'
    FAIL=$((FAIL + 1))
fi

printf '\nTotal: %d passed, %d failed\n' "$PASS" "$FAIL"
[[ $FAIL -eq 0 ]]
