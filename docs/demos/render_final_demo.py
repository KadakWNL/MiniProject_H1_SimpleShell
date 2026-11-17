#!/usr/bin/env python3
"""Render an Asciicast v2 PTY recording and synchronized narration as MP4."""

import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
import textwrap
from pathlib import Path

sys.path.append("/usr/lib/python3/dist-packages")

import imageio_ffmpeg
import pyte
from PIL import Image, ImageDraw, ImageFont


WIDTH, HEIGHT, FPS, DURATION = 1280, 720, 15, 120
BG = (10, 15, 27)
PANEL = (17, 25, 41)
TERMINAL = (5, 9, 16)
FG = (216, 225, 240)
MUTED = (129, 146, 171)
ACCENT = (72, 199, 142)
BLUE = (113, 169, 255)
FONT_MONO = "/mnt/c/Windows/Fonts/cour.ttf"
FONT_UI = "/mnt/c/Windows/Fonts/arial.ttf"


def parse_srt(path: str) -> list[tuple[float, float, str]]:
    raw = Path(path).read_text(encoding="utf-8-sig")
    cues = []
    for block in re.split(r"\r?\n\s*\r?\n", raw.strip()):
        lines = block.splitlines()
        if len(lines) < 3 or "-->" not in lines[1]:
            continue
        start, end = (x.strip() for x in lines[1].split("-->"))
        def seconds(value: str) -> float:
            h, m, tail = value.replace(",", ".").split(":")
            return int(h) * 3600 + int(m) * 60 + float(tail)
        cues.append((seconds(start), seconds(end), " ".join(lines[2:])))
    return cues


def color(value: str, default: tuple[int, int, int]) -> tuple[int, int, int]:
    palette = {
        "black": (0, 0, 0), "red": (205, 49, 49), "green": (13, 188, 121),
        "yellow": (229, 229, 16), "blue": (36, 114, 200), "magenta": (188, 63, 188),
        "cyan": (17, 168, 205), "white": (229, 229, 229),
        "brightblack": (102, 102, 102), "brightred": (241, 76, 76),
        "brightgreen": (35, 209, 139), "brightyellow": (245, 245, 67),
        "brightblue": (59, 142, 234), "brightmagenta": (214, 112, 214),
        "brightcyan": (41, 184, 219), "brightwhite": (255, 255, 255),
    }
    if value in palette:
        return palette[value]
    if re.fullmatch(r"#[0-9a-fA-F]{6}", value):
        return tuple(int(value[i:i + 2], 16) for i in (1, 3, 5))
    return default


def section_at(t: float) -> tuple[str, str]:
    sections = [
        (9.63, "01", "LAUNCH & PROMPT"),
        (25.85, "02", "BUILT-INS & QUOTED INPUT"),
        (39.43, "03", "INPUT, OVERWRITE & APPEND"),
        (58.21, "04", "FOUR-STAGE PIPELINE"),
        (74.49, "05", "BACKGROUND EXECUTION"),
        (90.68, "06", "ERROR RECOVERY"),
        (117.05, "07", "AUTOMATED TESTS"),
        (120.0, "08", "V1.0-FINAL · DEVELOPMENT FROZEN"),
    ]
    number, label = "00", "SIMPLE SHELL · FINAL DEMO"
    for end, n, name in sections:
        number, label = n, name
        if t < end:
            break
    return number, label


