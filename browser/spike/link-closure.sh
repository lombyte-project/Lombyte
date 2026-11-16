#!/bin/sh
# Links everything one root function reaches into build/spike/<name>.wasm, unchanged, so it can
# be run until the first missing service traps by name (browser/spike/run-closure.mjs).
#
#   - fixed-address globals come from the pinned RAM image (ram-image.mjs);
#   - assembly-only helpers already ported (browser/level-math.c, browser/spike/ee-helpers.c)
#     are linked in;
#   - every other missing function becomes a wasm import, which the runner turns into a named trap;
#   - a missing function whose callers disagree on its prototype cannot be one import, so it gets
#     a stub here and is listed in <name>-conflicts.txt.
#
# This is a measurement build. It deliberately uses --allow-undefined, which the gated browser
# targets forbid; nothing here is copied into build/browser or loaded by the game page.
#
# Usage: browser/spike/link-closure.sh <root symbol>[,<root symbol>...] <name> [level]
set -eu
cd "$(dirname "$0")/../.."
ROOT="$1"; NAME="$2"; LEVEL="${3:-l00}"
S=build/spike
LLVM="$(em-config LLVM_ROOT)"
EXPORTS="$(printf '%s' "$ROOT" | tr ',' '\n' | sed 's/^/--export=/' | tr '\n' ' ')"

node browser/spike/report.mjs "$LEVEL" --json "$S/report-$LEVEL.json" > "$S/report-$LEVEL.txt" 2>/dev/null
python3 browser/spike/exe-data-map.py "${LEVEL#l}"
node browser/spike/ram-image.mjs "$S/report-$LEVEL.json"
emcc -c "$S/ram.s" -o "$S/ram.o"
emcc -O2 -w -Ibrowser/include -Iinclude -c browser/level-math.c -o "$S/level-math.o"
emcc -O2 -fno-builtin -Wall -Wextra -Werror -Ibrowser/include -Iinclude -c browser/spike/ee-helpers.c -o "$S/ee-helpers.o"
emcc -O2 -Wall -Wextra -Werror -Ibrowser/include -Iinclude -c browser/spike/services.c -o "$S/services.o"

node browser/spike/closure.mjs "$ROOT" "$LEVEL" --list > "$S/$NAME-closure.txt"
sed -n '/^reached files:/,$p' "$S/$NAME-closure.txt" | tail -n +2 \
  | sed -E 's|^ +src/||; s|\.c$||; s|/|__|g; s|^|'"$S"'/obj/|; s|$|.o|' > "$S/$NAME-objects.txt"

: > "$S/$NAME-conflicts.txt"
pass=0
while :; do
  pass=$((pass + 1))
  { echo 'extern void spike_conflict(const char *) __attribute__((import_module("env"), import_name("spike_conflict")));'
    n=0
    while read -r symbol; do
      n=$((n + 1))
      echo "void spike_stub_$n(void) __asm__(\"$symbol\"); void spike_stub_$n(void) { spike_conflict(\"$symbol\"); }"
    done < "$S/$NAME-conflicts.txt"
  } > "$S/$NAME-stubs.c"
  emcc -O0 -w -c "$S/$NAME-stubs.c" -o "$S/$NAME-stubs.o"
  # ram.o first: the RAM image must be the first data segment so it starts at --global-base.
  # The linker puts the stack at the bottom, 0-0x7FFFF, leaving 0x80000-0xFFFFF free; the
  # scratchpad is mapped at 0xF0000-0xF3FFF (see normalize-sources.py).
  if "$LLVM/wasm-ld" --no-entry $EXPORTS --export=ee_ram --export=__stack_pointer \
      --export-if-defined=spike_gs_upload_count --export-if-defined=spike_gs_upload_overflow \
      --export-if-defined=spike_gs_upload_address --export-if-defined=spike_vu0_micro_program_requests --export-if-defined=spike_vsync_waits \
      --gc-sections --allow-undefined --allow-multiple-definition \
      --global-base=1048576 -z stack-size=524288 \
      -o "$S/$NAME.wasm" "$S/ram.o" "$S/level-math.o" "$S/ee-helpers.o" "$S/services.o" @"$S/$NAME-objects.txt" "$S/$NAME-stubs.o" \
      > "$S/$NAME-link.txt" 2>&1; then
    break
  fi
  new="$(grep 'error: function signature mismatch' "$S/$NAME-link.txt" | sed 's/.*mismatch: //' || true)"
  if [ -z "$new" ] || [ "$pass" -gt 200 ]; then
    echo "link failed; see $S/$NAME-link.txt" >&2
    grep 'error' "$S/$NAME-link.txt" | head -5 >&2
    exit 1
  fi
  echo "$new" >> "$S/$NAME-conflicts.txt"
done

echo "linked $S/$NAME.wasm from $(wc -l < "$S/$NAME-objects.txt" | tr -d ' ') recovered files"
echo "prototype conflicts stubbed: $(wc -l < "$S/$NAME-conflicts.txt" | tr -d ' ') ($S/$NAME-conflicts.txt)"
echo "caller/definition signature mismatches (trap at call): $(grep -c 'warning: function signature mismatch' "$S/$NAME-link.txt" || true)"
