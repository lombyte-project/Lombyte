#!/bin/sh
# Whole-tree spike: compile every recovered C file to a wasm32 object, logic unchanged, and keep the
# objects and per-file diagnostics under build/spike/ (ignored). This measures the port; it does
# not produce a runnable module. Follow with browser/spike/report.mjs.
#
# Usage: browser/spike/compile-all.sh [jobs]
set -eu
cd "$(dirname "$0")/../.."
EMCC="${EMCC:-emcc}"
JOBS="${1:-8}"
OUT=build/spike
rm -rf "$OUT/obj" "$OUT/err"
mkdir -p "$OUT/obj" "$OUT/err"

# Legacy-C conversions that clang now rejects are downgraded to warnings, as the existing browser
# targets do for recovered files. No -flto, so llvm-nm and llvm-objdump read real wasm symbol and
# relocation tables; -O0 keeps every reference. -g lets run-closure.mjs map a trap to a source line.
# -fwrapv-pointer makes pointer arithmetic wrap at 32 bits as it does on the EE: without it a
# negative index off a fixed-address global (mission_state[-1]) is folded into a load offset that
# WebAssembly evaluates past 4 GiB and traps.
CFLAGS="-O2 -g -fwrapv-pointer -fno-strict-aliasing -w -Wno-error=incompatible-pointer-types -Wno-error=int-conversion -Wno-error=incompatible-function-pointer-types -Wno-error=implicit-function-declaration -Wno-error=implicit-int -Wno-error=return-mismatch -Ibrowser/include -I$OUT/tree/include"

# Compile a copy in which each address-named function has one spelling (normalize-sources.py);
# src/ and include/ themselves are not modified. Object names still follow the src/ path.
python3 browser/spike/exe-data-map.py 0  # read by normalize-sources.py (address literals)
python3 browser/spike/normalize-sources.py
(cd "$OUT/tree" && find src -name '*.c' -not -path 'src/check/*' | sort) > "$OUT/sources.txt"

export EMCC CFLAGS OUT

# One sh per file: POSIX sh cannot export a function to xargs.
xargs -P "$JOBS" -I{} sh -c '
  source="$1"
  name="$(printf "%s" "${source#src/}" | sed "s/\.c\$//; s|/|__|g")"
  if "$EMCC" $CFLAGS -c "$OUT/tree/$source" -o "$OUT/obj/$name.o" 2> "$OUT/err/$name.txt"; then
    rm -f "$OUT/err/$name.txt"
  else
    rm -f "$OUT/obj/$name.o"
  fi
' sh {} < "$OUT/sources.txt"

total="$(wc -l < "$OUT/sources.txt" | tr -d ' ')"
ok="$(find "$OUT/obj" -name '*.o' | wc -l | tr -d ' ')"
echo "compiled $ok of $total recovered C files; failures in $OUT/err/"

# The boot program (boot.wasm, link-closure.sh with name "boot") runs before a level program is
# loaded: executable data keeps its own addresses. Only files whose normalized text depends on the
# executable-to-level map (address literals in boot sources) differ; those get boot variants.
rm -rf "$OUT/tree-boot" "$OUT/obj-boot"; mkdir -p "$OUT/obj-boot"
SPIKE_NO_EXE_MAP=1 SPIKE_TREE="$OUT/tree-boot" python3 browser/spike/normalize-sources.py > /dev/null
variants=0
while read -r source; do
  if ! cmp -s "$OUT/tree/$source" "$OUT/tree-boot/$source"; then
    name="$(printf "%s" "${source#src/}" | sed "s/\.c\$//; s|/|__|g")"
    if "$EMCC" $(printf '%s' "$CFLAGS" | sed "s|$OUT/tree/include|$OUT/tree-boot/include|") -c "$OUT/tree-boot/$source" -o "$OUT/obj-boot/$name.o" 2>/dev/null; then
      variants=$((variants + 1))
    fi
  fi
done < "$OUT/sources.txt"
echo "boot program variants: $variants files"

