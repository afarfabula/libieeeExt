#!/usr/bin/env python3
"""Generate the CS Academy library file from the tested single-file board."""

from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "board.cpp"
TARGET = ROOT / "workspace" / "board.cpp"
MARKER = "// Contest entry point"


def main() -> None:
    source = SOURCE.read_text(encoding="utf-8")
    if MARKER not in source:
        raise SystemExit(f"marker not found in {SOURCE}: {MARKER}")
    library = source.split(MARKER, 1)[0].rstrip()
    library += "\n\n} // namespace ib\n"
    TARGET.parent.mkdir(parents=True, exist_ok=True)
    TARGET.write_text(library, encoding="utf-8")
    print(f"wrote {TARGET.relative_to(ROOT)} ({len(library.encode())} bytes)")


if __name__ == "__main__":
    main()

