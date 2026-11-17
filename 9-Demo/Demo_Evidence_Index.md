# Demo Evidence Index

| Required item | Status | Evidence |
|---|---|---|
| Sprint 1 one-minute working-product demo | Recording committed | [Sprint 1 Demo Documentation & Video](../docs/demos/README.md) (`docs/demos/simple-shell-sprint1-demo.mp4`, 58.1 s) |
| Sprint 1 output screenshots | Captured | Frames extracted from the committed recording at 4 s, 15 s, 30 s, 44 s, and 57 s in [`screenshots/`](screenshots/) |
| Sprint 2/final two-minute demo | Recorded and committed | [`final_demo.mp4`](../docs/demos/final_demo.mp4), 2:00; raw PTY recording: [`final_demo.cast`](../docs/demos/final_demo.cast) |
| Final output/test screenshots | Captured | [`screenshots/final_ci_latest_run.png`](screenshots/final_ci_latest_run.png) and [`screenshots/final_release_v1.0-final.png`](screenshots/final_release_v1.0-final.png) |

## Sprint 1 output frames

| Time | Evidence |
|---|---|
| 00:04 | [`sprint1_demo_t4s.png`](screenshots/sprint1_demo_t4s.png) — build and launch |
| 00:15 | [`sprint1_demo_t15s.png`](screenshots/sprint1_demo_t15s.png) — prompt, `pwd`, `cd`, `pwd` |
| 00:30 | [`sprint1_demo_t30s.png`](screenshots/sprint1_demo_t30s.png) — quoted input and external commands |
| 00:44 | [`sprint1_demo_t44s.png`](screenshots/sprint1_demo_t44s.png) — error diagnostic and recovery |
| 00:57 | [`sprint1_demo_t57s.png`](screenshots/sprint1_demo_t57s.png) — `help` and clean exit |

Videos/screenshots are not fabricated. The Sprint 1 frames are direct extractions from the committed
MP4 at the stated timestamps; the CI and release captures are direct screenshots of the live GitHub
pages after the `v1.0-final` release.

## Final release demo — Issue #18

- **Video:** [`docs/demos/final_demo.mp4`](../docs/demos/final_demo.mp4) — 2:00, H.264/AAC, 1280×720 at 15 fps.
- **Thumbnail:** [`docs/demos/final_demo_thumbnail.png`](../docs/demos/final_demo_thumbnail.png).
- **Genuine terminal capture:** [`docs/demos/final_demo.cast`](../docs/demos/final_demo.cast), an Asciicast v2 recording of the actual tagged SimpleShell process running in a Linux pseudo-terminal. The video renders these captured terminal events; it does not synthesize command output.
- **Narration:** Natural English neural speech generated with Microsoft Edge TTS (`en-US-JennyNeural`), burned-in subtitles in [`final_demo.srt`](../docs/demos/final_demo.srt), and the full script in [`Final_Demo_Narration.md`](../docs/demos/Final_Demo_Narration.md).
- **Tested application source:** published `v1.0-final` tag, peeled commit `859b43ea8847b9abff28b9ea2c537b4c593deb5e`.

| Video time | Demonstration | Observed result |
|---|---|---|
| 00:00–00:09 | Launch and prompt | Tagged application launches and displays `simple-shell$`. |
| 00:09–00:26 | Built-ins and quoted input | `help`, `pwd`, `cd`, and quoted `echo` work; the quoted phrase is preserved. |
| 00:26–00:39 | `<`, `>`, and `>>` | A temporary file is created, appended, then read back with both expected lines. |
| 00:39–00:58 | Four-stage pipeline | Built-in `echo` feeds `tr | sort | uniq -c`; output is `1 blue` and `2 red`. |
| 00:58–01:14 | Background execution | `sleep 2 &` returns the prompt immediately; a follow-up command runs and the child exits 0. |
| 01:14–01:30 | Malformed syntax and recovery | Unterminated quoted input produces a syntax diagnostic; a valid `echo` then succeeds. |
| 01:30–01:57 | Automated tests | `make test` completes with zero failures: parser and executor suites pass, shell integration is 70/70, and both termination checks pass. |
| 01:57–02:00 | Release closeout | Displays `v1.0-final` and the development freeze. |

### Build and test evidence

Executed against a clean temporary export of the release tag in Ubuntu WSL:

```text
make       # exit 0; strict GCC C11 build
make test  # exit 0; parser 63/63, executor 29/29, integration 70/70, termination 2/2
           # total 164 passed, 0 failed
```

The live `make test` run and its real output are also visible in the terminal recording. The `v1.0-final` release tag remains unchanged at commit `859b43e`; subsequent `main` commits and this evidence delivery are documentation/media only, preserving the software development freeze.
