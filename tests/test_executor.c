#define _POSIX_C_SOURCE 200809L

#include "executor.h"
#include "parser.h"

#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static int failures = 0;

static void check(int condition, const char *name) {
    printf("%s: %s\n", condition ? "PASS" : "FAIL", name);

    if (!condition) {
        ++failures;
    }
}

/*
 * Capture stdout while testing execute_pipeline's status directly:
 * the REPL intentionally returns zero at EOF rather than the last
 * command status.
 */
static int run(const char *line, char *output, size_t size) {
    struct Pipeline pipeline;
    char error[256];

    if (parse_line(line, &pipeline, error, sizeof(error)) != 0) {
        fprintf(stderr, "parse failed: %s\n", error);
        exit(2);
    }

    (void)fflush(NULL);

    FILE *capture = tmpfile();
    int saved = dup(STDOUT_FILENO);

    if (capture == NULL || saved < 0 ||
        dup2(fileno(capture), STDOUT_FILENO) < 0) {
        perror("capture stdout");
        exit(2);
    }

    int result = execute_pipeline(&pipeline);

    struct stat expected_stdout;
    struct stat actual_stdout;
    if (fstat(fileno(capture), &expected_stdout) != 0 ||
        fstat(STDOUT_FILENO, &actual_stdout) != 0 ||
        expected_stdout.st_dev != actual_stdout.st_dev ||
        expected_stdout.st_ino != actual_stdout.st_ino) {
        fprintf(stderr, "FAIL: executor did not restore stdout\n");
        exit(2);
    }

    (void)fflush(stdout);

    if (dup2(saved, STDOUT_FILENO) < 0) {
        perror("restore stdout");
        exit(2);
    }

    close(saved);
    rewind(capture);

    size_t bytes = fread(output, 1U, size - 1U, capture);
    output[bytes] = '\0';

    fclose(capture);
    free_pipeline(&pipeline);

    return result;
}

/*
 * Count open file descriptors using /proc/self/fd.
 * The descriptor used internally by opendir() is excluded.
 */
static int open_descriptors(void) {
    DIR *dir = opendir("/proc/self/fd");

    if (dir == NULL) {
        return -1;
    }

    int count = 0;
    struct dirent *entry;
    int read_error = 0;

    errno = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 &&
            strcmp(entry->d_name, "..") != 0) {
            ++count;
        }

        errno = 0;
    }

    if (errno != 0) {
        read_error = errno;
    }

    if (closedir(dir) != 0 || read_error != 0) {
        return -1;
    }

    /* Exclude the descriptor opened by opendir itself. */
    return count - 1;
}

static pid_t background(const char *command) {
    char output[2048];
    long pid = -1;
    if (run(command, output, sizeof(output)) != 0 ||
        sscanf(output, "[background pid %ld]", &pid) != 1 || pid <= 0) {
        fprintf(stderr, "could not start background fixture\n");
        exit(2);
    }
    /* Do not signal a Valgrind fork child before it has exec'd the fixture. */
    char proc_path[80];
    (void)snprintf(proc_path, sizeof(proc_path), "/proc/%ld/exe", pid);
    for (int attempt = 0; attempt < 1000; ++attempt) {
        char executable[1024];
        ssize_t length = readlink(proc_path, executable, sizeof(executable) - 1U);
        if (length > 0) {
            executable[(size_t)length] = '\0';
            const char *base = strrchr(executable, '/');
            if (base != NULL && strcmp(base + 1, "sleep") == 0) {
                return (pid_t)pid;
            }
        }
        const struct timespec delay = {0, 10000000L};
        (void)nanosleep(&delay, NULL);
    }
    (void)kill((pid_t)pid, SIGKILL);
    fprintf(stderr, "background sleep did not reach exec readiness\n");
    exit(2);
}

/* Observe a completed child without consuming its status: the executor must reap it. */
static void await_exit(pid_t pid) {
    siginfo_t info;
    int result;
    do {
        result = waitid(P_PID, (id_t)pid, &info, WEXITED | WNOWAIT);
    } while (result < 0 && errno == EINTR);
    if (result < 0) {
        perror("waitid fixture");
        exit(2);
    }
}

static void check_reaped(pid_t pid, const char *name) {
    int status;
    errno = 0;
    check(waitpid(pid, &status, WNOHANG) == -1 && errno == ECHILD, name);
}

