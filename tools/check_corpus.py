#!/usr/bin/env python3
"""Validate the curated IEEEXtreme corpus and print coverage statistics."""

from __future__ import annotations

import argparse
import re
from collections import Counter, defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
CORPUS = ROOT / "docs" / "corpus" / "problem-tags.tsv"
REPLAY_TESTS = ROOT / "tests" / "problem_replay_tests.cpp"
REQUIRED_COLUMNS = [
    "id",
    "edition",
    "problem",
    "tags",
    "board_api",
    "solution_ref",
    "replay_status",
    "note",
]
STATUSES = {"PASS-E2E", "PASS-MODULE", "TODO-ADAPTER", "TODO-CORE"}
SOURCE_PREFIXES = {"HL", "ES", "EN", "PA"}
E2E_MARKERS = {
    "10-dog-walking": "Dog Walking",
    "10-full-adder": "Full Adder",
    "10-n-palindromes": "N-Palindromes",
}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--limit", type=int, default=30, help="number of tag rows to print"
    )
    return parser.parse_args()


def load_rows() -> list[dict[str, str]]:
    errors: list[str] = []
    lines = CORPUS.read_text(encoding="utf-8").splitlines()
    if not lines:
        raise SystemExit(f"{CORPUS} is empty")

    header = lines[0].split("\t")
    if header != REQUIRED_COLUMNS:
        raise SystemExit(
            f"bad header: expected {REQUIRED_COLUMNS!r}, got {header!r}"
        )

    rows: list[dict[str, str]] = []
    for line_no, line in enumerate(lines[1:], 2):
        fields = line.split("\t")
        if len(fields) != len(REQUIRED_COLUMNS):
            errors.append(f"line {line_no}: expected 8 columns, got {len(fields)}")
            continue
        row = dict(zip(REQUIRED_COLUMNS, fields))
        rows.append(row)

        if not re.fullmatch(r"\d+\.0", row["edition"]):
            errors.append(f"line {line_no}: bad edition {row['edition']!r}")
        if row["replay_status"] not in STATUSES:
            errors.append(f"line {line_no}: bad status {row['replay_status']!r}")
        prefix, separator, reference = row["solution_ref"].partition(":")
        if prefix not in SOURCE_PREFIXES or not separator or not reference:
            errors.append(f"line {line_no}: bad solution_ref {row['solution_ref']!r}")
        for tag in row["tags"].split("|"):
            if not re.fullmatch(r"[a-z0-9]+(?:-[a-z0-9]+)*", tag):
                errors.append(f"line {line_no}: bad tag {tag!r}")
        for api in row["board_api"].split("|"):
            if not api:
                errors.append(f"line {line_no}: empty board_api item")

    ids = [row["id"] for row in rows]
    for identifier, count in Counter(ids).items():
        if count != 1:
            errors.append(f"duplicate id {identifier!r}: {count}")

    replay_source = REPLAY_TESTS.read_text(encoding="utf-8")
    e2e_ids = {row["id"] for row in rows if row["replay_status"] == "PASS-E2E"}
    if e2e_ids != set(E2E_MARKERS):
        errors.append(
            "PASS-E2E rows do not match the executable replay tests: "
            f"{sorted(e2e_ids)}"
        )
    for identifier, marker in E2E_MARKERS.items():
        if marker not in replay_source:
            errors.append(f"missing replay marker for {identifier}: {marker!r}")

    if errors:
        raise SystemExit("corpus validation failed:\n- " + "\n- ".join(errors))
    return rows


def main() -> None:
    args = parse_args()
    rows = load_rows()
    tag_counts = Counter(tag for row in rows for tag in row["tags"].split("|"))
    status_counts = Counter(row["replay_status"] for row in rows)
    edition_counts = Counter(row["edition"] for row in rows)
    api_counts = Counter(
        api for row in rows for api in row["board_api"].split("|")
    )

    print(f"validated {len(rows)} problems from {len(edition_counts)} editions")
    print("editions: " + ", ".join(
        f"{edition}={edition_counts[edition]}"
        for edition in sorted(edition_counts, key=float)
    ))
    print("statuses: " + ", ".join(
        f"{status}={status_counts[status]}" for status in sorted(STATUSES)
    ))
    explicit_missing = {api: count for api, count in api_counts.items()
                        if api.startswith("missing:")}
    print("explicit missing APIs: " + (
        ", ".join(
            f"{api}={explicit_missing[api]}" for api in sorted(explicit_missing)
        )
        if explicit_missing else "none"
    ))
    print("\ntag frequency:")
    for tag, count in tag_counts.most_common(args.limit):
        editions = sorted(
            {
                row["edition"]
                for row in rows
                if tag in row["tags"].split("|")
            },
            key=float,
        )
        print(f"{count:3}  {tag:22} {','.join(editions)}")

    by_status: dict[str, list[str]] = defaultdict(list)
    for row in rows:
        by_status[row["replay_status"]].append(row["id"])
    print("\nreplay queue:")
    for status in ("PASS-E2E", "PASS-MODULE", "TODO-ADAPTER", "TODO-CORE"):
        print(f"{status}: {len(by_status[status])}")


if __name__ == "__main__":
    main()
