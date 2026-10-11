# Architecture Selection and Justification

## Selected style
The project uses a **layered, modular architecture** for a local command-line interpreter. Responsibilities are separated into the shell loop, parser, built-in handler, execution engine and operating-system services.

## Alternatives considered

| Style | Decision | Reason |
|---|---|---|
| Layered / modular | Selected | The input → parse → execute → output flow is clear; separated modules are easier to test and maintain. |
| Microservices | Not selected | There are no independent network services or deployment needs; microservices add unnecessary complexity. |
| Client-server | Not selected | The shell does not need remote clients, shared server state or a client/server protocol. |

## Two project-specific reasons
1. Parsing and syntax validation can be tested separately from built-in command handling and process execution.
2. Redirection, pipelines and background execution can evolve in execution-related modules while preserving shell-loop and parser responsibilities.

## Security advantage
Validate syntax before execution and launch external programs with fork/execvp rather than passing a command string through system(). This reduces unintended shell interpretation and makes the parsing boundary explicit.

## Performance benefit
Use local POSIX process/file-descriptor APIs and pipe()/dup2() rather than network calls between services, avoiding unnecessary intermediary services and serialization overhead.

## Important implementation rule
A standalone foreground built-in such as cd must run in the shell process so that directory changes persist. Built-ins inside pipelines may run in child processes.

## API interpretation
This is a C program with module-level interfaces, not a web/REST API. Function declarations under include/ define interfaces between the shell loop, parser, built-in handler and executor.