static void test_resource_failure(void) {
    struct rlimit original;
    if (getrlimit(RLIMIT_NOFILE, &original) != 0) {
        perror("getrlimit");
        exit(2);
    }
    struct rlimit limited = original;
    if (limited.rlim_cur > 64U) {
        limited.rlim_cur = 64U;
    }
    if (setrlimit(RLIMIT_NOFILE, &limited) != 0) {
        perror("setrlimit");
        exit(2);
    }
    const int before = open_descriptors();
    struct Pipeline pipeline;
    char error[256];
    if (parse_line("sleep 30 | cat | cat", &pipeline, error, sizeof(error)) != 0) {
        exit(2);
    }
    int descriptors[64];
    size_t count = 0U;
    int fd;
    while ((fd = open("/dev/null", O_RDONLY)) >= 0 && count < 64U) {
        descriptors[count++] = fd;
    }
    if (errno != EMFILE || count < 2U) {
        fprintf(stderr, "could not exhaust descriptor fixture\n");
        exit(2);
    }
    /* Two slots permit the first pipe/fork; only one slot remains for pipe #2. */
    close(descriptors[--count]);
    close(descriptors[--count]);
    const int result = execute_pipeline(&pipeline);
    while (count > 0U) {
        close(descriptors[--count]);
    }
    free_pipeline(&pipeline);
    check(result == 1, "second pipe failure returns failure after spawning a stage");
    check(before >= 0 && open_descriptors() == before,
          "pipe failure closes partial pipeline descriptors");
    int status;
    errno = 0;
    check(waitpid(-1, &status, WNOHANG) == -1 && errno == ECHILD,
          "pipe failure terminates and reaps the already spawned stage");
    if (setrlimit(RLIMIT_NOFILE, &original) != 0) {
        perror("restore descriptor limit");
        exit(2);
    }
    char output[64];
    check(run("echo recovered | cat", output, sizeof(output)) == 0 &&
          strcmp(output, "recovered\n") == 0, "executor recovers after pipe failure");
}

static void test_lifecycle_and_redirection(void) {
    char output[2048];
    char input[] = "/tmp/simple-shell-input-XXXXXX";
    int input_fd = mkstemp(input);
    if (input_fd < 0 || write(input_fd, "sentinel\n", 9U) != 9) {
        perror("input fixture");
        exit(2);
    }
    int saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin < 0 || lseek(input_fd, 0, SEEK_SET) < 0 ||
        dup2(input_fd, STDIN_FILENO) < 0) {
        exit(2);
    }
    close(input_fd);
    char command[1024];
    (void)snprintf(command, sizeof(command), "echo input < %s", "/dev/null");
    check(run(command, output, sizeof(output)) == 0 && strcmp(output, "input\n") == 0,
          "parent builtin accepts input redirection");
    char redirected[] = "/tmp/simple-shell-output-XXXXXX";
    int out_fd = mkstemp(redirected);
    if (out_fd < 0) {
        exit(2);
    }
    close(out_fd);
    (void)snprintf(command, sizeof(command), "echo combined < %s > %s", "/dev/null", redirected);
    int result = run(command, output, sizeof(output));
    FILE *file = fopen(redirected, "r");
    char contents[32] = {0};
    if (file != NULL) {
        (void)fgets(contents, sizeof(contents), file);
        fclose(file);
    }
    check(result == 0 && output[0] == '\0' && strcmp(contents, "combined\n") == 0,
          "parent builtin combines input and output redirection");
    unlink(redirected);
    (void)snprintf(command, sizeof(command), "echo failure < %s > /", "/dev/null");
    check(run(command, output, sizeof(output)) == 1,
          "output failure after input redirect returns failure");
    unlink(input);
    (void)snprintf(command, sizeof(command), "echo failure < %s", input);
    check(run(command, output, sizeof(output)) == 1,
          "parent builtin rejects missing input file");
    char sentinel[10] = {0};
    check(read(STDIN_FILENO, sentinel, 9U) == 9 && strcmp(sentinel, "sentinel\n") == 0,
          "stdin restored after successful and failed builtin redirects");
    if (dup2(saved_stdin, STDIN_FILENO) < 0) {
        exit(2);
    }
    close(saved_stdin);
    check(run("echo restored", output, sizeof(output)) == 0 &&
          strcmp(output, "restored\n") == 0, "stdout restored after builtin redirect failures");

    check(run("sh -c 'kill -TERM $$'", output, sizeof(output)) == 128 + SIGTERM,
          "foreground signal maps to 128 plus signal");
    pid_t first = background("sleep 300 &");
    pid_t second = background("sleep 300 &");
    pid_t third = background("sleep 300 &");
    (void)kill(second, SIGTERM);
    await_exit(second);
    reap_background();
    check_reaped(second, "middle background job is reaped after signal");
    check(kill(first, 0) == 0 && kill(third, 0) == 0,
          "out-of-order reaping preserves other background jobs");
    (void)kill(first, SIGTERM);
    await_exit(first);
    check(run("sleep 0.02", output, sizeof(output)) == 0,
          "foreground command succeeds while background child is signal-terminated");
    check_reaped(first, "foreground wait reaps completed background job");
    cleanup_background_jobs();
    check_reaped(third, "cleanup still tracks and reaps the last background job");
}

