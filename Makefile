# Lombyte — Ratchet & Clank (PS2, USA) matching decompilation
#
# The boot ELF is recompiled and must match retail byte-for-byte (full-ELF
# SHA gate); `make overlays` proves the level overlay C function by function.
# `make iso` patches the ELF into a copy of your legally owned disc image,
# leaving the raw game data untouched.
#
# Setup: put a legally owned disc dump in dumps/ (e.g. dumps/game.iso) and
# install the toolchain (docs/build/building.md). Then run `make elf`.

.DEFAULT_GOAL := check
.PHONY: elf iso clean-iso check progress overlays format lint browser-smoke browser-assets browser-assets-test
.PHONY: browser-runtime browser-runtime-test browser-gameplay-test browser-level browser-planet-wad
.PHONY: browser-wad browser-wad-test browser-game browser-hero browser-collision browser-collision-test
.PHONY: browser-spike browser-spike-mission browser-audio browser-audio-test

EMCC ?= emcc
EMCC_VERSION ?= 6.0.13-git
NODE ?= node
BROWSER_HERO_CFLAGS := -O2 -flto -Wno-unknown-attributes -Wno-incompatible-pointer-types -Wno-int-conversion -Wno-pointer-sign -Wno-deprecated-non-prototype -Ibrowser/include -Iinclude

browser-collision: ## Build native queries over the original static collision mesh
	mkdir -p build/browser
	$(EMCC) -O2 -Wall -Wextra -Werror -Ibrowser/include -Iinclude \
	  browser/collision.c src/world/collision/probe_ground_height.c \
	  --no-entry -sSTANDALONE_WASM=1 -sFILESYSTEM=0 -sERROR_ON_UNDEFINED_SYMBOLS=1 \
	  -sINITIAL_MEMORY=16777216 \
	  -sEXPORTED_FUNCTIONS='["_runtime_collision_reset","_runtime_collision_stage_address","_runtime_collision_capacity","_runtime_collision_load","_runtime_collision_guards_valid","_runtime_collision_cells","_runtime_collision_faces","_runtime_collision_segment","_runtime_collision_capsule","_runtime_collision_hit_position","_runtime_collision_hit_fraction","_runtime_collision_hit_material","_runtime_collision_hit_point","_runtime_collision_hit_normal","_runtime_collision_ground_height"]' \
	  -o build/browser/collision.wasm

