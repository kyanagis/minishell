#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT_DIR=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)
IN_DIR=${1:-"$ROOT_DIR/fuzzing/corpus/in"}
OUT_DIR=${2:-"$ROOT_DIR/fuzzing/corpus/out"}
DICT_FILE=${3:-"$ROOT_DIR/fuzzing/minishell.dict"}

resolve_afl_cc()
{
	if [ -n "${AFL_CC:-}" ] && command -v "$AFL_CC" >/dev/null 2>&1
	then
		echo "$AFL_CC"
		return 0
	fi
	for cc in afl-gcc afl-cc afl-clang-fast
	do
		if command -v "$cc" >/dev/null 2>&1
		then
			echo "$cc"
			return 0
		fi
	done
	return 1
}

if ! command -v afl-fuzz >/dev/null 2>&1
then
	echo "error: afl-fuzz not found. Install AFL++ first." >&2
	exit 1
fi

AFL_COMPILER=$(resolve_afl_cc || true)
if [ -z "$AFL_COMPILER" ]
then
	echo "error: AFL compiler wrapper not found (tried AFL_CC, afl-gcc, afl-cc, afl-clang-fast)." >&2
	exit 1
fi

if [ ! -d "$IN_DIR" ] || [ -z "$(find "$IN_DIR" -type f -print -quit)" ]
then
	"$SCRIPT_DIR/prepare_seeds.sh" "$IN_DIR"
fi

if [ ! -f "$DICT_FILE" ]
then
	"$SCRIPT_DIR/gen_dictionary.sh" "$DICT_FILE"
fi

mkdir -p "$OUT_DIR"

make -C "$ROOT_DIR" re AFL=1 AFL_CC="$AFL_COMPILER"

echo "afl compiler: $AFL_COMPILER"
echo "input corpus: $IN_DIR"
echo "output dir:  $OUT_DIR"
echo "dictionary:  $DICT_FILE"

echo "run: AFL_SKIP_CPUFREQ=1 AFL_AUTORESUME=1 afl-fuzz -i $IN_DIR -o $OUT_DIR -x $DICT_FILE -- $ROOT_DIR/minishell"
exec AFL_SKIP_CPUFREQ=1 AFL_AUTORESUME=1 afl-fuzz -i "$IN_DIR" -o "$OUT_DIR" -x "$DICT_FILE" -- "$ROOT_DIR/minishell"
