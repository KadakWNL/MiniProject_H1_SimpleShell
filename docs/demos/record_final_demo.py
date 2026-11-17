#!/usr/bin/env python3
"""Capture the final demo commands from the real SimpleShell PTY."""

import argparse
import errno
import fcntl
import json
import os
import pty
import select
import struct
import tempfile
import termios
import time


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--shell-dir", required=True, help="clean v1.0-final source/build directory")
    parser.add_argument("--output", required=True, help="output path for the Asciicast v2 recording")
    args = parser.parse_args()

    shell_dir = os.path.abspath(args.shell_dir)
    output_path = os.path.abspath(args.output)
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    data_file = os.path.join(tempfile.mkdtemp(prefix="simple-shell-final-demo-"), "redirect.txt")
    events: list[list[object]] = []

    pid, master = pty.fork()
    if pid == 0:
        os.chdir(shell_dir)
        os.environ["TERM"] = "xterm-256color"
        os.environ["LC_ALL"] = "C.UTF-8"
        os.execv("./simple-shell", ["./simple-shell"])

    fcntl.ioctl(master, termios.TIOCSWINSZ, struct.pack("HHHH", 30, 106, 0, 0))
    started = time.monotonic()
    pending = bytearray()

    def pump(seconds: float = 0.1) -> None:
        deadline = time.monotonic() + seconds
        while time.monotonic() < deadline:
            ready, _, _ = select.select([master], [], [], max(0.0, deadline - time.monotonic()))
            if not ready:
                break
            try:
                chunk = os.read(master, 65536)
            except OSError as exc:
                if exc.errno == errno.EIO:
                    return
                raise
            if not chunk:
                return
            events.append([round(time.monotonic() - started, 3), "o", chunk.decode("utf-8", "replace")])
            pending.extend(chunk)

    def wait_prompt(timeout: float = 30.0) -> None:
        deadline = time.monotonic() + timeout
        marker = b"simple-shell$ "
        while time.monotonic() < deadline:
            if pending.endswith(marker):
                pending.clear()
                return
            pump(min(0.25, deadline - time.monotonic()))
        raise TimeoutError(f"Shell prompt did not return; final output: {bytes(pending[-300:])!r}")

    def sleep_to(seconds: float) -> None:
        while time.monotonic() - started < seconds:
            pump(min(0.25, seconds - (time.monotonic() - started)))

    def command(text: str, at: float, timeout: float = 30.0) -> None:
        sleep_to(at)
        os.write(master, text.encode("utf-8") + b"\n")
        wait_prompt(timeout)

    wait_prompt()
    command("help", 12.0)
    command("pwd", 14.5)
    command('echo "quoted input keeps these words together"', 17.5)
    command("cd /tmp", 21.0)
    command("pwd", 23.0)
    command("cd " + shell_dir, 25.4)
    command(f'echo "first line: alpha" > {data_file}', 26.0)
    command(f'echo "second line: beta" >> {data_file}', 30.0)
    command(f"cat < {data_file}", 35.0)
    command("echo \"red blue red\" | tr ' ' '\\n' | sort | uniq -c", 39.5)
    command("sleep 2 &", 58.3)
    command('echo "prompt returned immediately while sleep runs"', 58.7)
    sleep_to(70.0)
    command('echo "unterminated', 74.5)
    command('echo "The shell recovered cleanly"', 80.2)
    command("make test", 90.7, timeout=60.0)
    sleep_to(118.2)
    os.write(master, b"exit\n")

    status = None
    for _ in range(40):
        pump(0.1)
        finished, raw_status = os.waitpid(pid, os.WNOHANG)
        if finished:
            status = os.waitstatus_to_exitcode(raw_status)
            break
    if status is None:
        os.kill(pid, 9)
        os.waitpid(pid, 0)
        raise RuntimeError("Shell did not exit after the exit command")

    with open(output_path, "w", encoding="utf-8") as recording:
        header = {
            "version": 2,
            "width": 106,
            "height": 30,
            "timestamp": int(time.time()),
            "env": {"TERM": "xterm-256color", "SHELL": "simple-shell"},
        }
        recording.write(json.dumps(header) + "\n")
        for event in events:
            recording.write(json.dumps(event, ensure_ascii=False, separators=(",", ":")) + "\n")

    print(f"PTY recording: {output_path}")
    print(f"Captured terminal events: {len(events)}")
    print(f"Terminal session duration: {events[-1][0]:.2f} seconds")
    print(f"Temporary redirection file: {data_file}")
    print(f"Shell exit status: {status}")


if __name__ == "__main__":
    main()