browser-hero: ## Build native hero locomotion/collision; compile additional frame and Novalis motion files
	mkdir -p build/browser
	$(EMCC) $(BROWSER_HERO_CFLAGS) \
	  -c src/overlays/shared/hero/update_hero_frame_pipeline.c -o build/browser/hero-frame.o
	$(EMCC) $(BROWSER_HERO_CFLAGS) \
	  -c src/overlays/l01/hero/update_hero_motion_for_current_state.c -o build/browser/hero-motion.o
	$(EMCC) $(BROWSER_HERO_CFLAGS) \
	  -c src/overlays/shared/hero/set_hero_animation.c -o build/browser/hero-animation.o
	$(EMCC) $(BROWSER_HERO_CFLAGS) \
	  -c src/overlays/shared/hero/hero_switch_item.c -o build/browser/hero-collision.o
	$(EMCC) $(BROWSER_HERO_CFLAGS) \
	  -c src/overlays/shared/hero/hero_frame_update.c -o build/browser/hero-ground.o
	$(EMCC) $(BROWSER_HERO_CFLAGS) \
	  -c src/overlays/shared/hero/hero_idle_sequence.c -o build/browser/hero-timing.o
	$(EMCC) $(BROWSER_HERO_CFLAGS) \
	  '-Dapproach_value(target,step,value)=approach_value(value,target,step)' \
	  -c src/math/interpolation/approach_value.c -o build/browser/hero-approach.o
	$(EMCC) $(BROWSER_HERO_CFLAGS) \
	  '-Dcompute_directed_angle_delta(direction,target,current)=compute_directed_angle_delta(target,direction,current)' \
	  -c src/overlays/shared/math/interpolation/compute_directed_angle_delta.c -o build/browser/hero-angle.o
	$(EMCC) $(BROWSER_HERO_CFLAGS) \
	  -c src/overlays/shared/mobys/find_moby_hit.c -o build/browser/hero-damping.o
	$(EMCC) $(BROWSER_HERO_CFLAGS) \
	  -c src/camera/build_spherical_offset.c -o build/browser/hero-spherical.o
	$(EMCC) -O2 -flto -Wall -Wextra -Werror -Ibrowser/include -Iinclude browser/hero.c browser/collision.c \
	  src/world/collision/probe_ground_height.c \
	  build/browser/hero-collision.o build/browser/hero-approach.o build/browser/hero-angle.o build/browser/hero-damping.o build/browser/hero-spherical.o build/browser/hero-timing.o \
	  --no-entry -sSTANDALONE_WASM=1 -sFILESYSTEM=0 -sERROR_ON_UNDEFINED_SYMBOLS=1 \
	  -sINITIAL_MEMORY=16777216 \
	  -sEXPORTED_FUNCTIONS='["_runtime_hero_control_reset","_runtime_hero_control_step","_runtime_hero_control_jump_count","_runtime_hero_control_jump_release","_runtime_hero_jump_animation_speed","_runtime_hero_control_crouch","_runtime_hero_control_crouched","_runtime_hero_control_slam","_runtime_hero_slam_animation_speed","_runtime_hero_control_grounded","_runtime_hero_locomotion_reset","_runtime_hero_locomotion_step","_runtime_hero_locomotion_speed","_runtime_hero_locomotion_yaw","_runtime_hero_locomotion_yaw_rate","_runtime_hero_collision_probe","_runtime_hero_collision_position","_runtime_hero_collision_wall_hit","_runtime_collision_reset","_runtime_collision_stage_address","_runtime_collision_capacity","_runtime_collision_load","_runtime_collision_guards_valid","_runtime_collision_cells","_runtime_collision_faces","_runtime_collision_segment","_runtime_collision_capsule","_runtime_collision_hit_position","_runtime_collision_hit_point","_runtime_collision_hit_normal","_runtime_collision_hit_material","_runtime_hero_locomotion_velocity"]' \
	  -o build/browser/hero-locomotion.wasm

browser-collision-test: browser-hero ## Check static capsule queries and the original hero resolver
	$(NODE) --test browser/collision.test.mjs

browser-wad: ## Build the bounded native WAD decoder separately from game startup
	@command -v $(EMCC) >/dev/null 2>&1 || { echo "Emscripten is required" >&2; exit 1; }
	@version="$$($(EMCC) --version | sed -n '1s/.* //p')"; \
	  test "$$version" = "$(EMCC_VERSION)" || { echo "Expected emcc $(EMCC_VERSION), found $$version" >&2; exit 1; }
	mkdir -p build/browser
	$(EMCC) -O2 -Wall -Wextra -Werror browser/wad.c \
	  --no-entry -sSTANDALONE_WASM=1 -sFILESYSTEM=0 -sERROR_ON_UNDEFINED_SYMBOLS=1 \
	  -sINITIAL_MEMORY=33554432 \
	  -sEXPORTED_FUNCTIONS='["_wad_reset","_wad_guards_valid","_wad_input_address","_wad_input_capacity","_wad_output_address","_wad_output_capacity","_wad_output_size","_wad_ready","_wad_decode"]' \
	  -o build/browser/wad.wasm

browser-wad-test: browser-runtime ## Test bounded WAD packets and optional combined initialization
	$(NODE) --test browser/wad.test.mjs

