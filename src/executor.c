
#define _POSIX_C_SOURCE 200809L

#include "executor.h"
#include "builtins.h"

#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MAX_BACKGROUND_PIDS 256U

static pid_t background_pids[MAX_BACKGROUND_PIDS];
static size_t background_pid_count = 0U;

/**
 * @brief Wait for a specific child process, retrying if interrupted.
 *
 * @param pid Process ID to wait for.
 * @param status Optional pointer to receive the child exit status.
 * @return 0 on success, -1 on failure.
 */
static int wait_for_pid(pid_t pid, int *status) {
    for (;;) {
        pid_t result = waitpid(pid, status, 0);

        if (result == pid) {
            return 0;
        }

        if (result < 0 && errno == EINTR) {
            continue;
        }

        return -1;
    }
}

/**
 * @brief Reap all child processes that have already been spawned.
 *
 * @param pids Array of child process IDs.
 * @param count Number of child processes.
 */
static void reap_spawned_children(const pid_t *pids, size_t count) {
    struct timespec delay = {0, 10000000L};
    int remaining[MAX_PIPE_SEGMENTS] = {0};

    /* Mark all spawned children as requiring cleanup. */
    for (size_t i = 0U; i < count; ++i) {
        remaining[i] = 1;
        (void)kill(pids[i], SIGTERM);
    }

    /* Allow up to one second for graceful termination. */
    for (int attempt = 0; attempt < 100; ++attempt) {
        int active = 0;

        for (size_t i = 0U; i < count; ++i) {
            if (!remaining[i]) {
                continue;
            }

            int status;
            pid_t result = waitpid(pids[i], &status, WNOHANG);

            if (result == pids[i] ||
                (result < 0 && errno == ECHILD)) {
                remaining[i] = 0;
            } else {
                active = 1;
            }
        }

        if (!active) {
            return;
        }

        (void)nanosleep(&delay, NULL);
    }

    /* Force termination of children that did not exit. */
    for (size_t i = 0U; i < count; ++i) {
        if (remaining[i]) {
            (void)kill(pids[i], SIGKILL);
        }
    }

    /* Reap all remaining children. */
    for (size_t i = 0U; i < count; ++i) {
        if (!remaining[i]) {
            continue;
        }

        int status;
        pid_t result;

        do {
            result = waitpid(pids[i], &status, 0);
        } while (result < 0 && errno == EINTR);

        if (result < 0 && errno != ECHILD) {
            fprintf(stderr, "shell: waitpid(%ld): %s\n",
                    (long)pids[i], strerror(errno));
        }
    }
}

/**
 * @brief Configure file descriptor redirections for input (<) and output (>, >>).
 *
 * @param cmd The command whose redirection parameters should be applied.
 * @return 0 on success, -1 on failure.
 */
static int apply_redirection(const struct Command *cmd) {
    if (cmd->input_file != NULL) {
        int fd = open(cmd->input_file, O_RDONLY);

        if (fd < 0) {
            fprintf(stderr, "shell: cannot open %s: %s\n",
                    cmd->input_file, strerror(errno));
            return -1;
        }

        if (dup2(fd, STDIN_FILENO) < 0) {
            fprintf(stderr, "shell: dup2: %s\n", strerror(errno));
            close(fd);
            return -1;
        }

        close(fd);
    }

    if (cmd->output_file != NULL) {
        int flags = O_WRONLY | O_CREAT |
                    (cmd->append_output ? O_APPEND : O_TRUNC);

        int fd = open(cmd->output_file, flags, 0666);

        if (fd < 0) {
            fprintf(stderr, "shell: cannot open %s: %s\n",
                    cmd->output_file, strerror(errno));
            return -1;
        }

        if (dup2(fd, STDOUT_FILENO) < 0) {
            fprintf(stderr, "shell: dup2: %s\n", strerror(errno));
            close(fd);
            return -1;
        }

        close(fd);
    }

    return 0;
}

/**
 * @brief Execute a built-in command in the parent process.
 *
 * Standard descriptors are backed up and restored when redirection
 * is requested.
 *
 * @param cmd The built-in command.
 * @return Command status.
 */
