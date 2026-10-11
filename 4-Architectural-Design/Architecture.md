# Architecture and Design — Simple Shell

## High-level flow

```text
User
  |
  v
Shell Loop (read input)
  |
  v
Lexer / Parser
  |
  +--> Built-in in parent (foreground single command)
  |
  +--> Process Executor
          |
          +--> fork/exec
          +--> I/O redirection (open/dup2)
          +--> pipeline (pipe/dup2)
          +--> waitpid for foreground
          +--> periodic reap for background children
```

## Module responsibilities

| Module | Responsibility |
|---|---|
| `main.c` | Entry point. |
| `shell.c` | Prompt, input loop, parsing/execution orchestration. |
| `parser.c` | Lexer, operator recognition, command/pipeline construction, memory cleanup. |
| `builtins.c` | Built-in semantics. `cd` must execute in the parent for persistent directory changes. |
| `executor.c` | Child process creation, redirection, pipelines, waiting, background reaping. |

## Important design decisions

1. External commands use `fork` + `execvp`, not `system`.
2. Single foreground built-ins execute in the shell process so state changes such as `cd` persist.
3. Built-ins participating in pipelines execute in the child process.
4. Prompts are written to `stderr`, so command output on `stdout` remains clean for automated testing/redirection.
5. The initial project uses periodic `waitpid(..., WNOHANG)` calls to reap background children rather than a signal-handler-based reaper, simplifying foreground wait behavior.