browser-runtime: browser-wad ## Build native state, original disc loader, and bounded archive decoder
	@command -v $(EMCC) >/dev/null 2>&1 || { echo "Emscripten is required" >&2; exit 1; }
	@version="$$($(EMCC) --version | sed -n '1s/.* //p')"; \
	  test "$$version" = "$(EMCC_VERSION)" || { echo "Expected emcc $(EMCC_VERSION), found $$version" >&2; exit 1; }
	mkdir -p build/browser
	$(EMCC) -O2 -Wall -Wextra -Werror -Wno-deprecated-non-prototype -Iinclude \
	  -c src/game/clear_scene_state_buffers.c -o build/browser/scene-reset.o
	$(EMCC) -O2 -Wall -Wextra -Werror -Wno-incompatible-pointer-types -Wno-unknown-attributes -Iinclude \
	  -c src/streaming/load_disc_sectors_into_global_buffer.c -o build/browser/disc-loader.o
	$(EMCC) -O2 -Wall -Wextra -Werror -Wno-incompatible-pointer-types -Wno-unknown-attributes -Iinclude \
	  -c src/streaming/read_file_entry_with_retry.c -o build/browser/level-header-reader.o
	$(EMCC) -O2 -Wall -Wextra -Werror -Iinclude \
	  build/browser/scene-reset.o build/browser/disc-loader.o build/browser/level-header-reader.o browser/count_vsync.c browser/runtime.c browser/disc.c \
	  src/world/loaders/relocate_sky_definition.c browser/sky.c \
	  --no-entry -sSTANDALONE_WASM=1 -sFILESYSTEM=0 -sERROR_ON_UNDEFINED_SYMBOLS=1 \
	  -sEXPORTED_FUNCTIONS='["_runtime_init","_runtime_reset_scene","_runtime_guards_valid","_runtime_buffer_address","_runtime_buffer_size","_runtime_fill","_runtime_step","_runtime_frame_count","_runtime_time","_runtime_disc_reset","_runtime_disc_guards_valid","_runtime_disc_stage_address","_runtime_disc_stage_size","_runtime_disc_table_address","_runtime_disc_table_size","_runtime_disc_load","_runtime_disc_ready","_runtime_disc_entry_sector","_runtime_disc_entry_count","_runtime_disc_service_calls","_runtime_disc_level_header_sector","_runtime_disc_level_header_load","_runtime_disc_level_header_ready","_runtime_disc_level_slot_sector","_runtime_disc_level_slot_count","_runtime_disc_music_track_sector","_runtime_sky_reset","_runtime_sky_guards_valid","_runtime_sky_stage_address","_runtime_sky_capacity","_runtime_sky_ready","_runtime_sky_load"]' \
	  -o build/browser/runtime.wasm
	cp browser/runtime.html build/browser/runtime.html
	cp browser/iso-reader.mjs browser/disc-loader.mjs build/browser/

browser-planet-wad: ## Build the WAD decoder sized for whole planet cores
	mkdir -p build/browser
	$(EMCC) -O2 -Wall -Wextra -Werror -DINPUT_LIMIT=16777216 -DOUTPUT_LIMIT=33554432 browser/wad.c \
	  --no-entry -sSTANDALONE_WASM=1 -sFILESYSTEM=0 -sERROR_ON_UNDEFINED_SYMBOLS=1 \
	  -sINITIAL_MEMORY=67108864 \
	  -sEXPORTED_FUNCTIONS='["_wad_reset","_wad_guards_valid","_wad_input_address","_wad_input_capacity","_wad_output_address","_wad_output_capacity","_wad_output_size","_wad_ready","_wad_decode"]' \
	  -o build/browser/planet-wad.wasm
browser-audio: ## Build the native PS2 ADPCM (VAG) decoder used for level music
	mkdir -p build/browser
	$(EMCC) -O2 -Wall -Wextra -Werror browser/audio.c \
	  --no-entry -sSTANDALONE_WASM=1 -sFILESYSTEM=0 -sERROR_ON_UNDEFINED_SYMBOLS=1 \
	  -sEXPORTED_FUNCTIONS='["_audio_reset","_audio_guards_valid","_audio_input_address","_audio_input_capacity","_audio_output_address","_audio_header_bytes","_audio_data_bytes","_audio_sample_rate","_audio_ended","_audio_odd_shift_blocks","_audio_begin","_audio_decode"]' \
	  -o build/browser/audio.wasm

browser-audio-test: browser-runtime browser-audio ## Test the ADPCM decoder and level music loading; set RNC_ISO_PATH for Veldin
	$(NODE) --test browser/audio.test.mjs

browser-game: browser-runtime browser-hero browser-level browser-planet-wad browser-audio ## Build the native game viewport with local Three.js
	@test -f build/browser/node_modules/three/build/three.module.js || npm install --prefix build/browser --no-save --ignore-scripts --package-lock=false three@0.180.0
	cp browser/game.html browser/game.mjs browser/gameplay-loader.mjs browser/level-actors.mjs browser/sky-geometry.mjs browser/level-loader.mjs browser/terrain-geometry.mjs browser/moby-geometry.mjs browser/moby-animation.mjs browser/music.mjs browser/interim-game.mjs browser/tie-geometry.mjs browser/shrub-geometry.mjs build/browser/
	cp browser/game.html build/browser/index.html