static int run_single_parent_builtin(const struct Command *cmd) {
    int saved_stdin = -1;
    int saved_stdout = -1;

    if (cmd->input_file != NULL) {
        saved_stdin = dup(STDIN_FILENO);

        if (saved_stdin < 0) {
            fprintf(stderr, "shell: dup stdin: %s\n", strerror(errno));
            return 1;
        }
    }

    if (cmd->output_file != NULL) {
        saved_stdout = dup(STDOUT_FILENO);

        if (saved_stdout < 0) {
            fprintf(stderr, "shell: dup stdout: %s\n", strerror(errno));

            if (saved_stdin >= 0) {
                close(saved_stdin);
            }

            return 1;
        }
    }

    if (apply_redirection(cmd) != 0) {
        if (saved_stdin >= 0) {
            (void)dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdin);
        }

        if (saved_stdout >= 0) {
            (void)dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }

        return 1;
    }

    int result = run_builtin(cmd, 0);

    /* Flush redirected output before restoring stdout. */
    (void)fflush(stdout);

    if (saved_stdin >= 0) {
        (void)dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }

    if (saved_stdout >= 0) {
        (void)dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }

    return (result == SHELL_STATUS_EXIT) ? SHELL_STATUS_EXIT : result;
}

/**
 * @brief Initialize a child process and execute its command.
 *
 * @param cmd Command to execute.
 * @param in_fd Input descriptor.
 * @param out_fd Output descriptor.
 */
static void child_exec(const struct Command *cmd, int in_fd, int out_fd) {
    if (cmd == NULL || cmd->argv[0] == NULL) {
        _exit(0);
    }

    if (in_fd != STDIN_FILENO) {
        if (dup2(in_fd, STDIN_FILENO) < 0) {
            fprintf(stderr, "shell: dup2 stdin: %s\n", strerror(errno));
            _exit(126);
        }

        close(in_fd);
    }

    if (out_fd != STDOUT_FILENO) {
        if (dup2(out_fd, STDOUT_FILENO) < 0) {
            fprintf(stderr, "shell: dup2 stdout: %s\n", strerror(errno));
            _exit(126);
        }

        close(out_fd);
    }

    if (apply_redirection(cmd) != 0) {
        _exit(1);
    }

    if (is_builtin(cmd->argv[0])) {
        int rc = run_builtin(cmd, 1);

        (void)fflush(NULL);
        _exit(rc == SHELL_STATUS_EXIT ? 0 : rc);
    }

    execvp(cmd->argv[0], cmd->argv);

    int exec_errno = errno;

    fprintf(stderr, "shell: %s: %s\n",
            cmd->argv[0], strerror(exec_errno));

    _exit(exec_errno == ENOENT ? 127 : 126);
}

static void remove_background_pid(pid_t pid)
{
    for (size_t i = 0U; i < background_pid_count; ++i) {
        if (background_pids[i] == pid) {
            for (size_t j = i; j + 1U < background_pid_count; ++j) {
                background_pids[j] = background_pids[j + 1U];
            }

            --background_pid_count;
            return;
        }
    }
}

/**
 * @brief Spawn and synchronize child processes for commands and pipelines.
 *
 * @param pipeline Parsed pipeline.
 * @return Exit status of the rightmost command, or 0 for background execution.
 */
