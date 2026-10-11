# Deployment and Environment Setup

## Target
Simple Shell is a local command-line program. The SRS targets Linux with POSIX APIs and a C11-capable GCC toolchain. No server, database or cloud infrastructure is required.

## Prerequisites
Linux or the agreed Linux VM, Git, GCC, GNU Make and Bash.

On Ubuntu, if development tools are missing:

    sudo apt update
    sudo apt install build-essential git

## Clone

    git clone https://github.com/ShrikarDesai07/MiniProject_H1_SimpleShell.git
    cd MiniProject_H1_SimpleShell

For an existing clone, check git status and the current branch before pulling.

## Build and run

    make
    ./simple-shell

Try supported commands such as pwd, echo "hello world", ls, a pipeline, and exit.

## Test

    make test
    make asan-test

Record actual command output, OS/compiler versions and commit SHA in the test report. The current report says validation output is outstanding, so this step is incomplete until it has actually run.

## Cleanup

    make clean

## Runtime/infrastructure
Local Linux process; terminal stdin/stdout/stderr; local files for redirection; POSIX process and file APIs. No service ports or external credentials are required.