browser-gameplay-test: browser-runtime browser-level browser-planet-wad ## Test level header/gameplay loading and actor setup; set RNC_ISO_PATH for Veldin
	$(NODE) --test browser/gameplay-loader.test.mjs browser/level.test.mjs

BROWSER_LEVEL_SOURCES := src/world/loaders/register_moby_class.c src/mobys/assign_moby_class_data.c \
  src/rendering/mobys/prepare_resident_class_render_data.c src/rendering/packets/build_indexed_resident_render_packet.c \
  src/rendering/packets/set_up_vis_gif_viewer.c src/mobys/animation/update_moby_animation_state.c src/mobys/animation/blend_moby_animation.c \
  src/rendering/packets/pack_render_command_fields.c src/overlays/shared/ui/menus/ensure_menu_cache_entry.c \
  src/overlays/l00/level_class_tables.c src/math/random/random_integer_below.c src/math/absolute_float.c

browser-level: ## Build native level actor setup: original class registration and placed-moby initialization
	mkdir -p build/browser/level
	@set -e; for source in $(BROWSER_LEVEL_SOURCES); do \
	  $(EMCC) $(BROWSER_HERO_CFLAGS) -c $$source -o build/browser/level/$$(basename $$source .c).o; done
	@# init_moby_instance declares its 64-bit field as `unsigned long` (64-bit on the EE, 32-bit on
	@# wasm32); browser/level-moby-instance.c compiles it with the EE width instead of editing src/.
	$(EMCC) $(BROWSER_HERO_CFLAGS) -c browser/level-moby-instance.c -o build/browser/level/init_moby_instance.o
	@# sample_camera_path and the pterodactyl update compile unchanged through wrappers that resolve
	@# prototype conflicts (see the comments in each wrapper).
	$(EMCC) $(BROWSER_HERO_CFLAGS) -c browser/level-camera-path.c -o build/browser/level/sample_camera_path.o
	$(EMCC) $(BROWSER_HERO_CFLAGS) -c browser/level-pterodactyl.c -o build/browser/level/pterodactyl.o
	$(EMCC) -O2 -flto -Wall -Wextra -Werror -Ibrowser/include -Iinclude browser/level.c browser/level-unported.c browser/level-math.c browser/moby-animation-advance.c \
	  -Wl,--fatal-warnings \
	  build/browser/level/*.o \
	  --no-entry -sSTANDALONE_WASM=1 -sFILESYSTEM=0 -sERROR_ON_UNDEFINED_SYMBOLS=1 \
	  -sINITIAL_MEMORY=33554432 \
	  -sEXPORTED_FUNCTIONS='["_runtime_level_reset","_runtime_level_guards_valid","_runtime_level_header_address","_runtime_level_header_capacity","_runtime_level_core_address","_runtime_level_core_capacity","_runtime_level_gameplay_address","_runtime_level_gameplay_capacity","_runtime_level_register_classes","_runtime_level_spawn_mobys","_runtime_level_class_count","_runtime_level_class_slot","_runtime_level_class_id","_runtime_level_class_has_update","_runtime_level_class_has_data","_runtime_level_moby_count","_runtime_level_placed_count","_runtime_level_skipped_count","_runtime_level_extra_slots","_runtime_level_pending_entity_updates","_runtime_level_table_value_stores","_runtime_level_moby_address","_runtime_level_moby_stride","_runtime_level_unported_name","_runtime_level_attach_pvars_and_paths","_runtime_level_tick","_runtime_level_pvar_users","_runtime_level_pvar_bytes","_runtime_level_pending_pvar_fixups","_runtime_level_relocated_pvar_pointers","_runtime_level_path_count","_runtime_level_path_address","_runtime_level_moby_fixup_pending","_runtime_level_tick_count","_runtime_level_ticked_updates","_runtime_level_unported_update_calls","_runtime_level_animation_advances","_runtime_level_animation_sound_calls","_runtime_level_pending_bounds_refreshes"]' \
	  -o build/browser/level.wasm

browser-spike: ## Measure the whole port: compile every recovered C file to wasm objects and report missing symbols
	browser/spike/compile-all.sh
	$(NODE) browser/spike/report.mjs l00 --json build/spike/report-l00.json

# Root defaults to the Veldin objective actor, clank_cutscene_trigger_update (class 834).
SPIKE_ROOT ?= FUN_L00_002d9dc8
SPIKE_NAME ?= clank
browser-spike-mission: ## After browser-spike: list, link, and run everything one mission root reaches until a missing service traps
	$(NODE) browser/spike/closure.mjs $(SPIKE_ROOT)
	browser/spike/link-closure.sh $(SPIKE_ROOT) $(SPIKE_NAME)
	$(NODE) browser/spike/run-closure.mjs $(SPIKE_NAME) $(SPIKE_ROOT) --poke32 0x1415F8=100

browser-runtime-test: browser-runtime ## Execute the original scene and frame-counter checks
	$(NODE) --test browser/runtime.test.mjs

browser-assets-test: ## Test bounded ISO reads; set RNC_ISO_PATH for the local-disc check
	$(NODE) --test browser/iso-reader.test.mjs

browser-assets: browser-assets-test ## Copy the local-only disc inspector into build/browser
	mkdir -p build/browser
	cp browser/iso-reader.mjs browser/iso.html build/browser/

browser-smoke: ## Compile original C and a browser test into build/browser
	@command -v $(EMCC) >/dev/null 2>&1 || { echo "Emscripten is required: install it, then retry make browser-smoke" >&2; exit 1; }
	@version="$$($(EMCC) --version | sed -n '1s/.* //p')"; \
	  test "$$version" = "$(EMCC_VERSION)" || { echo "Expected emcc $(EMCC_VERSION), found $$version" >&2; exit 1; }
	mkdir -p build/browser
	$(EMCC) -O2 -Wall -Wextra -Werror -Iinclude \
	  src/math/interpolation/approach_value.c browser/smoke.c \
	  --no-entry -sSTANDALONE_WASM=1 -sFILESYSTEM=0 \
	  -sERROR_ON_UNDEFINED_SYMBOLS=1 \
	  -sEXPORTED_FUNCTIONS='["_smoke_set_value","_smoke_get_value","_smoke_step","_smoke_absolute"]' \
	  -o build/browser/approach-value.wasm
	cp browser/smoke.html build/browser/index.html

check: ## Run the public CI checks locally (tests, script parse, progress report)
	python3 scripts/test_public_tools.py -v
	python3 -m py_compile scripts/*.py scripts/lib/*.py
	python3 scripts/progress-report.py
	python3 scripts/progress-map.py
	@# Struct layout checks (src/check/*_layout_check.c), compiled with the EE compiler when present.
	@cc=tools/compilers/game-compiler; if [ -x $$cc/cc1 ]; then \
	  for f in $$(find src/check -name '*_layout_check.c' | sort); do \
	    $$cc/cpp -Iinclude $$f | $$cc/cc1 -quiet -o /dev/null || exit 1; done; \
	  echo "layout checks: OK"; else echo "layout checks: skipped (no $$cc)"; fi

format: ## Format the C sources the human-first way (scripts/format.py)
	python3 scripts/format.py

lint: ## Lint browser JavaScript and spell-check browser comments and maintained docs
	npm run lint

progress: ## After `make elf`: measure C_FUZZY and preview the report and map in build/progress/
	.venv/bin/python scripts/progress-report.py --workspace build/baseline
	.venv/bin/python scripts/progress-map.py

elf: ## Rebuild the boot ELF byte-for-byte (full baseline + SHA gate)
	scripts/verify-baseline.sh
	@ls -l build/baseline/config/us/build/SCUS_971.99

overlays: ## Build src/overlays and prove every overlay function in C against retail (docs/build/overlays.md)
	.venv/bin/python configure.py --overlays
	.venv/bin/python scripts/overlay-check.py

iso: elf ## Patch the rebuilt boot ELF into a copy of the disc image
	@iso="$$(ls dumps/*.iso 2>/dev/null | head -1)"; \
	test -n "$$iso" || { echo "no dumps/*.iso found; place a legally owned dump in dumps/" >&2; exit 1; }; \
	python3 scripts/rebuild-iso.py --iso "$$iso"

clean-iso:
	@rm -f "build/Ratchet & Clank (USA) - rebuilt.iso"
