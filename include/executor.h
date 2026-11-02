#ifndef EXECUTOR_H
#define EXECUTOR_H

/**
 * @file executor.h
 * @brief Command execution engine, pipeline dispatch, and process lifecycle management.
 *
 * Coordinates process creation (fork), binary execution (execvp), I/O redirection
 * (dup2), inter-process communication pipelines (pipe), foreground synchronization
 * (waitpid), and asynchronous background process reaping.
 */

#include "parser.h"

/**
 * @brief Execute a parsed command pipeline.
 *
 * For single built-in commands without background execution (&), the command is executed
 * directly within the parent shell process with file redirection preserved and restored.
 * For external commands or multi-command pipelines (|), child processes are spawned,
 * pipes are established, and file descriptors are properly configured.
 *
 * In foreground mode, the parent waits for child termination and captures the exit status.
 * In background mode (&), the parent records the background PID, returns immediately,
 * and allows subsequent commands to execute without delay.
 *
 * @param pipeline Pointer to the parsed Pipeline containing commands, redirections, and background flag.
 * @return Exit status of the final pipeline command, 0 for background jobs, or SHELL_STATUS_EXIT.
 */
int execute_pipeline(const struct Pipeline *pipeline);

/**
 * @brief Non-blocking check to reap finished background processes.
 *
 * Invokes waitpid(-1, &status, WNOHANG) in a loop before displaying each interactive
 * prompt to prevent zombie process accumulation and report asynchronous completion.
 */
void reap_background(void);

/**
 * @brief Terminate and wait for all remaining background child processes.
 */
void cleanup_background_jobs(void);

#endif /* EXECUTOR_H */
