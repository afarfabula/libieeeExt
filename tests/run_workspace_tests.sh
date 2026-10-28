#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="$(mktemp -d)"
trap 'rm -rf -- "$build_dir"' EXIT

python3 "$repo_root/tools/build_workspace_board.py"
g++ -std=c++23 -static -O2 -pthread -Wall -Wno-unused-result \
    -DCS_ACADEMY -DONLINE_JUDGE \
    "$repo_root/workspace/Main.cpp" -o "$build_dir/workspace"
g++ -std=c++23 -static -O2 -pthread -Wall -Wno-unused-result \
    -DCS_ACADEMY -DONLINE_JUDGE \
    "$repo_root/workspace/TemplateMain.cpp" -o "$build_dir/template"
g++ -std=c++23 -static -O2 -pthread -Wall -Wno-unused-result \
    -DCS_ACADEMY -DONLINE_JUDGE \
    "$repo_root/workspace/SubmissionMain.cpp" -o "$build_dir/submission"
g++ -std=c++23 -static -O2 -pthread -Wall -Wno-unused-result \
    -DCS_ACADEMY -DONLINE_JUDGE \
    "$repo_root/workspace/RobotSubmission.cpp" -o "$build_dir/robot"

check() {
    local input="$1"
    local expected="$2"
    local actual
    actual="$(printf '%s' "$input" | "$build_dir/submission")"
    if [[ "$actual" != "$expected" ]]; then
        echo "workspace sample failed: expected $expected, got $actual" >&2
        exit 1
    fi
}

check_robot() {
    local input="$1"
    local expected="$2"
    local actual
    actual="$(printf '%s' "$input" | "$build_dir/robot")"
    if [[ "$actual" != "$expected" ]]; then
        echo "robot submission failed: expected $expected, got $actual" >&2
        exit 1
    fi
}

check $'4 4 4\n.S..\n##..\n...#\n#T##\nRRDL\n' "2"
check $'2 1 3\nS\nT\nRUL\n' "-1"
check $'1 10 6\nS........T\nLLRRRR\n' "1"
check_robot $'4 4 4\n.S..\n##..\n...#\n#T##\nRRDL\n' "2"
check_robot $'2 1 3\nS\nT\nRUL\n' "-1"
check_robot $'1 10 6\nS........T\nLLRRRR\n' "1"
check_robot $'4 4 3\n#...\n...S\n..##\n#..T\nRLD\n' "5"

echo "workspace template samples passed"

