
#define _POSIX_C_SOURCE 200809L

#include "shell.h"

#include "builtins.h"
#include "executor.h"
#include "parser.h"

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int sigchld_pipe[2] = {-1, -1};
static volatile sig_atomic_t termination_requested = 0;
static volatile sig_atomic_t sigchld_write_fd = -1;

/**
 * @brief Notify the shell that a child process changed state.
 *
 * Only async-signal-safe operations are used here.
 */
static void handle_sigchld(int signal_number) {
    (void)signal_number;

    int saved_errno = errno;
    int fd = (int)sigchld_write_fd;

    if (fd >= 0) {
        const char notification = 'C';
        (void)write(fd, &notification, sizeof(notification));
    }

    errno = saved_errno;
}

/**
 * @brief Request controlled shell termination.
 */
static void handle_sigterm(int signal_number) {
    (void)signal_number;

    int saved_errno = errno;
    termination_requested = 1;
    executor_termination_requested = 1;

    int fd = (int)sigchld_write_fd;

    if (fd >= 0) {
        const char notification = 'T';
        (void)write(fd, &notification, sizeof(notification));
    }

    errno = saved_errno;
}

/**
 * @brief Set non-blocking and close-on-exec flags on a descriptor.
 */
static int configure_pipe_fd(int fd) {
    int flags = fcntl(fd, F_GETFL);

    if (flags < 0 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        return -1;
    }

    flags = fcntl(fd, F_GETFD);

    if (flags < 0 || fcntl(fd, F_SETFD, flags | FD_CLOEXEC) < 0) {
        return -1;
    }

    return 0;
}

/**
 * @brief Initialize child notifications and signal handlers.
 */
static int initialize_child_notifications(
    struct sigaction *old_action,
    struct sigaction *old_term_action) {

    if (pipe(sigchld_pipe) < 0) {
        return -1;
    }

    if (configure_pipe_fd(sigchld_pipe[0]) < 0 ||
        configure_pipe_fd(sigchld_pipe[1]) < 0) {

        int saved_errno = errno;

        close(sigchld_pipe[0]);
        close(sigchld_pipe[1]);

        sigchld_pipe[0] = -1;
        sigchld_pipe[1] = -1;

        errno = saved_errno;
        return -1;
    }

    struct sigaction action;
    memset(&action, 0, sizeof(action));

    action.sa_handler = handle_sigchld;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigchld_write_fd = (sig_atomic_t)sigchld_pipe[1];

    if (sigaction(SIGCHLD, &action, old_action) < 0) {
        int saved_errno = errno;

        sigchld_write_fd = -1;

        close(sigchld_pipe[0]);
        close(sigchld_pipe[1]);

        sigchld_pipe[0] = -1;
        sigchld_pipe[1] = -1;

        errno = saved_errno;
        return -1;
    }

    struct sigaction term_action;
    memset(&term_action, 0, sizeof(term_action));

    term_action.sa_handler = handle_sigterm;
    sigemptyset(&term_action.sa_mask);
    term_action.sa_flags = 0;

    if (sigaction(SIGTERM, &term_action, old_term_action) < 0) {
        int saved_errno = errno;

        sigchld_write_fd = -1;
        (void)sigaction(SIGCHLD, old_action, NULL);

        close(sigchld_pipe[0]);
        close(sigchld_pipe[1]);

        sigchld_pipe[0] = -1;
        sigchld_pipe[1] = -1;

        errno = saved_errno;
        return -1;
    }

    return 0;
}

/**
 * @brief Restore previous signal handlers and close notification descriptors.
 */
static void cleanup_child_notifications(
    const struct sigaction *old_action,
    const struct sigaction *old_term_action) {

    sigchld_write_fd = -1;

    (void)sigaction(SIGCHLD, old_action, NULL);
    (void)sigaction(SIGTERM, old_term_action, NULL);

    if (sigchld_pipe[0] >= 0) {
        close(sigchld_pipe[0]);
        sigchld_pipe[0] = -1;
    }

    if (sigchld_pipe[1] >= 0) {
        close(sigchld_pipe[1]);
        sigchld_pipe[1] = -1;
    }
}

/**
 * @brief Display the shell prompt.
 */
static void print_prompt(void) {
    fputs(SHELL_DEFAULT_PROMPT, stderr);
    fflush(stderr);
}

/**
 * @brief Drain pending child notifications and reap terminated children.
 */
