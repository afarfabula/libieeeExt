#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
board="$repo_root/board.cpp"
dist="$repo_root/dist"
archive="$dist/libieeeExt-100k.zip"
limit=100000

mkdir -p "$dist"
zip -X -9 -j "$archive" "$board" >/dev/null

source_size="$(wc -c < "$board")"
archive_size="$(wc -c < "$archive")"
if (( source_size >= limit )); then
    echo "board.cpp is ${source_size} bytes; limit is ${limit}" >&2
    exit 1
fi
if (( archive_size >= limit )); then
    echo "archive is ${archive_size} bytes; limit is ${limit}" >&2
    exit 1
fi

echo "archive: $archive"
echo "archive size: ${archive_size}/${limit} bytes"
echo "source size: ${source_size}/${limit} bytes"
sha256sum "$archive"
