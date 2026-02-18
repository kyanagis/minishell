#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT_DIR=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)
OUT_FILE=${1:-"$ROOT_DIR/fuzzing/minishell.dict"}
TMP_FILE=$(mktemp "${TMPDIR:-/tmp}/minishell_dict.XXXXXX")

cleanup()
{
	rm -f "$TMP_FILE"
}
trap cleanup EXIT INT TERM

mkdir -p "$(dirname -- "$OUT_FILE")"

cat > "$TMP_FILE" <<'DICT'
op_pipe="|"
op_in="<"
op_out=">"
op_heredoc="<<"
op_append=">>"
word_dq="\"text\""
word_sq="'text'"
word_var_user="$USER"
word_var_status="$?"
word_var_home="$HOME"
word_empty_dq="\"\""
word_empty_sq="''"
word_space="a b"
kw_if="if"
kw_then="then"
kw_fi="fi"
DICT

for file in "$ROOT_DIR"/src/built_in/*.c
 do
	name=$(basename "$file" .c)
	case "$name" in
		env_utils|export_util|exec_builtin)
			continue
			;;
	esac
	printf 'builtin_%s="%s"\n' "$name" "$name" >> "$TMP_FILE"
done

sort -u "$TMP_FILE" > "$OUT_FILE"
line_count=$(wc -l < "$OUT_FILE" | tr -d ' ')
echo "generated $OUT_FILE ($line_count entries)"
