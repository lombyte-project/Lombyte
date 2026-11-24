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
# The boot program (NAME=boot) is the executable on its own, before any level program is loaded:
# executable names keep their own addresses (no exe-data map), the level's handler tables are not
# installed, and files whose text depends on the map use their boot variants (compile-all.sh).
RAM="$S/ram"
if [ "$NAME" = boot ]; then
  RAM="$S/boot-ram"
  SPIKE_NO_EXE_MAP=1 SPIKE_RAM_OUT="$RAM.s" node browser/spike/ram-image.mjs "$S/report-$LEVEL.json"
else
  node browser/spike/ram-image.mjs "$S/report-$LEVEL.json"
fi
emcc -c "$RAM.s" -o "$RAM.o"
emcc -O2 -w -Ibrowser/include -Iinclude -c browser/level-math.c -o "$S/level-math.o"
emcc -O2 -fno-builtin -Wall -Wextra -Werror -Ibrowser/include -Iinclude -c browser/spike/ee-helpers.c -o "$S/ee-helpers.o"
emcc -O2 -fno-builtin -Wall -Wextra -Werror -Ibrowser/include -Iinclude -c browser/moby-animation-advance.c -o "$S/moby-animation-advance.o"
emcc -O2 -fno-builtin -fno-strict-aliasing -Wall -Wextra -Werror -Ibrowser/include -Iinclude -c browser/moby-joint-matrices.c -o "$S/moby-joint-matrices.o"
emcc -O2 -Wall -Wextra -Werror -Ibrowser/include -Iinclude -c browser/spike/services.c -o "$S/services.o"
# The static-world collision queries (assembly-only on the console) from the gated collision module.
# Its hit record and collision_point bind to the original labels, which ram.o pins.
emcc -O2 -Wall -Wextra -Werror -DRNC_SPIKE_COLLISION -Ibrowser/include -Iinclude -c browser/collision.c -o "$S/collision.o"
# The level's handler tables with WebAssembly function references (see the file's header).
TABLES=""
TABLE_ROOTS=""
if [ -f "browser/spike/level-tables-$LEVEL.c" ] && [ "$NAME" != boot ]; then
  emcc -O1 -Wall -Wextra -Werror -Ibrowser/include -Iinclude -c "browser/spike/level-tables-$LEVEL.c" -o "$S/level-tables.o"
  TABLES="$S/level-tables.o"
  # The tables are data, reached at run time through pinned addresses, so no relocation leads the
  # closure walk to them. Name them as extra closure roots so every handler they list is linked.
  TABLE_ROOTS="$(grep -o '__asm__("D_L[0-9]*_[0-9A-F]*")' "browser/spike/level-tables-$LEVEL.c" | sed 's/__asm__("//; s/")//' | sort -u | tr '\n' ',' | sed 's/,$//')"
fi
# libc for the adapters' math and memcpy calls; only the members they reference are linked.
LIBC="$(em-config CACHE)/sysroot/lib/wasm32-emscripten/libc.a"

node browser/spike/closure.mjs "$ROOT${TABLE_ROOTS:+,$TABLE_ROOTS}" "$LEVEL" --list > "$S/$NAME-closure.txt"
sed -n '/^reached files:/,$p' "$S/$NAME-closure.txt" | tail -n +2 \
  | sed -E 's|^ +src/||; s|\.c$||; s|/|__|g; s|^|'"$S"'/obj/|; s|$|.o|' > "$S/$NAME-objects.txt"
if [ "$NAME" = boot ]; then # the boot variants replace their level-program counterparts
  while read -r object; do
    variant="$S/obj-boot/$(basename "$object")"
    if [ -f "$variant" ]; then echo "$variant"; else echo "$object"; fi
  done < "$S/$NAME-objects.txt" > "$S/$NAME-objects.tmp" && mv "$S/$NAME-objects.tmp" "$S/$NAME-objects.txt"
fi

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
      --export-if-defined=spike_gs_upload_address --export-if-defined=spike_vu0_micro_program_requests --export-if-defined=spike_vsync_waits --export-if-defined=spike_vif1_chain_address --export-if-defined=spike_vif1_chain_sends --export-if-defined=rnc_animation_runaway --export-if-defined=spike_sound_batches --export-if-defined=spike_sound_commands --export-if-defined=spike_sound_stream_starts --export-if-defined=spike_world_passes --export-if-defined=spike_spr_transfers --export-if-defined=spike_pad_reads --export-if-defined=spike_iop_modules --export-if-defined=spike_gs_crt_mode --export-if-defined=spike_movies_played --export-if-defined=spike_mc_calls --export-if-defined=spike_moby_matrices --export-if-defined=spike_resume_after_handover --export-if-defined=spike_cd_callback_installed --export-if-defined=spike_streams_without_header --export-if-defined=spike_vblank_runs --export-if-defined=spike_local_pose_calls --export-if-defined=rnc_layer_pose_nan --export-if-defined=rnc_layer_pose_calls --export-if-defined=spike_collision_bad_segments --export-if-defined=spike_collision_first_bad_segment --export-if-defined=spike_moby_matrix_buffer \
      --export-if-defined=runtime_collision_reset --export-if-defined=runtime_collision_stage_address \
      --export-if-defined=runtime_collision_capacity --export-if-defined=runtime_collision_load \
      --export-if-defined=spike_install_level_tables --export-if-defined=spike_collision_unverified --export-if-defined=runtime_collision_cells --export-if-defined=runtime_collision_faces \
      --gc-sections --allow-undefined --allow-multiple-definition \
      --global-base=1048576 -z stack-size=524288 \
      -o "$S/$NAME.wasm" "$RAM.o" "$S/level-math.o" "$S/ee-helpers.o" "$S/moby-animation-advance.o" "$S/moby-joint-matrices.o" "$S/services.o" "$S/collision.o" $TABLES @"$S/$NAME-objects.txt" "$S/$NAME-stubs.o" "$LIBC" \
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

# A module that reaches sceGsSyncV imports spike_vsync_yield; Asyncify lets that import unwind to
# the host and resume, so the original main loop runs one frame per host call (run-closure.mjs).
if grep -q spike_vsync_yield "$S/$NAME.wasm" 2>/dev/null; then
  "$(em-config BINARYEN_ROOT)/bin/wasm-opt" "$S/$NAME.wasm" -g --asyncify \
    --pass-arg=asyncify-imports@env.spike_vsync_yield,env.host_disc_read,env.spike_play_movie -o "$S/$NAME.wasm"
  echo "asyncify: sceGsSyncV yields to the host"
fi
echo "linked $S/$NAME.wasm from $(wc -l < "$S/$NAME-objects.txt" | tr -d ' ') recovered files"
echo "prototype conflicts stubbed: $(wc -l < "$S/$NAME-conflicts.txt" | tr -d ' ') ($S/$NAME-conflicts.txt)"
echo "caller/definition signature mismatches (trap at call): $(grep -c 'warning: function signature mismatch' "$S/$NAME-link.txt" || true)"
