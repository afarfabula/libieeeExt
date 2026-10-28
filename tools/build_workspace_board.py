#!/usr/bin/env python3
"""Generate the CS Academy library file from the tested single-file board."""

from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "board.cpp"
TARGET = ROOT / "workspace" / "board.cpp"
TEMPLATE_MAIN = ROOT / "workspace" / "TemplateMain.cpp"
SUBMISSION_MAIN = ROOT / "workspace" / "SubmissionMain.cpp"
MARKER = "// Contest entry point"

TEMPLATE_SUFFIX = """

void solve() {
    // Put contest-specific input, algorithm selection, and output here.
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
"""


def compact_source(source: str) -> str:
    """Remove comments, blank lines and indentation without changing tokens."""
    lines = []
    for line in source.splitlines():
        line = line.strip()
        if not line or line.startswith("//"):
            continue
        lines.append(line)
    return "\n".join(lines) + "\n"


def main() -> None:
    source = SOURCE.read_text(encoding="utf-8")
    if MARKER not in source:
        raise SystemExit(f"marker not found in {SOURCE}: {MARKER}")
    library = source.split(MARKER, 1)[0].rstrip()
    library += "\n\n} // namespace ib\n"
    template = compact_source(library + TEMPLATE_SUFFIX)
    submission = compact_source(source)
    TARGET.parent.mkdir(parents=True, exist_ok=True)
    TARGET.write_text(library, encoding="utf-8")
    TEMPLATE_MAIN.write_text(template, encoding="utf-8")
    SUBMISSION_MAIN.write_text(submission, encoding="utf-8")
    print(f"wrote {TARGET.relative_to(ROOT)} ({len(library.encode())} bytes)")
    print(
        f"wrote {TEMPLATE_MAIN.relative_to(ROOT)} "
        f"({len(TEMPLATE_MAIN.read_bytes())} bytes)"
    )
    print(
        f"wrote {SUBMISSION_MAIN.relative_to(ROOT)} "
        f"({len(SUBMISSION_MAIN.read_bytes())} bytes)"
    )


if __name__ == "__main__":
    main()