def make_frame(screen: pyte.Screen, t: float, cue: str, mono: ImageFont.FreeTypeFont,
               ui: ImageFont.FreeTypeFont, small: ImageFont.FreeTypeFont) -> Image.Image:
    image = Image.new("RGB", (WIDTH, HEIGHT), BG)
    draw = ImageDraw.Draw(image)
    number, label = section_at(t)

    draw.rectangle((0, 0, WIDTH, 64), fill=(15, 22, 36))
    draw.text((42, 18), "SIMPLESHELL", font=ui, fill=FG)
    draw.text((205, 22), "FINAL RELEASE DEMONSTRATION", font=small, fill=MUTED)
    draw.text((WIDTH - 258, 22), "v1.0-final  ·  859b43e", font=small, fill=ACCENT)
    draw.line((42, 64, WIDTH - 42, 64), fill=(42, 57, 77), width=1)

    draw.text((48, 84), f"{number}  /  {label}", font=ui, fill=BLUE)
    stamp = f"00:{int(t):02d}" if t < 60 else f"{int(t)//60:02d}:{int(t)%60:02d}"
    draw.text((WIDTH - 116, 86), stamp, font=small, fill=MUTED)

    card = (42, 119, WIDTH - 42, 613)
    draw.rounded_rectangle(card, radius=12, fill=TERMINAL, outline=(39, 53, 73), width=2)
    draw.rounded_rectangle((42, 119, WIDTH - 42, 154), radius=12, fill=(25, 35, 52))
    draw.rectangle((42, 141, WIDTH - 42, 154), fill=(25, 35, 52))
    for x, c in ((62, (255, 95, 86)), (82, (255, 189, 46)), (102, (39, 201, 63))):
        draw.ellipse((x, 130, x + 10, 140), fill=c)
    draw.text((130, 126), "Ubuntu  ·  SimpleShell interactive session", font=small, fill=MUTED)

    left, top = 66, 164
    cell_w, line_h = 10.7, 14.8
    for y, row in screen.buffer.items():
        for x, cell in row.items():
            char = cell.data
            if not char or char == " ":
                continue
            fg = color(cell.fg, FG)
            bg = color(cell.bg, TERMINAL)
            px, py = int(left + x * cell_w), int(top + y * line_h)
            if bg != TERMINAL:
                draw.rectangle((px, py, px + cell_w, py + line_h), fill=bg)
            draw.text((px, py - 2), char, font=mono, fill=fg)

    draw.rounded_rectangle((42, 630, WIDTH - 42, 691), radius=9, fill=PANEL)
    lines = textwrap.wrap(cue, width=98)[:2] if cue else []
    for i, line in enumerate(lines):
        bbox = draw.textbbox((0, 0), line, font=ui)
        draw.text(((WIDTH - (bbox[2] - bbox[0])) / 2, 641 + i * 23), line, font=ui, fill=FG)
    draw.rectangle((42, 704, WIDTH - 42, 708), fill=(37, 50, 68))
    progress = max(1, int((WIDTH - 84) * min(t / DURATION, 1.0)))
    draw.rectangle((42, 704, 42 + progress, 708), fill=ACCENT)
    return image


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--cast", required=True)
    parser.add_argument("--audio", required=True)
    parser.add_argument("--subtitles", required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("--thumbnail", required=True)
    args = parser.parse_args()

    with open(args.cast, encoding="utf-8") as source:
        header = json.loads(next(source))
        events = [json.loads(line) for line in source if line.strip()]
    cues = parse_srt(args.subtitles)
    screen = pyte.Screen(header["width"], header["height"])
    stream = pyte.Stream(screen)
    event_index = 0
    mono = ImageFont.truetype(FONT_MONO, 15)
    ui = ImageFont.truetype(FONT_UI, 17)
    small = ImageFont.truetype(FONT_UI, 13)

    with tempfile.TemporaryDirectory(prefix="simple-shell-video-frames-") as frame_dir:
        listing = Path(frame_dir) / "frames.txt"
        frame_paths = []
        thumbnail_written = False
        for second in range(DURATION):
            t = float(second)
            while event_index < len(events) and events[event_index][0] <= t:
                event = events[event_index]
                if event[1] == "o":
                    stream.feed(event[2])
                event_index += 1
            cue = next((text for start, end, text in cues if start <= t < end), "")
            frame = make_frame(screen, t, cue, mono, ui, small)
            if second == 44:
                frame.save(args.thumbnail, format="PNG", optimize=True)
                thumbnail_written = True
            frame_path = Path(frame_dir) / f"frame-{second:03d}.png"
            frame.save(frame_path, format="PNG", optimize=True)
            frame_paths.append(frame_path)
        if not thumbnail_written:
            frame.save(args.thumbnail, format="PNG", optimize=True)

        with listing.open("w", encoding="utf-8") as out:
            for path in frame_paths:
                out.write(f"file '{path.as_posix()}'\n")
                out.write("duration 1\n")
            out.write(f"file '{frame_paths[-1].as_posix()}'\n")

        ffmpeg = imageio_ffmpeg.get_ffmpeg_exe()
        command = [
            ffmpeg, "-y", "-hide_banner", "-loglevel", "error",
            "-f", "concat", "-safe", "0", "-i", str(listing),
            "-i", os.path.abspath(args.audio),
            "-map", "0:v:0", "-map", "1:a:0", "-vf", f"fps={FPS},format=yuv420p",
            "-c:v", "libx264", "-preset", "medium", "-crf", "22",
            "-c:a", "aac", "-b:a", "128k", "-af", "loudnorm=I=-18:TP=-1.5:LRA=8,apad", "-t", str(DURATION),
            "-movflags", "+faststart", os.path.abspath(args.output),
        ]
        subprocess.run(command, check=True)
    print(f"Rendered {args.output}")
    print(f"Duration target: {DURATION}s; frame rate: {FPS}; resolution: {WIDTH}x{HEIGHT}")


if __name__ == "__main__":
    main()
