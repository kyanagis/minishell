#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT_DIR=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)
OUT_DIR=${1:-"$ROOT_DIR/fuzzing/corpus/in"}

mkdir -p "$OUT_DIR"

cat > "$OUT_DIR/00_smoke" <<'__SEED__'
echo hello
exit
__SEED__

cat > "$OUT_DIR/01_quotes" <<'__SEED__'
echo "hello $USER"
echo 'single quoted'
exit
__SEED__

cat > "$OUT_DIR/02_pipeline" <<'__SEED__'
echo alpha beta gamma | wc -w
exit
__SEED__

cat > "$OUT_DIR/03_redir_out" <<'__SEED__'
echo out > /tmp/minish_fuzz_out
cat /tmp/minish_fuzz_out
exit
__SEED__

cat > "$OUT_DIR/04_redir_in" <<'__SEED__'
cat < /dev/null
exit
__SEED__

cat > "$OUT_DIR/05_append" <<'__SEED__'
echo one > /tmp/minish_fuzz_append
echo two >> /tmp/minish_fuzz_append
cat /tmp/minish_fuzz_append
exit
__SEED__

cat > "$OUT_DIR/06_heredoc_basic" <<'__SEED__'
cat << LIMITER
line 1
line 2
LIMITER
exit
__SEED__

cat > "$OUT_DIR/07_heredoc_expand" <<'__SEED__'
cat << LIMITER
Hello $USER
LIMITER
exit
__SEED__

cat > "$OUT_DIR/08_heredoc_quoted" <<'__SEED__'
cat << 'LIMITER'
$USER literal
LIMITER
exit
__SEED__

cat > "$OUT_DIR/09_env_status" <<'__SEED__'
env | head -n 3
echo $?
exit
__SEED__

cat > "$OUT_DIR/10_export_unset" <<'__SEED__'
export FUZZ_KEY=VALUE
echo $FUZZ_KEY
unset FUZZ_KEY
echo $FUZZ_KEY
exit
__SEED__

cat > "$OUT_DIR/11_cd_pwd" <<'__SEED__'
pwd
cd /
pwd
exit
__SEED__

cat > "$OUT_DIR/12_exit_numeric" <<'__SEED__'
exit 7
__SEED__

cat > "$OUT_DIR/13_syntax_pipe" <<'__SEED__'
| echo ng
echo after
exit
__SEED__

cat > "$OUT_DIR/14_syntax_redir" <<'__SEED__'
echo hi >
echo after
exit
__SEED__

cat > "$OUT_DIR/15_long_token" <<'__SEED__'
echo aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
exit
__SEED__

count=$(find "$OUT_DIR" -type f | wc -l | tr -d ' ')
echo "prepared $count seeds in $OUT_DIR"
