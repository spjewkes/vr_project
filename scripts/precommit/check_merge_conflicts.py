#!/usr/bin/env python3
"""Fail if text files contain unresolved merge conflict markers."""

from pathlib import Path
import sys

MARKERS = ("<<<<<<<", "=======", ">>>>>>>")


def main() -> int:
    failed = False

    for filename in sys.argv[1:]:
        path = Path(filename)
        if not path.is_file():
            continue

        try:
            lines = path.read_text(encoding="utf-8").splitlines()
        except UnicodeDecodeError:
            continue

        for line_number, line in enumerate(lines, start=1):
            if any(line.startswith(marker) for marker in MARKERS):
                print(f"{path}:{line_number}: unresolved merge conflict marker")
                failed = True

    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
