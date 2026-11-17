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

## Final release demo ? Issue #18

- **Video:** [`docs/demos/se-compressed.mp4`](../docs/demos/se-compressed.mp4), supplied for final submission; duration 1:58.63.
- **Release:** `v1.0-final` is published on `main` at the release commit documented in the project release notes.
- **Existing evidence:** [`screenshots/final_ci_latest_run.png`](screenshots/final_ci_latest_run.png) and [`screenshots/final_release_v1.0-final.png`](screenshots/final_release_v1.0-final.png).
