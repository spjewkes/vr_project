#!/usr/bin/env python3
"""Trim trailing whitespace and ensure text files end with one newline."""

from pathlib import Path
import sys


def fix_text(text: str) -> str:
    lines = text.splitlines()
    fixed = "\n".join(line.rstrip(" \t") for line in lines)

    if fixed:
        fixed += "\n"

    return fixed


def main() -> int:
    changed = False

    for filename in sys.argv[1:]:
        path = Path(filename)
        if not path.is_file():
            continue

        data = path.read_bytes()
        if b"\0" in data:
            continue

        try:
            text = data.decode("utf-8")
        except UnicodeDecodeError:
            continue

        fixed = fix_text(text)
        if fixed != text:
            path.write_text(fixed, encoding="utf-8")
            print(f"fixed whitespace: {path}")
            changed = True

    return 1 if changed else 0


if __name__ == "__main__":
    raise SystemExit(main())