static void process_child_notifications(void) {
    char buffer[128];

    for (;;) {
        ssize_t count = read(sigchld_pipe[0], buffer, sizeof(buffer));

        if (count > 0) {
            continue;
        }

        if (count < 0 && errno == EINTR) {
            continue;
        }

        break;
    }

    reap_background();
}

/**
 * @brief Read one input line while monitoring child notifications.
 *
 * @return 1 for a complete line, 0 for EOF, -1 for an input error,
 *         or -2 when SIGTERM requests termination.
 */
static int read_command_line(char **line, size_t *capacity) {
    size_t length = 0U;

    if (*line == NULL || *capacity == 0U) {
        *capacity = 128U;
        *line = malloc(*capacity);

        if (*line == NULL) {
            return -1;
        }
    }

    for (;;) {
        if (termination_requested) {
            return -2;
        }

        struct pollfd descriptors[2];

        descriptors[0].fd = STDIN_FILENO;
        descriptors[0].events = POLLIN;
        descriptors[0].revents = 0;

        descriptors[1].fd = sigchld_pipe[0];
        descriptors[1].events = POLLIN;
        descriptors[1].revents = 0;

        int result = poll(descriptors, 2U, -1);

        if (result < 0) {
            if (errno == EINTR) {
                continue;
            }

            return -1;
        }

        if (termination_requested) {
            return -2;
        }

        if (descriptors[1].revents & (POLLIN | POLLERR | POLLHUP)) {
            process_child_notifications();
        }

        if (termination_requested) {
            return -2;
        }

        if (descriptors[0].revents & (POLLERR | POLLNVAL)) {
            errno = EIO;
            return -1;
        }

        if (descriptors[0].revents & (POLLIN | POLLHUP)) {
            char character;
            ssize_t count = read(STDIN_FILENO, &character, 1U);

            if (count == 0) {
                if (length == 0U) {
                    return 0;
                }

                break;
            }

            if (count < 0) {
                if (errno == EINTR || errno == EAGAIN) {
                    continue;
                }

                return -1;
            }

            if (character == '\n') {
                break;
            }

            if (length + 1U >= *capacity) {
                size_t new_capacity = *capacity * 2U;

                if (new_capacity <= *capacity) {
                    errno = ENOMEM;
                    return -1;
                }

                char *new_line = realloc(*line, new_capacity);

                if (new_line == NULL) {
                    return -1;
                }

                *line = new_line;
                *capacity = new_capacity;
            }

            (*line)[length++] = character;
        }
    }

    if (termination_requested) {
        return -2;
    }

    while (length > 0U && (*line)[length - 1U] == '\r') {
        --length;
    }

    (*line)[length] = '\0';

    return 1;
}

/**
 * @brief Interactive shell Read-Eval-Print Loop.
 */
int shell_loop(void) {
    char *line = NULL;
    size_t capacity = 0U;
    int running = 1;
    int exit_status = 0;

    struct sigaction old_action;
    struct sigaction old_term_action;

    termination_requested = 0;
    executor_termination_requested = 0;

    if (initialize_child_notifications(
            &old_action, &old_term_action) < 0) {

        fprintf(stderr,
                "shell: cannot initialize child notifications: %s\n",
                strerror(errno));

        free(line);
        return 1;
    }

    while (running) {
        if (termination_requested) {
            exit_status = 128 + SIGTERM;
            break;
        }

        reap_background();
        print_prompt();

        errno = 0;
        int read_result = read_command_line(&line, &capacity);

        if (read_result == -2 || termination_requested) {
            exit_status = 128 + SIGTERM;
            break;
        }

        if (read_result == 0) {
            fputc('\n', stderr);
            break;
        }

        if (read_result < 0) {
            if (errno == EINTR) {
                clearerr(stdin);
                continue;
            }

            fprintf(stderr, "shell: input error: %s\n", strerror(errno));
            exit_status = 1;
            break;
        }

        struct Pipeline pipeline;
        char error[256];

        if (parse_line(line, &pipeline, error, sizeof(error)) != 0) {
            fprintf(stderr, "shell: syntax error: %s\n", error);
            continue;
        }

        if (pipeline.count == 0U) {
            free_pipeline(&pipeline);
            continue;
        }

        if (termination_requested) {
            free_pipeline(&pipeline);
            exit_status = 128 + SIGTERM;
            break;
        }

        int result = execute_pipeline(&pipeline);

        if (result == SHELL_STATUS_EXIT) {
            running = 0;
        }

        free_pipeline(&pipeline);
    }

    free(line);

    cleanup_background_jobs();

    cleanup_child_notifications(&old_action, &old_term_action);

    return exit_status;
}