int main(void) {
    /* A leaked pipe writer or serial wait must fail rather than hang CI. */
    alarm(120U);

    test_lifecycle_and_redirection();
    test_resource_failure();

    char output[2048];

    check(run("printf 'a\\nb\\n' | wc -l", output, sizeof(output)) == 0 &&
          strcmp(output, "2\n") == 0,
          "external producer and consumer");

    check(run("echo builtin | cat", output, sizeof(output)) == 0 &&
          strcmp(output, "builtin\n") == 0,
          "builtin producer output is flushed");

    check(run("printf ignored | echo builtin", output, sizeof(output)) == 0 &&
          strcmp(output, "builtin\n") == 0,
          "builtin consumer supported");

    check(run("echo left | echo right", output, sizeof(output)) == 0 &&
          strcmp(output, "right\n") == 0,
          "both stages may be builtins");

    check(run("seq 20000 | wc -l", output, sizeof(output)) == 0 &&
          strcmp(output, "20000\n") == 0,
          "concurrent stages transfer more than pipe capacity");

    check(run("yes | head -n 1", output, sizeof(output)) == 0 &&
          strcmp(output, "y\n") == 0,
          "early consumer exit closes unused pipe readers");

    check(run("printf x | sh -c 'exit 7'", output, sizeof(output)) == 7,
          "rightmost stage determines pipeline status");

    check(run("sh -c 'exit 7' | cat", output, sizeof(output)) == 0 &&
          output[0] == '\0',
          "left failure does not override rightmost success");

    check(run("printf x | definitely-not-a-command",
              output, sizeof(output)) == 127,
          "missing consumer returns 127 without hanging producer");

    check(run("definitely-not-a-command | cat",
              output, sizeof(output)) == 0 &&
          output[0] == '\0',
          "missing producer closes writer so consumer sees EOF");

    /* --- Multi-stage pipeline execution and deadlock prevention --- */

    check(run("printf 'a\\nb\\nc\\n' | grep -v b | wc -l",
              output, sizeof(output)) == 0 &&
          strcmp(output, "2\n") == 0,
          "three-stage pipeline external producer, filter, consumer");

    check(run("printf 'gamma\\nalpha\\nbeta\\n' | sort | head -n 2 | wc -l",
              output, sizeof(output)) == 0 &&
          strcmp(output, "2\n") == 0,
          "four-stage pipeline sorts and filters correctly");

    check(run("echo initial | cat | cat", output, sizeof(output)) == 0 &&
          strcmp(output, "initial\n") == 0,
          "three-stage pipeline with builtin producer");

    check(run("printf 'line\\n' | cat | cat | wc -l",
              output, sizeof(output)) == 0 &&
          strcmp(output, "1\n") == 0,
          "multi-stage pipeline with intermediate filters");

    check(run("echo first | cat | echo third",
              output, sizeof(output)) == 0 &&
          strcmp(output, "third\n") == 0,
          "multi-stage pipeline with builtin producer and consumer");

    check(run("seq 50000 | cat | wc -l",
              output, sizeof(output)) == 0 &&
          strcmp(output, "50000\n") == 0,
          "three-stage pipeline deadlock prevention on large data transfer");

    check(run("yes | cat | head -n 1",
              output, sizeof(output)) == 0 &&
          strcmp(output, "y\n") == 0,
          "three-stage early consumer termination closes pipe readers without hang");

    check(run("printf test | cat | sh -c 'exit 42'",
              output, sizeof(output)) == 42,
          "three-stage rightmost stage exit status propagated");

    check(run("printf test | sh -c 'exit 19' | cat",
              output, sizeof(output)) == 0 &&
          output[0] == '\0',
          "three-stage middle stage failure does not override rightmost success");

    /* Maximum pipeline segment execution (32 stages). */
    char max_stage_cmd[1024] = "echo max_stage";

    for (int i = 1; i < MAX_PIPE_SEGMENTS; ++i) {
        strcat(max_stage_cmd, " | cat");
    }

    check(run(max_stage_cmd, output, sizeof(output)) == 0 &&
          strcmp(output, "max_stage\n") == 0,
          "32-stage pipeline executes successfully end-to-end");

    char *cwd = getcwd(NULL, 0);

    if (cwd == NULL) {
        perror("getcwd");
        return 2;
    }

    char *after;
    int rc = run("cd / | cat", output, sizeof(output));
    after = getcwd(NULL, 0);

    check(rc == 0 && after != NULL && strcmp(cwd, after) == 0,
          "pipeline cd does not mutate parent directory");

    free(after);
    free(cwd);

    check(run("exit | cat", output, sizeof(output)) == 0 &&
          output[0] == '\0',
          "pipeline exit does not terminate parent");

    check(run("echo alive | cat", output, sizeof(output)) == 0 &&
          strcmp(output, "alive\n") == 0,
          "parent continues after child exit builtin");

    char marker[] = "/tmp/simple-shell-wait-XXXXXX";
    int marker_fd = mkstemp(marker);

    if (marker_fd < 0) {
        perror("mkstemp");
        return 2;
    }

    close(marker_fd);

    char command[512];

    for (int stage = 0; stage < 2; ++stage) {
        /*
         * A file side effect proves completion without relying on elapsed
         * time or command-substitution pipes keeping the harness blocked.
         */
        (void)snprintf(
            command,
            sizeof(command),
            stage == 0
                ? "sh -c 'sleep 0.1; echo done > %s' | true"
                : "true | sh -c 'sleep 0.1; echo done > %s'",
            marker
        );

        rc = run(command, output, sizeof(output));

        FILE *file = fopen(marker, "r");
        int completed = file != NULL && fgetc(file) == 'd';

        if (file != NULL) {
            fclose(file);
        }

        check(rc == 0 && completed,
              stage == 0
                  ? "waits for producer even when consumer exits first"
                  : "waits for consumer even when producer exits first");

        file = fopen(marker, "w");

        if (file == NULL) {
            perror("truncate marker");
            return 2;
        }

        fclose(file);
    }

    /* Verify parent waits for intermediate stage in multi-stage pipeline. */
    (void)snprintf(
        command,
        sizeof(command),
        "true | sh -c 'sleep 0.1; echo done > %s' | true",
        marker
    );

    rc = run(command, output, sizeof(output));

    FILE *file = fopen(marker, "r");
    int intermediate_completed = file != NULL && fgetc(file) == 'd';

    if (file != NULL) {
        fclose(file);
    }

    check(rc == 0 && intermediate_completed,
          "waits for intermediate stage in three-stage pipeline");

    unlink(marker);

    /* --- File descriptor stress audit --- */

    struct rlimit original;

    if (getrlimit(RLIMIT_NOFILE, &original) != 0) {
        perror("getrlimit");
        return 2;
    }

    struct rlimit limited = original;

    if (limited.rlim_cur > 64U) {
        limited.rlim_cur = 64U;
    }

    if (setrlimit(RLIMIT_NOFILE, &limited) != 0) {
        perror("setrlimit");
        return 2;
    }

    int before = open_descriptors();
    int stress_ok = 1;

    for (int cycle = 0; cycle < 100; ++cycle) {
        if (run("echo x | cat > /dev/null",
                output, sizeof(output)) != 0 ||
            output[0] != '\0') {
            stress_ok = 0;
            break;
        }
    }

    check(before >= 0 && stress_ok &&
          before == open_descriptors(),
          "100 pipelines under 64-descriptor limit leave descriptor count unchanged");

    int stress_4stage_ok = 1;

    for (int cycle = 0; cycle < 50; ++cycle) {
        if (run("echo x | cat | cat | cat > /dev/null",
                output, sizeof(output)) != 0 ||
            output[0] != '\0') {
            stress_4stage_ok = 0;
            break;
        }
    }

    check(before >= 0 && stress_4stage_ok &&
          before == open_descriptors(),
          "50 4-stage pipelines under 64-descriptor limit leave descriptor count unchanged");

    if (setrlimit(RLIMIT_NOFILE, &original) != 0) {
        perror("restore descriptor limit");
        return 2;
    }

    int status;
    errno = 0;

    check(waitpid(-1, &status, WNOHANG) == -1 && errno == ECHILD,
          "all foreground children reaped");

    alarm(0U);

    printf("\nExecutor tests: %d failed\n", failures);

    return failures == 0 ? 0 : 1;
}