static int execute_children(const struct Pipeline *pipeline) {
    pid_t pids[MAX_PIPE_SEGMENTS];
    size_t pid_count = 0U;

    /*
     * Reap completed background children before checking capacity.
     * Reserve enough tracking slots before starting a background pipeline.
     */
    if (pipeline->background) {
        reap_background();

        if (pipeline->count >
            MAX_BACKGROUND_PIDS - background_pid_count) {
            fprintf(stderr, "shell: too many background processes\n");
            return 1;
        }
    }

    int previous_read = STDIN_FILENO;

    for (size_t i = 0U; i < pipeline->count; ++i) {
        int pipe_fd[2] = {-1, -1};

        if (i + 1U < pipeline->count) {
            if (pipe(pipe_fd) != 0) {
                fprintf(stderr, "shell: pipe: %s\n", strerror(errno));

                if (previous_read != STDIN_FILENO) {
                    close(previous_read);
                }

                reap_spawned_children(pids, pid_count);
                return 1;
            }
        }

        pid_t pid = fork();

        if (pid < 0) {
            fprintf(stderr, "shell: fork: %s\n", strerror(errno));

            if (pipe_fd[0] >= 0) {
                close(pipe_fd[0]);
            }

            if (pipe_fd[1] >= 0) {
                close(pipe_fd[1]);
            }

            if (previous_read != STDIN_FILENO) {
                close(previous_read);
            }

            reap_spawned_children(pids, pid_count);
            return 1;
        }

        if (pid == 0) {
            int in_fd = previous_read;
            int out_fd = (i + 1U < pipeline->count)
                             ? pipe_fd[1]
                             : STDOUT_FILENO;

            if (pipe_fd[0] >= 0) {
                close(pipe_fd[0]);
            }

            child_exec(&pipeline->commands[i], in_fd, out_fd);
            _exit(126);
        }

        pids[pid_count++] = pid;

        if (previous_read != STDIN_FILENO) {
            close(previous_read);
        }

        if (pipe_fd[1] >= 0) {
            close(pipe_fd[1]);
        }

        previous_read = (i + 1U < pipeline->count)
                            ? pipe_fd[0]
                            : STDIN_FILENO;
    }

    if (previous_read != STDIN_FILENO) {
        close(previous_read);
    }

    /*
     * Register every process in the background pipeline only after
     * successful creation of all its stages.
     */
    if (pipeline->background) {
        for (size_t i = 0U; i < pid_count; ++i) {
            background_pids[background_pid_count++] = pids[i];
        }

        printf("[background pid %ld]\n", (long)pids[pid_count - 1U]);
        (void)fflush(stdout);
        return 0;
    }

    int last_status = 0;
    int wait_failed = 0;
    size_t remaining = pid_count;

    while (remaining > 0U) {
        int status = 0;
        pid_t completed = waitpid(-1, &status, 0);

        if (completed < 0) {
            if (errno == EINTR) {
                continue;
            }

            fprintf(stderr, "shell: waitpid: %s\n", strerror(errno));
            wait_failed = 1;
            break;
        }

        size_t stage = 0U;

        while (stage < pid_count && pids[stage] != completed) {
            ++stage;
        }

        if (stage == pid_count) {
            remove_background_pid(completed);

            if (WIFEXITED(status)) {
                fprintf(stderr, "[background pid %ld exited %d]\n",
                        (long)completed, WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                fprintf(stderr,
                        "[background pid %ld terminated by signal %d]\n",
                        (long)completed, WTERMSIG(status));
            }

            continue;
        }

        --remaining;

        if (stage + 1U == pid_count) {
            if (WIFEXITED(status)) {
                last_status = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                last_status = 128 + WTERMSIG(status);
            }
        }
    }

    return wait_failed ? 1 : last_status;
}
/**
 * @brief Dispatch execution of a parsed pipeline.
 *
 * @param pipeline Parsed pipeline.
 * @return Command execution status.
 */
int execute_pipeline(const struct Pipeline *pipeline) {
    if (pipeline == NULL || pipeline->count == 0U) {
        return 0;
    }

    if (pipeline->count == 1U &&
        pipeline->commands[0].argv[0] != NULL &&
        is_builtin(pipeline->commands[0].argv[0]) &&
        !pipeline->background) {
        return run_single_parent_builtin(&pipeline->commands[0]);
    }

    return execute_children(pipeline);
}

/**
 * @brief Reap terminated asynchronous background child processes.
 */
void reap_background(void) {
    for (;;) {
        int status = 0;
        pid_t pid = waitpid(-1, &status, WNOHANG);

        if (pid > 0) {
            remove_background_pid(pid);
            if (WIFEXITED(status)) {
                fprintf(stderr, "[background pid %ld exited %d]\n",
                        (long)pid, WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                fprintf(stderr,
                        "[background pid %ld terminated by signal %d]\n",
                        (long)pid, WTERMSIG(status));
            }

            continue;
        }

        if (pid < 0 && errno == EINTR) {
            continue;
        }

        if (pid < 0 && errno != ECHILD) {
            fprintf(stderr, "shell: waitpid: %s\n", strerror(errno));
        }

        break;
    }
}


void cleanup_background_jobs(void) {
    struct timespec delay = {0, 10000000L};

    /* Reap children that have already exited. */
    reap_background();

    /* First request graceful termination. */
    for (size_t i = 0U; i < background_pid_count; ++i) {
        (void)kill(background_pids[i], SIGTERM);
    }

    /*
     * Allow up to one second for background processes
     * to terminate gracefully.
     */
    for (int attempt = 0; attempt < 100; ++attempt) {
        size_t i = 0U;

        while (i < background_pid_count) {
            pid_t pid = background_pids[i];
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG);

            if (result == pid || (result < 0 && errno == ECHILD)) {
                remove_background_pid(pid);
                continue;
            }

            if (result < 0 && errno != EINTR) {
                fprintf(stderr, "shell: waitpid(%ld): %s\n",
                        (long)pid, strerror(errno));
            }

            ++i;
        }

        if (background_pid_count == 0U) {
            return;
        }

        (void)nanosleep(&delay, NULL);
    }

    /* Force termination of any processes still running. */
    for (size_t i = 0U; i < background_pid_count; ++i) {
        (void)kill(background_pids[i], SIGKILL);
    }

    /* Reap every remaining tracked child. */
    while (background_pid_count > 0U) {
        pid_t pid = background_pids[0];
        int status;
        pid_t result;

        do {
            result = waitpid(pid, &status, 0);
        } while (result < 0 && errno == EINTR);
        if (result == pid || (result < 0 && errno == ECHILD)) {
            remove_background_pid(pid);
        } else {
            fprintf(stderr, "shell: waitpid(%ld): %s\n",
                    (long)pid, strerror(errno));
            break;
        }
    }
}