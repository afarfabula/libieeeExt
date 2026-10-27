#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
board="$repo_root/board.cpp"
tests="$repo_root/tests/board_tests.cpp"
replays="$repo_root/tests/problem_replay_tests.cpp"
cxx="${CXX:-g++}"
build_dir="$(mktemp -d)"
trap 'rm -rf -- "$build_dir"' EXIT

common_flags=(-std=c++17 -O2 -Wall -Wextra -Wpedantic)
if [[ "${SANITIZE:-0}" == "1" ]]; then
    common_flags=(-std=c++17 -O1 -g -fsanitize=address,undefined
                  -fno-omit-frame-pointer)
fi

source_size="$(wc -c < "$board")"
if (( source_size >= 100000 )); then
    echo "board.cpp is ${source_size} bytes; the 100,000-byte budget is exhausted" >&2
    exit 1
fi

echo "board.cpp: ${source_size}/100000 bytes"
"$cxx" "${common_flags[@]}" "$board" -o "$build_dir/board"
"$build_dir/board" </dev/null
"$cxx" "${common_flags[@]}" "$tests" -o "$build_dir/board_tests"
"$build_dir/board_tests"
"$cxx" "${common_flags[@]}" "$replays" -o "$build_dir/problem_replay_tests"
"$build_dir/problem_replay_tests"
