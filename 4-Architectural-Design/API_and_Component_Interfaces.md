# API and Component Interfaces

Simple Shell is a local C program and does not expose a web/REST API. “API” here refers to module-level C interfaces declared under include/.

| Module | Source | Header | Responsibility |
|---|---|---|---|
| Shell loop | src/shell.c | include/shell.h | Prompt, input loop and orchestration |
| Parser | src/parser.c | include/parser.h | Tokenization, quote/escape handling, operators and parse-result cleanup |
| Built-ins | src/builtins.c | include/builtins.h | cd, pwd, echo, help and exit |
| Executor | src/executor.c | include/executor.h | Process creation, redirection, pipelines, waiting and background cleanup |

## Interaction contract
1. The shell loop receives one command line.
2. The parser returns a structured command representation or a parse error.
3. The shell dispatches standalone built-ins or sends external/pipelined commands to the executor.
4. The executor uses POSIX process/file APIs, reports failures and collects foreground status.
5. Parser-owned resources and opened descriptors are released on success and error paths.

Exact function signatures in the current header files are the implementation source of truth.
