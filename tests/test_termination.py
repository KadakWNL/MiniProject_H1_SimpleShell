"""SIGTERM must interrupt foreground waits and stop background descendants."""
import os
import signal
import subprocess
import tempfile
import time

for background, stubborn in ((False, False), (True, False), (False, True), (True, True)):
    with tempfile.TemporaryDirectory() as directory:
        marker = directory + '/pid'
        shell = subprocess.Popen(['./simple-shell'], stdin=subprocess.PIPE,
                                 stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        if stubborn:
            # Install SIG_IGN before publishing readiness; exec preserves that disposition.
            command = "sh -c 'trap \"\" TERM; echo $$ > " + marker + "; exec sleep 30'"
        else:
            command = "sh -c 'sleep 30 & echo $! > " + marker + "; wait'"
        shell.stdin.write((command + (' &' if background else '') + '\n').encode())
        shell.stdin.flush()
        child = None
        try:
            for _ in range(100):
                if os.path.exists(marker) and os.path.getsize(marker):
                    child = int(open(marker).read())
                    break
                time.sleep(.02)
            assert child, 'command did not start'
            started = time.monotonic()
            shell.send_signal(signal.SIGTERM)
            assert shell.wait(timeout=3) == 143
            if stubborn:
                assert time.monotonic() - started >= .8, "SIGTERM-ignoring child bypassed grace interval"
            for _ in range(100):
                path = f'/proc/{child}/stat'
                if not os.path.exists(path) or open(path).read().split()[2] == 'Z':
                    break
                time.sleep(.02)
            else:
                raise AssertionError('descendant still running')
            print(f'PASS: SIGTERM cleanup, background={background}, ignores_SIGTERM={stubborn}')
        finally:
            shell.kill() if shell.poll() is None else None
            shell.wait()
            shell.stdin.close()
            if child:
                try:
                    os.kill(child, signal.SIGKILL)
                except ProcessLookupError:
                    pass
