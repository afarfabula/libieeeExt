#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="$(mktemp -d)"
trap 'rm -rf -- "$build_dir"' EXIT

python3 "$repo_root/tools/build_workspace_board.py"
g++ -std=c++23 -static -O2 -pthread -Wall -Wno-unused-result \
    -DCS_ACADEMY -DONLINE_JUDGE \
    "$repo_root/workspace/Main.cpp" -o "$build_dir/workspace"

check() {
    local input="$1"
    local expected="$2"
    local actual
    actual="$(printf '%s' "$input" | "$build_dir/workspace")"
    if [[ "$actual" != "$expected" ]]; then
        echo "workspace sample failed: expected $expected, got $actual" >&2
        exit 1
    fi
}

check $'4 4 4\n.S..\n##..\n...#\n#T##\nRRDL\n' "2"
check $'2 1 3\nS\nT\nRUL\n' "-1"
check $'1 10 6\nS........T\nLLRRRR\n' "1"

echo "workspace template samples passed"

