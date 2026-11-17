# Final Demo Script — 2:00

This is the command sequence recorded from the released application in a Linux pseudo-terminal. The matching neural narration is documented in [`../docs/demos/Final_Demo_Narration.md`](../docs/demos/Final_Demo_Narration.md); the raw recording is [`../docs/demos/final_demo.cast`](../docs/demos/final_demo.cast).

| Time | Commands / action | Observed behavior |
|---|---|---|
| 00:00–00:09 | Launch `./simple-shell` | Interactive `simple-shell$` prompt appears. |
| 00:09–00:26 | `help`; `pwd`; `echo "quoted input keeps these words together"`; `cd /tmp`; `pwd` | Built-ins run, quoted words stay together, and the directory changes. |
| 00:26–00:39 | `echo "first line: alpha" > file`; `echo "second line: beta" >> file`; `cat < file` | Temporary file is created, appended, and read back with both lines. |
| 00:39–00:58 | `echo "red blue red" \| tr ' ' '\\n' \| sort \| uniq -c` | Built-in producer runs in a four-stage pipeline; output counts one blue and two red entries. |
| 00:58–01:14 | `sleep 2 &`; `echo "prompt returned immediately while sleep runs"` | Prompt returns without waiting; the follow-up runs and the background child exits successfully. |
| 01:14–01:30 | `echo "unterminated`; `echo "The shell recovered cleanly"` | Parser reports an unterminated quote and continues accepting commands. |
| 01:30–01:57 | `make test` | Parser, executor, shell integration, and termination suites pass; integration reports 70 passed, 0 failed. |
| 01:57–02:00 | `exit` | Clean close with `v1.0-final` and freeze status. |

## Validation before recording

- Source: published tag `v1.0-final`, peeled commit `859b43ea8847b9abff28b9ea2c537b4c593deb5e`.
- Build: `make` completed with exit code 0.
- Regression: `make test` completed with exit code 0; 164 checks passed and none failed.
- Video: [`../docs/demos/final_demo.mp4`](../docs/demos/final_demo.mp4), exactly 2:00, H.264/AAC at 1280×720 and 15 fps.
- Voice: natural neural English generated using Microsoft Edge TTS, voice `en-US-JennyNeural`; synchronized captions are in [`../docs/demos/final_demo.srt`](../docs/demos/final_demo.srt).
