#!/bin/sh
# Re-normalizes the source copy and recompiles only the named recovered files, after a change to
# source-fixes.json or normalize-sources.py that touches few files. Same flags as compile-all.sh;
# run compile-all.sh instead when a rule could affect the whole tree.
#
# Usage: browser/spike/recompile.sh src/path/a.c [src/path/b.c ...]
set -eu
cd "$(dirname "$0")/../.."
EMCC="${EMCC:-emcc}"
OUT=build/spike
CFLAGS="-O0 -g -fwrapv-pointer -w -Wno-error=incompatible-pointer-types -Wno-error=int-conversion -Wno-error=incompatible-function-pointer-types -Wno-error=implicit-function-declaration -Wno-error=implicit-int -Wno-error=return-mismatch -Ibrowser/include -I$OUT/tree/include"
python3 browser/spike/normalize-sources.py > /dev/null
for source in "$@"; do
  name="$(printf "%s" "${source#src/}" | sed "s/\.c\$//; s|/|__|g")"
  if "$EMCC" $CFLAGS -c "$OUT/tree/$source" -o "$OUT/obj/$name.o" 2> "$OUT/err/$name.txt"; then
    rm -f "$OUT/err/$name.txt"
    echo "compiled $source"
  else
    rm -f "$OUT/obj/$name.o"
    echo "FAILED $source (see $OUT/err/$name.txt)" >&2
    head -5 "$OUT/err/$name.txt" >&2
    exit 1
  fi
done
