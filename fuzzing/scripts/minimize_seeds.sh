#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT_DIR=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)
IN_DIR=${1:-"$ROOT_DIR/fuzzing/corpus/in"}
OUT_DIR=${2:-"$ROOT_DIR/fuzzing/corpus/min"}
AFL_CMIN_BIN=${AFL_CMIN_BIN:-afl-cmin}

if ! command -v "$AFL_CMIN_BIN" >/dev/null 2>&1
then
	echo "error: $AFL_CMIN_BIN not found. Install AFL++ first." >&2
	exit 1
fi

if [ ! -x "$ROOT_DIR/minishell" ]
then
	echo "error: minishell binary not found. run 'make' first." >&2
	exit 1
fi

mkdir -p "$OUT_DIR"
rm -f "$OUT_DIR"/*

AFL_SKIP_CPUFREQ=1 "$AFL_CMIN_BIN" -i "$IN_DIR" -o "$OUT_DIR" -- "$ROOT_DIR/minishell"
count=$(find "$OUT_DIR" -type f | wc -l | tr -d ' ')
echo "minimized corpus to $count seeds in $OUT_DIR"
