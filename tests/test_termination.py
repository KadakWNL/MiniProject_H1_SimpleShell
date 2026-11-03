"""SIGTERM must interrupt foreground waits and stop background descendants."""
import os
import signal
import subprocess
import tempfile
import time

for background in (False, True):
    with tempfile.TemporaryDirectory() as directory:
        marker = directory + '/pid'
        shell = subprocess.Popen(['./simple-shell'], stdin=subprocess.PIPE,
                                 stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        shell.stdin.write(("sh -c 'sleep 30 & echo $! > " + marker +
                           "; wait'" + (' &' if background else '') + '\n').encode())
        shell.stdin.flush()
        child = None
        try:
            for _ in range(100):
                if os.path.exists(marker) and os.path.getsize(marker):
                    child = int(open(marker).read())
                    break
                time.sleep(.02)
            assert child, 'command did not start'
            shell.send_signal(signal.SIGTERM)
            assert shell.wait(timeout=3) == 143
            for _ in range(100):
                path = f'/proc/{child}/stat'
                if not os.path.exists(path) or open(path).read().split()[2] == 'Z':
                    break
                time.sleep(.02)
            else:
                raise AssertionError('descendant still running')
            print('PASS: SIGTERM cleanup, background=' + str(background))
        finally:
            shell.kill() if shell.poll() is None else None
            shell.wait()
            shell.stdin.close()
            if child:
                try:
                    os.kill(child, signal.SIGKILL)
                except ProcessLookupError:
                    pass
