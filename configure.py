#! /usr/bin/env python3
"""
Configures the project for building. Invokes splat to split the target ELF and
creates build files for ninja.

Run from the workspace root (the repository checkout, or the isolated
staging root that verify-baseline.sh prepares for the 32-bit EE
compiler).
"""

from __future__ import annotations

import argparse
import contextlib
import copy
import json
import os
import re
import shlex
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any, List, Set, Union, cast

import ninja_syntax
import yaml

import splat
import splat.scripts.split as split
from splat.segtypes.linker_entry import LinkerEntry
from splat.util.conf import load as splat_load_yaml

ROOT = Path.cwd()

COMPILER = "ee-gcc2.9-991111-01"
CROSS = "mips-ps2-decompals-"
COMPILER_FLAGS = "-DMATCHING_DECOMP -O2 -g2 -gstabs"
LANG_DEFINE = "-DBUILD_US_VERSION"

# Units whose retail code was compiled with the EE-GCC 2.9 pin (sd/ld) instead of
# the SN textbin compiler. DEFAULT: every textbin unit uses SN
# (ee-gcc2.9-991111b/r4, emits sq/lq), which matches the game textbin range;
# libc/SDK/core units stay on EE-GCC 2.9 until their own profile is identified.
# SDK_COMPILER_UNITS is the per-unit exception list for textbin.
SN_TOOLCHAIN_ROOT = os.environ.get("SN_TOOLCHAIN_ROOT", "").strip()
# The routing and flag tables below use unit paths from rnc1.us.yaml.  When a
# source unit is renamed or moved, carry its compiler/profile entries forward.
# Locally built patched public 991111 cc1 (R5900 quadword saves + classic
# mult/mflo).  Not vendored: the environment points at the pinned build and the
# tree falls back to the frozen compilers when it is absent.  The source patch
# and its build script live in patches/ee-gcc-2.9-991111-01/; see
# docs/patched-toolchain.md.
EE_GCC_PATCHED_ROOT = os.environ.get("EE_GCC_PATCHED_ROOT", "").strip()
# Reconstructed Sony/Cygnus 2.9-ee-991111b game compiler (RncDecomp-tools
# patch stack through P46, cc1 eb7a3497c39e0f73fc4f01bcda7de426c071c8f2f9d4702ffecefb5a6e1c8743).
# Native Linux toolchain; units listed in
# GAME_COMPILER_UNITS are built by it instead of the SN binary.
GAME_COMPILER_ROOT = os.environ.get("GAME_COMPILER_ROOT", "").strip()
# Promoted textbin units matched byte-exact under the SN compiler.
SN_COMPILER_UNITS = {
    # fun_00233980 is a save-less leaf, so the sq/lq fingerprint classifies it
    # as "none" and sends it to EE-GCC 2.9, which hoists the bump-pointer load
    # and diverges from retail. Fresh SN -O2 -g2 -mno-split-addresses reproduces
    # the retail reload schedule exactly (100/100/100/100 direct objdiff and
    # linked-byte comparison against the retail ELF).
    "rendering/vu1_add_g_sregister",
    # fun_002172c0 has no callee saves (retail style "none"), so the sq/lq
    # fingerprint cannot classify it; retail branch-delay scheduling matches
    # SN exactly (fresh SN -O2 -g2 object = 100% four-way; EE-GCC 2.9 = 95.2%
    # order-only nop/addiu swap). See the private evidence archive for details.
    "input/pad/clear_pad_input",
    # Save-less counting loops (retail style "none"): fresh SN -O2 -g2 objects
    # are 100/100/100; the EE-GCC 2.9 fallback ties at 59.7-91.1% (pipeline-2026-09-13-11
    # wave 3, fun_00215300 / fun_00215348 / fun_00215290).
    "ui/menus/count_nonzero_entries_up_to_10",
    "ui/menus/count_nonzero_entries_up_to_30",
    "ui/menus/count_nonzero_entries_up_to_40",
    # Save-less leaf (retail style "none", 19 instructions, no frame): fresh
    # SN -O2 -g2 is 100/100/100; the EE-GCC 2.9 fallback stages at 91.11%.
    "video/decoder/buffers/read_buf_begin_get",
    # Save-less leaf (retail style "none", 27 instructions, no frame): fresh
    # SN -O2 -g2 is 100/100/100 with the numeric D_0015F6A0 pointer load and the
    # v1/a3 pointer roles; the EE-GCC 2.9 fingerprint would route it to EE-GCC 2.9.
    "ui/help/find_help_message_index",
    "ui/text/measure_text_width_regular",
    "ui/text/measure_text_width_small",
    "ui/text/measure_text_width_large",
    "ui/menus/hit_test_fixed_screen_rectangle",
    "audio/decoder/audio_dec_reset",
    "audio/streaming/start_audio_stream_read",
    "rendering/entities/draw_moby_entries_from_object",
    "audio/streaming/snd_stop_all_streams",
    "audio/movie/snd_reset_movie_sound",
    "audio/movie/snd_get_movie_nax",
    "gameplay/camera/backup_current_cam",
    "rendering/get_occlusion_grid_from_pair",
    "rendering/update_occlusion",
    "ui/text/font_print_large",
    "ui/text/font_print_small",
    "ui/text/font_print_right",
    "ui/text/font_print_right_small",
    "ui/text/font_print_right_large",
    "textbin/fun_001f6fd0",
    "ui/text/font_print_window_regular",
    "ui/text/font_print_window_small",
    "textbin/fun_001f7978",
    "ui/frames/draw_stretchable_ui_frame",
    "rendering/refresh_point_light",
    "gameplay/entities/init_moby_class_dists",
    "gameplay/entities/stash_moby_class_dists",
    "gameplay/entities/restore_moby_class_dists",
    "math/random/random_integer_below",
    "ui/menus/compute_clamped_count_difference",
    "audio/streaming/register_audio_stream_callback",
    "input/pad/update_primary_pad_state",
    "textbin/fun_0021cae0",
    "textbin/fun_0021df58",
    "textbin/fun_0021eaf0",
    "ui/map/draw_map_screen_overlay",
    "ui/menus/missions/draw_mission_menu_labels",
    "textbin/fun_00221930",
    "textbin/fun_00221a88",
    "textbin/fun_00222f18",
    "textbin/fun_00222f58",
    "gameplay/state/clear_scene_state_buffers",
    "audio/banks/load_audio_bank_by_location",
    "rendering/buffers/copy_render_buffer_pair",
    "rendering/entities/register_entity_render_resources",
    "textbin/fun_00239750",
    "runtime/threads/switch_thread",
    "audio/decoder/audio_dec_delete",
    "audio/decoder/audio_dec_send",
    "video/decoder/vi_buf_end_put",
    "video/decoder/vi_buf_count",
    "video/decoder/video_dec_set_stream",
    "video/decoder/video_dec_begin_put",
    "video/decoder/video_dec_end_put",
    "video/decoder/video_dec_delete",
    "video/decoder/video_dec_input_count",
    "video/decoder/vo_buf_get_data",
    # Pure-C promotions from src/assembly/textbin: fully compiled under SN and
    # gate-verified byte-identical; listed by full unit name because
    # _unit_uses_sn() deliberately excludes the assembly/textbin wrapper units
    # (their INCLUDE_ASM oracles only assemble under the native EE-GCC 2.9).
    "audio/decoder/audio_dec_begin_put",
    "textbin/fun_002220f0",
    "ui/text/update_scrolling_status_message",
    "assembly/runtime/dma/clear_dma_queue_entry",
    "runtime/dma/clear_dma_queue_entry",
    # AttachManipulator is a save-less leaf (no sq/lq fingerprint), so the
    # retail-save-style routing would send it to EE-GCC 2.9; SN -O2 reproduces
    # the retail schedule byte-exactly.
    "gameplay/entities/attach_manipulator",
    # InitializeRenderState: the whole unit is 13 SN-style instructions; fresh
    # SN -O2 with s16 fields at 0x40/0x42/0x5C/0x5E/0x78/0x7A and the retail
    # store order is 100/100/100 direct and patha linked-byte equal
    # (run-14 mass-d; EE-GCC 2.9 stages at 76.00).  The unit is a
    # non-textbin prefix, so the per-unit set is the only routing hook.
    "rendering/state/initialize_render_state",
    # vu1_sync_chain: wait for DMA channels in a mask to go idle, report a
    # timeout after 100000 spins
    "textbin/rendering/vu1_sync_chain",
    # fun_001f0bd0: queue a debug text entry and sprintf it into the text pool
    "textbin/fun_001f0bd0",
    # snd_send_current_batch: send the current sound command batch over SIF RPC
    # and flip to the other buffer
    "textbin/audio/rpc/snd_send_current_batch",
    # fun_00226670: do-while over a signed s32 byte cursor (retail guards at the
    # bottom with a signed slt) plus two address forms for one symbol
    "textbin/fun_00226670",
    # fun_00208030: expand a 4bpp coverage map through the 16-entry weight table
    # into a 1bpp threshold mask (4 source rows per output row)
    "textbin/fun_00208030",
    # fun_001f2070: project the camera-relative point through the view matrix
    # into GS screen coordinates and depth
    "textbin/fun_001f2070",
}

# C units relocated from src/textbin/<module> to semantic source roots.
# This preserves the old textbin save-style compiler rule without encoding
# provenance in the public source path.
TEXTBIN_ORIGIN_UNITS = {
    "audio/banks/load_audio_bank_by_location",
    "audio/banks/snd_resolve_bank_xrefs",
    "audio/banks/snd_unload_bank",
    "audio/decoder/audio_dec_begin_put",
    "audio/decoder/audio_dec_create",
    "audio/decoder/audio_dec_delete",
    "audio/decoder/audio_dec_reset",
    "audio/decoder/audio_dec_send",
    "audio/decoder/audio_dec_start",
    "audio/decoder/is_audio_ok",
    "audio/decoder/log_audio_error",
    "audio/decoder/process_audio_stream",
    "audio/decoder/send_to_spu",
    "audio/decoder/terminate_audio_system",
    "audio/effects/snd_auto_reverb",
    "audio/effects/snd_pre_alloc_reverb_work_area",
    "audio/effects/snd_set_reverb_ex",
    "audio/load",
    "audio/mixer/snd_set_group_voice_range",
    "audio/mixer/snd_set_master_volume",
    "audio/mixer/snd_set_mixer_mode",
    "audio/mixer/snd_set_playback_mode",
    "audio/movie/snd_close_movie_sound",
    "audio/movie/snd_get_movie_nax",
    "audio/movie/snd_init_movie_sound",
    "audio/movie/snd_reset_movie_sound",
    "audio/movie/snd_start_movie_sound",
    "audio/movie/snd_update_movie_adpcm",
    "audio/music/music_stop",
    "audio/rpc/snd_post_message",
    "audio/rpc/snd_reset_state_and_flush_commands",
    "audio/snd_got_returns",
    "audio/streaming/advance_audio_stream_state",
    "audio/streaming/clear_record_flag_by_key",
    "audio/streaming/complete_stream_buffer_transfer",
    "audio/streaming/continue_audio_stream_if_ready",
    "audio/streaming/finish_audio_stream_read",
    "audio/streaming/get_stream_buffer_size",
    "audio/streaming/register_audio_stream_callback",
    "audio/streaming/request_audio_stream_break",
    "audio/streaming/select_next_stream_buffer",
    "audio/streaming/snd_continue_vag_stream",
    "audio/streaming/snd_get_vag_stream_time_remaining_cb",
    "audio/streaming/snd_init_vag_streaming_ex",
    "audio/streaming/snd_is_vag_stream_buffered_cb",
    "audio/streaming/snd_pause_vag_stream",
    "audio/streaming/snd_stop_all_streams",
    "audio/streaming/snd_stream_safe_cd_break",
    "audio/streaming/snd_stream_safe_cd_callback",
    "audio/streaming/snd_stream_safe_cd_get_error",
    "audio/streaming/snd_stream_safe_cd_read",
    "audio/streaming/snd_stream_safe_cd_sync",
    "audio/streaming/snd_stream_safe_check_cd_idle",
    "audio/streaming/start_audio_stream_read",
    "audio/streaming/update_audio_stream_until_idle",
    "audio/voices/allocate_voice_for_bank_entry",
    "audio/voices/allocate_voice_for_group_entry",
    "audio/voices/release_voice_slot",
    "audio/voices/set_sound_handle_id",
    "audio/voices/snd_continue_all_sounds_in_group",
    "audio/voices/snd_pause_all_sounds_in_group",
    "audio/voices/snd_play_sound_vol_pan_pmpb",
    "audio/voices/snd_set_sound_params_cb",
    "audio/voices/snd_sound_is_still_playing_cb",
    "audio/voices/snd_stop_all_sounds",
    "audio/voices/snd_stop_sound",
    "gameplay/animation/apply_pending_animation",
    "gameplay/animation/find_animation_definition_index",
    "gameplay/animation/find_valid_animation_frame_index",
    "gameplay/animation/load_animation_definition",
    "gameplay/animation/queue_animation_update",
    "gameplay/animation/set_animation_parameter",
    "gameplay/animation/update_moby_animation_state",
    "gameplay/callbacks/dispatch_callback_list_1",
    "gameplay/callbacks/dispatch_callback_list_2",
    "gameplay/callbacks/dispatch_callback_list_3",
    "gameplay/callbacks/dispatch_callback_list_4",
    "gameplay/callbacks/enqueue_callback_list_1",
    "gameplay/callbacks/enqueue_callback_list_4",
    "gameplay/callbacks/reset_callback_registries",
    "gameplay/camera/backup_current_cam",
    "gameplay/camera/build_spherical_offset",
    "gameplay/camera/execute_camera_post_update_callbacks",
    "gameplay/entities/attach_manipulator",
    "gameplay/entities/draw_moby_list",
    "gameplay/entities/draw_mobys",
    "gameplay/entities/draw_mobys_clean_up",
    "gameplay/entities/draw_mobys_setup",
    "gameplay/entities/find_or_allocate_id_slot",
    "gameplay/entities/init_moby_class_dists",
    "gameplay/entities/mark_moby_for_removal",
    "gameplay/entities/process_moby_anim_data",
    "gameplay/entities/restore_moby_class_dists",
    "gameplay/entities/stash_moby_class_dists",
    "gameplay/entities/update_moby",
    "gameplay/state/clear_scene_state_buffers",
    "gameplay/state/compute_interpolated_record_value",
    "gameplay/state/dispatch_game_state_update",
    "gameplay/state/is_active_state_entry",
    "gameplay/state/is_value_within_interpolated_window",
    "gameplay/state/process_global_state_flags",
    "gameplay/state/run_game_main_loop",
    "gameplay/state/transition_default_draw",
    "gameplay/state/transition_draw_sky",
    "input/pad/clear_pad_input",
    "input/pad/update_primary_pad_state",
    "math/conversion/round_float_to_decimal_places",
    "math/conversion/truncate_float_to_s32",
    "math/random/random_angle_radians",
    "math/random/random_float_between",
    "math/random/random_integer_below",
    "math/rotations/build_quaternion_from_axis_angle",
    "math/vectors/normalize_vector_triplet",
    "rendering/buffers/copy_render_buffer_pair",
    "rendering/buffers/swap_render_buffer_chain",
    "rendering/culling/is_point_inside_clip_volume",
    "rendering/debug/prepare_debug_profiler_render",
    "rendering/detach_point_light",
    "rendering/dma/start_vif1_dma_transfer",
    "rendering/dmac_vif1_disable",
    "rendering/dmac_vif1_enable",
    "rendering/draw_shrubs",
    "rendering/draw_ties_1",
    "rendering/draw_two_texture_panels",
    "rendering/effects/draw_fogged_fullscreen_sprite",
    "rendering/entities/draw_moby",
    "rendering/entities/draw_moby_entries_from_object",
    "rendering/entities/register_entity_render_resources",
    "rendering/geometry/transform_scaled_vertex_batch",
    "rendering/get_occlusion_grid_from_pair",
    "rendering/packets/append_draw_buffer_packet",
    "rendering/packets/append_draw_environment_packet",
    "rendering/packets/append_gif_transfer_packet",
    "rendering/packets/emit_rgba_draw_packet",
    "rendering/packets/put_disp_buffer",
    "rendering/packets/write_vif_unpack_packet",
    "rendering/refresh_point_light",
    "rendering/sky/sky_draw_shell",
    "rendering/texture/append_palette_transfer_packet",
    "rendering/texture/append_texture_transfer_packet",
    "rendering/texture/compose_bitmap_from_mask",
    "rendering/texture/initialize_alpha_lookup_table",
    "rendering/texture/patch_tie_texture_fields",
    "rendering/transitions/draw_transition_overlay",
    "rendering/update_occlusion",
    "rendering/vu1_add_data_ref",
    "rendering/vu1_add_g_sregister",
    "rendering/vu1_gs_regs_normal",
    "rendering/vu1_tex_flush",
    "runtime/cache/synchronize_cache_range",
    "runtime/debug/debug_print_stub",
    "runtime/diagnostics/print_register_values_and_halt",
    "runtime/dma/disable_dmac",
    "runtime/dma/enable_dmac",
    "runtime/interrupts/disable_intc",
    "runtime/interrupts/enable_intc",
    "runtime/memory/calculate_ring_buffer_bounds",
    "runtime/memory/reserve_aligned_buffer_space",
    "runtime/newlib/reentrant_read",
    "runtime/newlib/reentrant_syscall_with_three_arguments",
    "runtime/newlib/reentrant_syscall_with_two_arguments",
    "runtime/newlib/reentrant_write",
    "runtime/relocation/relocate_asset_entry_pointers",
    "runtime/rpc/free_sif_system_memory",
    "runtime/rpc/get_available_rpc_packet",
    "runtime/rpc/initialize_sif_rpc",
    "runtime/startup/run_global_constructors",
    "runtime/threads/switch_thread",
    "runtime/time/decode_bcd_time_fields",
    "runtime/time/encode_bcd_time_fields",
    "storage/cd/handle_cd_read_callback",
    "storage/cd/load_disc_sectors_into_global_buffer",
    "storage/cd/read_file_entry_with_retry",
    "storage/memory_card/data/calculate_crc16",
    "storage/memory_card/data/validate_data_crc",
    "storage/memory_card/memcard_initialize",
    "storage/memory_card/memcard_make_whole_save",
    "storage/wad/wad_get_sectors",
    "ui/fonts/load_debug_font",
    "ui/frames/draw_stretchable_ui_frame",
    "ui/help/find_help_message_index",
    "ui/help/force_help_message",
    "ui/help/get_help_message_text",
    "ui/help/help_draw_prompt",
    "ui/help/try_set_help_message",
    "ui/hud/hud_heap_alloc",
    "ui/hud/load_compressed_hud_bank",
    "ui/map/draw_map_screen_overlay",
    "ui/map/find_map_entry_slot",
    "ui/map/move_map_entry_slot",
    "ui/map/promote_first_available_map_entry",
    "ui/menus/compute_clamped_count_difference",
    "ui/menus/count_nonzero_entries_up_to_10",
    "ui/menus/count_nonzero_entries_up_to_30",
    "ui/menus/count_nonzero_entries_up_to_40",
    "ui/menus/draw_menu_selection_marker",
    "ui/menus/draw_quit_game_menu",
    "ui/menus/format_scaled_display_value",
    "ui/menus/hit_test_fixed_screen_rectangle",
    "ui/menus/missions/draw_mission_menu_labels",
    "ui/text/copy_text_to_shared_buffer",
    "ui/text/font_print_center",
    "ui/text/font_print_center_large",
    "ui/text/font_print_center_small",
    "ui/text/font_print_large",
    "ui/text/font_print_right",
    "ui/text/font_print_right_large",
    "ui/text/font_print_right_small",
    "ui/text/font_print_small",
    "ui/text/font_print_window_regular",
    "ui/text/font_print_window_small",
    "ui/text/measure_text_width",
    "ui/text/measure_text_width_large",
    "ui/text/measure_text_width_regular",
    "ui/text/measure_text_width_small",
    "ui/text/set_scrolling_status_message",
    "ui/text/update_scrolling_status_message",
    "video/decoder/buffers/get_fifo_index",
    "video/decoder/buffers/read_buf_begin_get",
    "video/decoder/callbacks/get_mpeg_timestamp",
    "video/decoder/callbacks/handle_end_image",
    "video/decoder/callbacks/handle_mpeg_error",
    "video/decoder/callbacks/handle_mpeg_no_data",
    "video/decoder/dma/set_dma_channel_3_control_register",
    "video/decoder/dma/set_dma_channel_4_control_register",
    "video/decoder/run_video_decoder",
    "video/decoder/vi_buf_begin_put",
    "video/decoder/vi_buf_count",
    "video/decoder/vi_buf_delete",
    "video/decoder/vi_buf_end_put",
    "video/decoder/vi_buf_flush",
    "video/decoder/video_dec_begin_put",
    "video/decoder/video_dec_create",
    "video/decoder/video_dec_delete",
    "video/decoder/video_dec_end_put",
    "video/decoder/video_dec_flush",
    "video/decoder/video_dec_input_count",
    "video/decoder/video_dec_is_flushed",
    "video/decoder/video_dec_put_ts",
    "video/decoder/video_dec_set_stream",
    "video/decoder/vo_buf_get_data",
    "video/decoder/vo_buf_get_tag",
    "video/display/wait_for_display_vsync",
    "world/data/parse_typed_resource_record",
    "world/parse_space_scene_chunk",
    "world/streaming/initialize_dma_packet_payloads",
    "world/streaming/load_and_initialize_level_chunk",
    "world/streaming/load_level_chunk_from_disc",
}

# Units proven byte-exact under the patched 991111 build.  Keep the set
# explicit: this compiler is a per-unit profile, not a replacement for the
# frozen SN/EE-GCC 2.9 trees (its SN-class controls do not reach 100).
EE_GCC_PATCHED_UNITS = {
    # Retail uses classic mult/mflo; the frozen trees emit the R5900 rd-form.
    # 100/100/100 + patha linked-byte equal (0x12D3A0), 2026-09-12.
    "sdk/time/bcd_to_time",
    # Retail keeps the ORed value in v0 and the 0x15 constant in v1; the
    # patched profile plus a v0 register pin reproduces all 30 instructions
    # (98.67 without the pin). patha linked-byte equal, 2026-09-12.
    "textbin/fun_00208f28",
    # lq/sq-save class exacts recovered by the extended pool sweep; promoted
    # as a batch (patha byte-equal each; batch2 gate 2026-09-12).
    "runtime/state/read_state_field",
    "runtime/memory/calculate_ring_buffer_bounds",
    "math/random/random_angle_radians",
    "video/decoder/callbacks/handle_mpeg_error",
    # _pictureCodingExtension: absolute IPU_CTRL volatile stores must fill the
    # _nextBit call delay slots; the patched profile splits the AT macro and the
    # at-store policy brackets it with .set noat. 100/100/100, gate 2026-09-13.
    "sdk/library/picturecodingextension",
    # _lastFrame: retail keeps two independent count-1 computations in the
    # _dispRefImage argument setup.  The v3 patched profile blocks the CSE and
    # reload-CSE folds and reverses load_register_parameters; 100/100/100 and
    # full-ELF gate 2026-09-13.
    "sdk/library/_lastFrame",
    # Run-12 campaign 12b: retail's absolute global access and word stores in
    # these textbin tails are reproduced only by the patched profile (frozen
    # SN emits gp-relative access and byte stores); 100/100/100 direct and
    # patha linked-byte equal 2026-09-13.
    "gameplay/callbacks/enqueue_callback_list_1",
    "gameplay/callbacks/enqueue_callback_list_4",
    "textbin/fun_00226e08",
    # fun_002133d0 (run-14 worker b): retail materializes the float constants
    # pi/1.0/0.5 with lui/ori/mtc1 and carries the FP hazard NOPs; the frozen
    # profiles emit .lit4 loads and drop the NOPs. The patched profile plus the
    # permuter shape is 100/100/100 and patha linked-byte equal, 2026-09-14.
    "textbin/fun_002133d0",
    # fun_00206e18: the patched profile preserves the retail FP hazard NOP
    # and, with the descriptive-C v1 result barrier, the final result copy.
    "textbin/fun_00206e18",
    # truncate_float_to_s32: retail converts the float argument in place
    # (cvt.w.s $f12,$f12); the v4 patched profile emits that form under
    # -mastra-inplace-cvt.
    "math/conversion/truncate_float_to_s32",
    # Promoted by the decomp workbench: exact only under the patched
    # 991111 profile (fresh SN/EE-GCC 2.9 measurements are lower).
    "textbin/fun_0022c6f8",
    # Promoted by the decomp workbench: exact only under the patched
    # 991111 profile (fresh SN/EE-GCC 2.9 measurements are lower).
    "textbin/fun_00226848",
    # get_mpeg_timestamp: 16-byte result read through a 0x18-byte stack buffer
    "video/decoder/callbacks/get_mpeg_timestamp",
    # load_irx_module: Four-word SIF DMA transfer record and wait/execute
    # sequence match the patched route.
    "textbin/unclassified/load_irx_module",
    # fun_001fecc8: find an id in the 150-entry pair table and return its
    # partner
    "textbin/fun_001fecc8",
    # sce_dma_put_env: libdma sceDmaPutEnv: validate the env, program
    # D_CTRL/PCR/SQWC/RBOR/RBSR and keep a copy
    "sdk/dma/sce_dma_put_env",
}

# Per-unit extra flags for the patched 991111 profile.  Every -mastra-* option
# is opt-in and absent by default; flag-absent output is byte-identical.
EE_GCC_PATCHED_FLAG_UNITS = {
    "picturecodingextension": "-mastra-volatile-delay -mastra-sd-saves",
    "textbin/fun_00226848": "-mastra-no-lo-sum-tie",
    "_lastFrame": "-mastra-sd-saves -mastra-cse-argdup -mastra-call-args-reverse",
    "math/conversion/truncate_float_to_s32": "-mastra-inplace-cvt",
}

# Per-unit assembler policies applied by the generated padless-asm.py helper.
PADLESS_POLICY_UNITS = {
    "picturecodingextension": "at-store",
    # Ps2EeAs is single-pass and cc1 emits `.extern NAME, SIZE` at end of file:
    # for fun_0022f778 the la-only small-data symbol D_00160510 stayed a
    # lui+addiu pair; hoisting its directive yields retail's single
    # `addiu $3,$gp,-0x66f0` (86.117 -> 86.258 on the padless route).  The
    # route entry itself is added only once the unit stops being a pending
    # INCLUDE_ASM wrapper (the SN driver -S stage cannot expand the oracle's
    # `.include`, so a pending unit must keep the native EE-GCC 2.9 route).
    "fun_0022f778": "la-gprel",
}

SDK_COMPILER_UNITS = {
    # e.g. "textbin/fun_XXXX" when a textbin unit is proven EE-GCC-2.9-compiled.
    # fun_00124a20: empty varargs DPRINT, as Sony libdbc.o
    "runtime/debug/debug_print_stub",
    # fun_0011c938: array externs keep lui/lw addressing, as Sony libkernl
    # iopheap.o
    "runtime/rpc/free_sif_system_memory",
    # fun_001206d8: volatile cdvd statics keep loads out of delay slots, as Sony
    # libcdvd cdvd000.o
    "storage/cd/handle_cd_read_callback",
    # fun_0011ace8: rpc packet scan with the pid wrap written as if/else, as
    # Sony libkernl sifrpc.o
    "runtime/rpc/get_available_rpc_packet",
    # newlib reentrant wrappers (errno cleared, copied into the reent block on
    # -1); retail saves with sd/ld, so the SDK compiler, not SN.
    "runtime/newlib/reentrant_syscall_with_two_arguments",
    "runtime/newlib/reentrant_syscall_with_three_arguments",
    "runtime/newlib/reentrant_read",  # _read_r
    "runtime/newlib/reentrant_write",  # _write_r
    # fun_0011dc18: libgcc __do_global_ctors (gbl-ctors.h DO_GLOBAL_CTORS_BODY),
    # SDK compiler
    "runtime/startup/run_global_constructors",
    # Promoted by the decomp workbench: measured exact on the sony-2.9 line
    # (cc), so the full build must not fall back
    # to the default route.
    "sdk/library/cpr8",
    # fun_00116da8: newlib vfprintf: CHECK_INIT then _vfprintf_r
    "textbin/fun_00116da8",
    # reset_gs_registers_pr: reset the privileged GS display registers to the
    # stored mode
    "textbin/rendering/state/reset_gs_registers_pr",
    # fun_00120a28: the table walk matches when the cursor is a named local and
    # the two 8-byte globals are read through their own struct wrappers
    "textbin/fun_00120a28",
}

# Recovered C units that own the small .rodata retail kept inside the
# preserved `core_rdata` blob.  Key: configured unit-name suffix; value:
# (retail VMA, retail file offset) of the unit's compiled `.rodata` bytes.
# apply_retail_link_layout emits an overlay section for any configured `c`
# unit matching the suffix, so both the assembly-backed and the promoted
# (normalized) unit names resolve to the same retail bytes.
RODATA_OVERLAYS = {
    "_dtoa_r": (0x152330, 0x532B0),
    # _getpic's switch emits a 5-entry jump table (0x14 bytes) that retail
    # stored at 0x153AA0 inside core_rdata; the expected object references it
    # as the splat symbol jtbl_00153AA0, so the compiled .rodata must land at
    # the same VMA/file offset for the relocations to resolve content-equal.
    "_getpic": (0x153AA0, 0x54A20),
    "dispatch_game_state_update": (0x1E8960, 0xE98E0),  # retail switch table
    "fun_0020baf0": (0x1E8390, 0xE9310),  # unlock-condition switch table
    "fun_0021ddf8": (0x1E87A0, 0xE9720),  # item-handle release switch table
}

# Per-unit extra compiler flags for the native EE-GCC 2.9 units whose
# exact codegen requires a different scheduling model.  Keyed by unit-name
# suffix so both the assembly-backed and the normalized/promoted name match.
# sce_sif_init_iop_heap: retail tail (lui v0; sw; move v0) is byte-exact only
# under -fno-schedule-insns; applying it globally to all EE-GCC 2.9 units changes
# scePad2Read and other already-exact siblings.
EE_GCC_FLAG_UNITS = {
    "sce_sif_init_iop_heap": "-fno-schedule-insns",
    # Absolute-store macros and the final GP store's delay-slot placement.
    "initialize_global_state_entry": "-mno-split-addresses -fno-schedule-insns",
    # Retail writes the absolute global through the assembler `$at` macro
    # (`lui $1,%hi; sw ...,%lo($1)`); the default split-address sequence uses a
    # general register instead.  Validated 100/100/100 under EE-GCC 2.9 + flag.
    "clear_stage_state_flag": "-mno-split-addresses",
    "initialize_streaming_state": "-mno-split-addresses",
    # DIntr: Sony libkernel privileged-loop glue.  The ps2sdk glue.c shape
    # (pinned eie/next/res + `.p2align 3`) matches retail only under the size
    # optimization with the missing-cse-follow-jumps policy; the default
    # -O2 compile picks `daddu a0,v1` for the out arm instead of $zero and
    # schedules the return move out of the jr delay slot.  100/100/100 under
    # EE-GCC 2.9 with this flag pair (campaign pipeline-2026-09-11-7).
    "DIntr": "-Os -fno-cse-follow-jumps",
    # __swrite: retail's field layout is u16@0xC + s16@0xE (not s32@0xE, which
    # the compiler pads to 0x10) and the s64 return is the dsll32/dsra32
    # sign-extension pair, which the local compiler only emits when the s32
    # result is forced through an s64 local + (u32) truncation.  Exact under
    # -Os -fno-cse-follow-jumps (pipeline-2026-09-13-11).
    "__swrite": "-Os",
    # AppendDmaTag: retail folds the non-small global's absolute load as
    # `lui v0,%hi; lw v0,%lo(v0)` and the absolute store through the $at macro;
    # -G0 + -mno-split-addresses reproduces that (100/100/100/100 + patha,
    # run-12 wave-2 campaign 12f).
    "append_dma_tag": "-G0 -mno-split-addresses",
    # cmd_sem_init: retail stores the first CreateSema result in call 2's
    # delay slot.  Under -fno-schedule-insns the E8 store is issued before
    # call 2's `a0 = sp`, so the daddu takes the slot; the empty
    # `asm("" : "+r"(r1))` one-cycle edge delays the E8 store so reorg fills
    # the call-2 slot instead (pipeline-2026-09-13-12g).
    "cmd_sem_init": "-fno-schedule-insns",
    # No -fno-edge-lcm entry remains: the six that did (draw_debug_profiler,
    # fun_0022f778, draw_dialog_text, memcard_update_state, sound_update,
    # setup_fs_aa_buffer) belong to units that are still assembly wrappers,
    # where the option cannot change a byte. Those units build on the 991111-01
    # route, where -fno-edge-lcm is a real requirement for their C once a body
    # replaces the wrapper: re-add it then, with the measurement and the reason
    # recorded in this comment.
}


# Per-unit extra compiler flags for SN-routed textbin units whose exact
# codegen requires a non-default option.  fun_00225530 retail loads the global
# with a non-split address sequence (`lui v1,%hi; lw v1,%lo(v1)`), which the SN
# driver only reproduces with -mno-split-addresses; the default emits a split
# base register and scores 87.65% instead of 100%.
# textbin units verified byte-exact under the reconstructed game compiler
# (objdiff code=100 with the flags below; the first two are also patha
# link-verified).  Both this compiler and SN reproduce these retail objects
# only with the per-unit flags, i.e. the original build used them.
GAME_COMPILER_UNITS = {
    # 2026-09-25: these five were promoted exact on a non-default route, so
    # the baseline build compiled them with the default cc_sn and the linked
    # ELF differed from retail in exactly 336 bytes, all inside them:
    #   fun_001fee88 164 B  fun_00208770 95 B  fun_00215390 40 B
    #   fun_0022ea08 98 B  fun_00239690  6 B
    # The route lives in the workbench registry; without the entry here the
    # per-unit gate and the full build measure different compilers.
    "textbin/fun_001fee88",
    "textbin/fun_00215390",

    # fun_0021fc68: The final helper arguments are 64-bit; reading
    # D_001A00F0 + 0x258 preserves the retail 0x001A0348 ld/sd call setup.
    "rendering/transitions/draw_transition_overlay",
    # fun_00214128: The 3x3 matrix pass uses a 16-byte scratch record, zeroes
    # its pad lane, and passes a true 1.0f to the vector helper.
    "math/vectors/normalize_vector_triplet",
    # draw_shrubs: Direct byte-pointer update preserves retail ordering;
    # volatile D_0015EE74 keeps its write before WriteDmaChannel.
    "rendering/draw_shrubs",
    # fun_00225cd8: The promoted helper is void with one ignored argument; a
    # separate table-base pointer plus byte offset reproduces the retail loop
    # and address setup.
    "audio/streaming/complete_stream_buffer_transfer",
    # fun_00233c90: Builds the 0x3000000B / 0x5000000B DMA tag for D_0013CF10
    # and advances the 0x10-byte cursor.
    "textbin/fun_00233c90",
    # fun_00233c28: Builds the 0x30000003 / 0x50000003 DMA tag for D_001DE3F0
    # and advances the 0x10-byte cursor.
    "textbin/fun_00233c28",
    # fun_00233bc8: Builds the 0x30000003 / 0x50000003 DMA tag for D_001DE3C0
    # and advances the 0x10-byte cursor.
    "rendering/vu1_gs_regs_normal",
    # fun_00233b68: Builds the 0x30000003 / 0x50000003 DMA tag for D_001DEE00
    # and advances the 0x10-byte cursor.
    "rendering/vu1_tex_flush",
    # fun_00233830: The register-bound tag writes and cursor increment match
    # retail; the $gp-relative cursor address resolves to D_00160F00.
    "rendering/vu1_add_data_ref",
    # fun_00233938: Register-bound stores reproduce the DMA tag and preserve the
    # four retail pointer reloads; the final cursor store uses D_00160F00 = $gp
    # - 0x5D00.
    "textbin/fun_00233938",
    # fun_001eb410: A register-constrained page-relative read reproduces
    # retail's D_0018A2E8 lui/lw pair.
    "gameplay/state/transition_default_draw",
    # fun_0021d1f8: A fixed local table pointer and explicit unsigned threshold
    # test reproduce the four-record scan and helper result branch.
    "textbin/fun_0021d1f8",
    # fun_00225d88: A five-record do/while with one 8-byte pointer step matches
    # the target exactly.
    "audio/streaming/get_stream_buffer_size",
    # send_to_spu: Four-word stack DMA descriptor with retail SIF calls
    # reproduces the target object exactly.
    "audio/decoder/send_to_spu",
    # fun_00214720: plain C Vec4 transform and six ordered bounds checks
    # reproduce retail
    "rendering/culling/is_point_inside_clip_volume",
    # fun_0023a318: a packed Pair64 copy preserves the retail split 64-bit loads
    # and stores while float-return helper prototypes match the call shape
    "textbin/fun_0023a318",
    # fun_00205000: a typed global table with arrays at byte offsets 0x278,
    # 0x28c, and 0x2a4 preserves the retail indices and register order
    "ui/map/move_map_entry_slot",
    # fun_001f6200: initializing total and count before the empty-input branch
    # matches the retail delay-slot ordering
    "ui/text/measure_text_width",
    # fun_0023ba60: the four-argument sceCdRead call uses a 16-byte command
    # buffer and the initialized return value survives the call
    "textbin/fun_0023ba60",
    # fun_00214c48: drop the stale integer callee argument and assign the float
    # helper result back through the incoming FPU parameter
    "math/conversion/round_float_to_decimal_places",
    # memcard_make_whole_save: mutable byte pointer preserves the byte-stride
    # descriptor walk and helper argument order
    "storage/memory_card/memcard_make_whole_save",
    # fun_001fb368: volatile global pointer plus cached first access matches
    # repeated retail loads
    "rendering/packets/append_gif_transfer_packet",
    # fun_00230ee8: Dropping stale call arguments and using the retail state
    # switch matches exactly.
    "gameplay/state/dispatch_game_state_update",
    # draw_ties_1: Plain register hints and corrected void prototypes match the
    # retail call and cache sequence.
    "rendering/draw_ties_1",
    # draw_mobys_setup: Plain register hints preserve the retail callback
    # argument and pointer increment.
    "gameplay/entities/draw_mobys_setup",
    # fun_00214db0: plain trigonometric expansion preserves the retail
    # call/multiply order; native game-compiler exact 100/100/100
    "gameplay/camera/build_spherical_offset",
    # fun_002334d8: rewrite MMIO busy waits around the real SpinWait callee;
    # native game-compiler exact 100/100/100
    "rendering/dma/start_vif1_dma_transfer",
    # fun_00222290: plain control-flow rewrite with preserved caller
    # save/restore and non-small-data halfword store; native game-compiler exact
    # 100/100/100
    "gameplay/state/process_global_state_flags",
    # fun_0020cd48: plain rewrite: correct pointer argument order and read the
    # float field with lwc1; native game-compiler exact 100/100/100
    "rendering/geometry/transform_scaled_vertex_batch",
    # fun_0023ce28: plain retail-listing rewrite: reload the state base in the
    # wait loop; native game-compiler exact 100/100/100
    "video/decoder/run_video_decoder",
    # fun_00220648: switch on unk44; D_001516D8 read as D_001516D0.unk8 (struct,
    # not gp-small), D_00137B80 as a struct with 8-byte pair arrays at
    # 0x2C8/0x2F8; exact with default flags.
    "audio/streaming/advance_audio_stream_state",
    # fun_0012f2b8: plain rewrite (as promoted fun_0012f208): 0x3000-byte read
    # buffer then the 4-byte sceCdRMode, retry loop, 0x2960-byte copy to
    # D_00137B80; exact with default flags.
    "storage/cd/load_disc_sectors_into_global_buffer",
    # fun_0022dba0: plain rewrite (idx = arg0 + D_0015F5B4, arg1 passed through,
    # unk88 stored before unk7E), exact with default flags
    "audio/voices/allocate_voice_for_group_entry",
    # fun_00235780: plain rewrite: -1-terminated index list over D_001E1700
    # object pointers, 0x50-byte parts, s16 pair table merged into the low 14
    # bits; exact with default flags.
    "rendering/texture/patch_tie_texture_fields",
    # fun_0022c5a8: plain rewrite: float callee arguments as literals
    # (0.5f/6.0f/0.75f with f32 prototypes), void callees, D_0013E5BC[0]; exact
    # with default flags.
    "textbin/fun_0022c5a8",
    # fun_00225c18: plain rewrite: 5-entry {unk0, flags} table scan (flags^1
    # when arg0), exact with default flags
    "audio/streaming/select_next_stream_buffer",
    # fun_001ed940: plain rewrite: &D_001870D0 held in a pointer, D_0015EF9C a
    # scalar the assembler does not size (retail lui/at stores, gp only in delay
    # slots), if/else chain; exact with default flags.
    "textbin/fun_001ed940",
    # fun_00207b08: plain rewrite: void callees, D_0013D560 as s32[] indexed by
    # D_0015ED84 re-read after func_00208030; exact with default flags.
    "textbin/fun_00207b08",
    # fun_00204f60: plain rewrite: early return of func_00204EF8's result in the
    # loop variable, unk278[]/unk28C[] arrays, func_00205000(0, i); exact with
    # default flags.
    "ui/map/promote_first_available_map_entry",
    # fun_001ff570: plain rewrite: 13-entry 0x90-byte struct array search on
    # unk64, direct array indexing for the stores; exact with default flags.
    "gameplay/animation/set_animation_parameter",
    # fun_002270e8: plain rewrite (byte-pointer advance 0x20/0x30, both callees
    # take arg0), exact with default flags.
    "world/data/parse_typed_resource_record",
    # fun_00219fa0 is exact with the game's reconstructed 991111 compiler and
    # -mastra-r5900-extern-buffer. Keep it off EE_GCC_PATCHED_ROOT: that
    # separate patched SDK profile does not implement this game-only option.
    "textbin/fun_00219fa0",
    "textbin/fun_002071c0",
    "ui/hud/hud_heap_alloc",
    "audio/streaming/snd_init_vag_streaming_ex",
    "textbin/fun_00221968",
    # 2026-09-22 pending-unit sweep follow-up: exact on the game compiler
    # with default flags (100/100/100, full-ELF PASS).
    "rendering/buffers/swap_render_buffer_chain",
    "ui/help/try_set_help_message",
    "rendering/sky/sky_draw_shell",
    # fun_001f37e8: exact with default flags once its four same-file small
    # globals are declared to GAS before the body (__asm__ .extern); retail
    # only reaches those through gp, every other global absolutely.
    "gameplay/callbacks/reset_callback_registries",
    # dmac_vif1_enable: exact with default flags; the source shape (D_0015F1C
    # read through the pointer later reused for the DMAC register store) came
    # from a decomp-permuter run on the game-compiler route.
    "rendering/dmac_vif1_enable",
    # music_stop: exact with default flags (shared D_001516D0 layout from
    # include/rnc/d_001516d0.h; the final unk22/unk23 store order came from a
    # decomp-permuter run).
    "audio/music/music_stop",
    # draw_moby_list: exact with default flags once func_0020D218 is declared
    # void (an s32 declaration gives the call a dead v0 set, which pushes the
    # final D_0015FF14 value into v1) and the second argument is (long).
    "gameplay/entities/draw_moby_list",
    # fun_001e9ab8: exact with default flags once func_00233980 has its
    # promoted prototype (s32, s64): the second argument is passed as a
    # 64-bit value (callee-proto-fix.py).
    "gameplay/state/transition_draw_sky",
    # fun_001f79a8: exact with default flags: func_00233980(s32, s64) prototype,
    # the D_0015F348 constant written as -0.04f (m2c had its bit pattern as an
    # integer), and the gp .extern for D_0015F348.
    "textbin/fun_001f79a8",
    # fun_0020c940: exact with default flags; clean rewrite of the permuter
    # body (unk7D is tested, then re-read into the index, which gives retail's
    # v1 -> a1 copy).
    "textbin/fun_0020c940",
    # fun_0021bda0: exact with default flags; clean rewrite (unk28C[] array,
    # FUN_002166e8 called without arguments, unused parameters dropped).
    "textbin/fun_0021bda0",
    # fun_001eb740: exact with default flags on the 0049 compiler (a plain
    # rewrite; SN 2.95.2 emits the same code, the pre-0049 cc1 if-converted
    # the return tail).
    "gameplay/state/is_active_state_entry",
    "audio/streaming/snd_stream_safe_cd_break",
    "audio/streaming/snd_stream_safe_cd_callback",
    "audio/streaming/snd_stream_safe_cd_get_error",
    "audio/streaming/snd_stream_safe_cd_read",
    "gameplay/entities/attach_manipulator",
    "audio/decoder/audio_dec_begin_put",
    "rendering/vu1_add_g_sregister",
    "audio/streaming/snd_stream_safe_cd_sync",
    "audio/decoder/is_audio_ok",
    "audio/decoder/process_audio_stream",
    "textbin/fun_0023a3b8",
    "video/decoder/callbacks/handle_mpeg_no_data",
    "video/decoder/callbacks/handle_end_image",
    "video/decoder/vi_buf_begin_put",
    "video/display/wait_for_display_vsync",
    "rendering/debug/prepare_debug_profiler_render",
    "rendering/packets/put_disp_buffer",
    "audio/voices/allocate_voice_for_bank_entry",
    "ui/help/force_help_message",
    "textbin/fun_001f21c0",
    "ui/menus/draw_menu_selection_marker",
    "audio/rpc/snd_reset_state_and_flush_commands",
    "textbin/fun_0021e1f8",
    "textbin/fun_0021f120",
    "textbin/fun_001ff780",
    "textbin/fun_00221930",
    "runtime/dma/clear_dma_queue_entry",
    "rendering/state/initialize_render_state",
    "audio/decoder/audio_dec_start",
    "audio/banks/snd_resolve_bank_xrefs",
    "audio/mixer/snd_set_master_volume",
    "audio/mixer/snd_set_playback_mode",
    "audio/mixer/snd_set_mixer_mode",
    "audio/mixer/snd_set_group_voice_range",
    "audio/voices/snd_stop_all_sounds",
    "audio/voices/snd_sound_is_still_playing_cb",
    "audio/streaming/snd_stop_all_streams",
    "audio/effects/snd_pre_alloc_reverb_work_area",
    "audio/effects/snd_auto_reverb",
    "audio/movie/snd_init_movie_sound",
    "audio/movie/snd_reset_movie_sound",
    "audio/movie/snd_close_movie_sound",
    "audio/movie/snd_update_movie_adpcm",
    "audio/movie/snd_get_movie_nax",
    "ui/fonts/load_debug_font",
    "gameplay/camera/backup_current_cam",
    "textbin/fun_001eda60",
    "ui/text/measure_text_width_regular",
    "ui/text/measure_text_width_small",
    "ui/text/measure_text_width_large",
    "ui/text/font_print_large",
    "ui/text/font_print_small",
    "ui/text/font_print_right",
    "ui/text/font_print_right_small",
    "ui/text/font_print_right_large",
    "ui/text/font_print_center",
    "ui/text/font_print_center_small",
    "ui/text/font_print_center_large",
    "textbin/fun_001f6fd0",
    "ui/text/font_print_window_regular",
    "ui/text/font_print_window_small",
    "textbin/fun_001f7978",
    "ui/help/find_help_message_index",
    "ui/help/help_draw_prompt",
    "textbin/fun_001ff480",
    "ui/frames/draw_stretchable_ui_frame",
    "rendering/refresh_point_light",
    "rendering/detach_point_light",
    "ui/hud/load_compressed_hud_bank",
    "world/parse_space_scene_chunk",
    "ui/menus/hit_test_fixed_screen_rectangle",
    "textbin/fun_00207100",
    "textbin/fun_00207300",
    "textbin/fun_00207bb0",
    "gameplay/entities/process_moby_anim_data",
    "gameplay/entities/stash_moby_class_dists",
    "gameplay/entities/restore_moby_class_dists",
    "gameplay/entities/draw_mobys_clean_up",
    "gameplay/entities/draw_mobys",
    "math/random/random_integer_below",
    "math/random/random_float_between",
    "textbin/fun_002144d8",
    "math/rotations/build_quaternion_from_axis_angle",
    "gameplay/state/is_value_within_interpolated_window",
    "ui/menus/compute_clamped_count_difference",
    "ui/menus/count_nonzero_entries_up_to_40",
    "ui/menus/count_nonzero_entries_up_to_10",
    "ui/menus/count_nonzero_entries_up_to_30",
    "audio/streaming/register_audio_stream_callback",
    "audio/streaming/continue_audio_stream_if_ready",
    "audio/streaming/request_audio_stream_break",
    "audio/streaming/start_audio_stream_read",
    "audio/load",
    "audio/streaming/update_audio_stream_until_idle",
    "audio/streaming/finish_audio_stream_read",
    "input/pad/clear_pad_input",
    "input/pad/update_primary_pad_state",
    "textbin/fun_0021cae0",
    "textbin/fun_0021d2c8",
    "textbin/fun_0021df58",
    "textbin/fun_0021e608",
    "ui/menus/draw_quit_game_menu",
    "textbin/fun_0021eaf0",
    "ui/map/draw_map_screen_overlay",
    "ui/menus/missions/draw_mission_menu_labels",
    "textbin/fun_0021fce0",
    "textbin/fun_0021fd78",
    "rendering/draw_two_texture_panels",
    "textbin/fun_002212b8",
    "textbin/fun_00221a48",
    "textbin/fun_00221a88",
    "textbin/fun_00221d68",
    "textbin/fun_002220f0",
    "textbin/fun_00222d98",
    "textbin/fun_00222f18",
    "textbin/fun_00222f58",
    "textbin/fun_002242b8",
    "textbin/fun_00225660",
    "rendering/entities/draw_moby_entries_from_object",
    "textbin/fun_00226718",
    "gameplay/state/clear_scene_state_buffers",
    "audio/banks/load_audio_bank_by_location",
    "textbin/fun_0022da68",
    "runtime/rpc/initialize_sif_rpc",
    "rendering/dmac_vif1_disable",
    "runtime/diagnostics/print_register_values_and_halt",
    "rendering/buffers/copy_render_buffer_pair",
    "rendering/entities/register_entity_render_resources",
    "ui/text/set_scrolling_status_message",
    "ui/text/update_scrolling_status_message",
    "textbin/fun_00238630",
    "ui/menus/format_scaled_display_value",
    "textbin/fun_002386e8",
    "textbin/fun_00239750",
    "runtime/threads/switch_thread",
    "audio/decoder/terminate_audio_system",
    "audio/decoder/log_audio_error",
    "audio/decoder/audio_dec_delete",
    "audio/decoder/audio_dec_reset",
    "audio/decoder/audio_dec_send",
    "video/decoder/dma/set_dma_channel_3_control_register",
    "video/decoder/dma/set_dma_channel_4_control_register",
    "video/decoder/vi_buf_end_put",
    "video/decoder/vi_buf_count",
    "video/decoder/video_dec_create",
    "video/decoder/video_dec_begin_put",
    "video/decoder/video_dec_end_put",
    "video/decoder/video_dec_delete",
    "video/decoder/video_dec_input_count",
    "video/decoder/vo_buf_get_tag",
    "rendering/get_occlusion_grid_from_pair",
    "gameplay/entities/init_moby_class_dists",
    "storage/memory_card/memcard_initialize",
    "video/decoder/buffers/read_buf_begin_get",
    "audio/voices/snd_continue_all_sounds_in_group",
    "audio/streaming/snd_continue_vag_stream",
    "audio/streaming/snd_get_vag_stream_time_remaining_cb",
    "audio/streaming/snd_is_vag_stream_buffered_cb",
    "audio/voices/snd_pause_all_sounds_in_group",
    "audio/streaming/snd_pause_vag_stream",
    "audio/effects/snd_set_reverb_ex",
    "audio/streaming/snd_stream_safe_check_cd_idle",
    "audio/banks/snd_unload_bank",
    "rendering/update_occlusion",
    "video/decoder/vi_buf_delete",
    "video/decoder/video_dec_is_flushed",
    "video/decoder/video_dec_set_stream",
    "video/decoder/vo_buf_get_data",
    "textbin/fun_00225490",
    "textbin/fun_0022c7e8",
    # fun_0012e368: RPC wrapper: one-word request buffer passed to func_0012E6E0
    # (0x15)
    "audio/voices/snd_stop_sound",
    # fun_0012e4c0: RPC wrapper: six-word request buffer, two pass-through args
    # (0x21)
    "audio/voices/snd_set_sound_params_cb",
    # fun_0012e308: RPC wrapper: six-word request buffer, two pass-through args
    # (0x11)
    "audio/voices/snd_play_sound_vol_pan_pmpb",
    # fun_0020ad38: result preset to 0, compare only when the count is set
    "storage/memory_card/data/validate_data_crc",
    # fun_0023c660: semaphore-guarded round up to a 2048-byte sector
    "video/decoder/vi_buf_flush",
    # fun_0020c828: state byte from a pointer compare, s64 timestamp
    "gameplay/entities/mark_moby_for_removal",
    # fun_0012f108: RPC wrapper: five-word request buffer (0x3E)
    "audio/movie/snd_start_movie_sound",
    # fun_001ebec8: class-table dispatch through the update slot (+0x8)
    "gameplay/entities/update_moby",
    # fun_001ec3d8: class-table dispatch through the draw slot (+0x10)
    "rendering/entities/draw_moby",
    # fun_0012d9d8: main loop: each step returns the next step function
    "gameplay/state/run_game_main_loop",
    # fun_00216b68: handle claim: 64-bit handle argument narrowed to the
    # pointer, 0xFFFFFFFF marks a free slot
    "audio/voices/set_sound_handle_id",
    # fun_001fb680: GIF tag through the byte packet pointer, re-read after each
    # store
    "rendering/texture/append_texture_transfer_packet",
    # fun_001fb6e0: GIF tag through the byte packet pointer, re-read after each
    # store
    "rendering/texture/append_palette_transfer_packet",
    # fun_001fb3d0: GIF tag with a physical address (masked to 0x0FFFFFFF)
    # through the byte packet pointer
    "rendering/packets/append_draw_buffer_packet",
    # video_dec_put_ts: timestamp entry built on the stack, position relative to
    # the decoder base
    "video/decoder/video_dec_put_ts",
    # fun_001fb2d0: GIF tag into the packet when one is open, else
    # sceGsPutDrawEnv
    "rendering/packets/append_draw_environment_packet",
    # fun_00233888: VIF packet: DMA cnt tag, STCYCL, UNPACK V4-32 header, then
    # the payload copy
    "rendering/packets/write_vif_unpack_packet",
    # fun_001ff308: queue an animation on a channel unless it is already the
    # queued one; returns its serial
    "gameplay/animation/queue_animation_update",
    # fun_0012f368: read a file entry through sceCdRead with retries and copy it
    # into the resident buffer
    "storage/cd/read_file_entry_with_retry",
    # handle_camera_collision_with_hero: spawn or free the camera collision moby
    # when the hero's collision flag changes
    "textbin/gameplay/camera/handle_camera_collision_with_hero",
    # submit_audio_stream_io_request: start a CD read with the default mode and
    # clear the read state
    "textbin/audio/streaming/submit_audio_stream_io_request",
    # fun_00208840: run the current state handler and reset the frame counter
    # when the state changed
    "textbin/fun_00208840",
    # fun_0020cfd0: wind direction from a sampled vector: fast sin/cos of its
    # angle
    "textbin/fun_0020cfd0",
    # fun_00231608: switch the display resolution for a mode (PAL/NTSC table),
    # resync GS and restore the vsync callback
    "textbin/fun_00231608",
    # pcm_callback: MPEG PCM callback: copy the demuxed audio chunk (wrapping
    # the ring) into the IOP audio buffer
    "textbin/audio/decoder/callbacks/pcm_callback",
    # fun_00226e58: unlock menu entries by progress counters (15/30 and 10) and
    # pick their label ids
    "textbin/fun_00226e58",
    # music_start_track_30000: start a music track on channel 1 from the region
    # track table (stream arg only when D_0015EE1C set)
    "audio/music/music_start_track_30000",
    # fun_00228a30: patch tie class GIF tex words through the texture remap
    # table
    "textbin/fun_00228a30",
    # dma_moby_textures: chain the moby texture DMA refs into the packet
    "textbin/rendering/texture/dma_moby_textures",
    # fun_00232f20: read stash slot data back from the IOP in 0xFFFF-qword RPC
    # chunks
    "textbin/fun_00232f20",
    # memcard_get_name: build the memory card save name from the header and copy
    # it to the six name slots
    "textbin/storage/memory_card/memcard_get_name",
    # hud_send_texture: upload a HUD texture through sceGsSetDefLoadImage,
    # queued in the packet or sent immediately
    "textbin/ui/hud/hud_send_texture",
    # vi_buf_modify_pts: ViBuf: trim queued timestamps overlapped by new data
    # (IsInRegion helper)
    "textbin/video/decoder/vi_buf_modify_pts",
    # link_hud_bank: relocate a HUD texture bank's two entry tables to its
    # aligned VRAM base
    "textbin/ui/hud/link_hud_bank",
    # calculate_dma_transfer_address: address arithmetic matches on the native
    # route once the DMA descriptor fields are read as one struct
    "runtime/dma/calculate_dma_transfer_address",
    # enqueue_voice_request: the address must be an integer add with the index
    # on the left: i * 0x90 + (s32)handlers; pointer indexing yields base-first
    "textbin/audio/voices/enqueue_voice_request",
    # create_moby: the third vararg DebugPrint(..., oclass) pins oclass in a2
    # and frees a0 for the moby state temp; 11 ARG_MISMATCH rows become 0
    "textbin/gameplay/entities/create_moby",
    # draw_tfrag: one address-taken 0x40 struct Locals keeps the dead
    # sp+0x3C=1.0f store and the 0x70 frame; callees void with prototypes from
    # the promoted definitions
    "textbin/rendering/draw_tfrag",
    # setup_sky_gif_paging: paging setup matches once the page count is derived
    # from the packed struct field rather than recomputed
    "textbin/rendering/sky/setup_sky_gif_paging",
    # fade_to_black: the transition ramp matches with the two 64-bit halves
    # assigned in retail's order
    "textbin/rendering/transitions/fade_to_black",
    # update_fog: the fog-source selector must be read as struct field 0x2D4
    # into D_00187000, not as a scalar extern
    "textbin/rendering/update_fog",
    # vu1_send_chain: the bnezl + delay-slot pair is defeated by writing the
    # compare as a separate == 0 pair, which pulls the sw up to the subu
    "textbin/rendering/vu1_send_chain",
    # video_callback: the callback table is walked with a named cursor so the
    # index and the base keep retail's addend order
    "textbin/video/decoder/callbacks/video_callback",
    # fun_001f6060: the audio callback dispatch matches once every callee
    # prototype is taken from the promoted definitions
    "textbin/fun_001f6060",
    # fun_00204ef8: hoisting the two array bases into named s32 *locals flips
    # both indexed addu to retail's index-then-base order
    "textbin/fun_00204ef8",
    # fun_00215440: byte arithmetic for the D_0013D290 stride, a >8-byte struct
    # wrapper so the store stays gp-relative, and an explicit if/return chain
    "audio/music/music_start_track_60000",
    # music_start_track_40000: same family shape as music_start_track_60000; the
    # store order is what fixes it, not the constant
    "audio/music/music_start_track_40000",
    # fun_0021c7a0: real % modulo (EE-GCC expands smod as slt+movn), the unk50
    # base in its own surviving variable, and the byte cursor as a signed s32
    "textbin/fun_0021c7a0",
    # cam_interp_values: native route preserves the retail floating-point
    # compare interlock nop; linked bytes verified
    "textbin/gameplay/camera/cam_interp_values",
    # fun_002265d8: append a light record (13 words, cleared active flag first)
    # to the 8-entry table
    "textbin/fun_002265d8",
    # fun_001f0c50: draw text centred on x: sum glyph widths (unknown glyphs use
    # 0x20), then call the left-aligned draw
    "textbin/fun_001f0c50",
    # music_start_track_20000: start music channel 1 from the 20000-range track
    # table (same shape as music_start_track_60000)
    "audio/music/music_start_track_20000",
    # music_start_track_10000: start music channel 1 from the 10000-range track
    # table (same shape as music_start_track_60000)
    "audio/music/music_start_track_10000",
    # fun_00214970: Native route is byte-exact when the zero branch stores the
    # nine globals in the same order as the non-zero branch (ED80, ED60, ED64,
    # ED68, ED6C, ED70, ED74, ED78, ED7C): the ee-gcc store scheduler then emits
    # them as ED68, ED70, ED74, ED78, ED7C, ED80, ED60, ED64 with the gp-rel
    # ED6C in the delay slot, which is retail's order, so the four wrong linked
    # immediates (two LO16 and one GPREL16 word pairs) become right and
    # exact=True.
    "textbin/fun_00214970",
    # fun_00215970: The six 10000-step track-range thresholds as a plain else-if
    # chain plus the D_0013A664 + track*0x250 + D_0015ED88*4 lookup, and the
    # D_001516D0 field stores in the same order as the already-exact siblings
    # (unk50 first), made the whole dispatcher byte-exact.
    "audio/music/music_start_track_by_id",
    # memcard_prepare_data: Declaring the block pointer copy and the -4 align
    # mask inside the `if (blk->base != 0)` block (so the copy lands in the loop
    # preheader and is not coalesced with a2) and keeping an explicit `u8 *src`
    # temp for `block->base + slot * block->size` before the two header stores
    # gave s0=block/s1=p and byte-exact .text.
    "textbin/storage/memory_card/data/memcard_prepare_data",
    # music_preseek_track: retail keeps the music table address UNSPLIT (lui +
    # addiu +0x7b80 + addiu +0x2AA8): the base in one local and the 0x2AA8
    # offset in a SEPARATE s32 make it a register term, so legitimize_address
    # splits the address into two registers and the offset stays in the
    # instruction stream
    "textbin/audio/music/music_preseek_track",
    # music_start_track: retail keeps the music table address UNSPLIT (lui +
    # addiu +0x7b80 + addiu +0x2AA8): holding the base in one local and the
    # 0x2AA8 offset in a SEPARATE s32 makes it a register term, so
    # legitimize_address splits the address into two registers and the offset
    # stays in the instruction stream
    "textbin/audio/music/music_start_track",
    # stash_send_data: the tail needs four post-call locals: the counter
    # reloaded into n, an index copy i, the old cur in a local, and next = cur +
    # bytes computed before the counter update, with the counter store and the
    # three entry writes after it
    "textbin/world/data/stash_send_data",
    # fun_001f33b8: textbin/fun_001f33b8: banked on native at 81.61; the default
    # cc rule takes no per-unit flags, so the public score could not reproduce
    # it
    "assembly/textbin/fun_001f33b8",
    # fun_00221f58: textbin/fun_00221f58: banked on native at 97.65; the default
    # cc rule takes no per-unit flags, so the public score could not reproduce
    # it
    "assembly/textbin/fun_00221f58",
    # fun_00203120: upload a list of texture/CLUT images to VRAM from the
    # D_0015EE8C page cursor (PSMT8/PSMCT32/PSMCT16 sizes)
    "textbin/fun_00203120",
    # obtain_all_gold_weapons_menu: draw the two-line "obtained all gold
    # weapons" message box with its icons
    "textbin/ui/menus/weapons/obtain_all_gold_weapons_menu",
    # fun_00206860: decode one row of 4bpp run-length packed pixels (12-bit
    # count/colour codes) into a nibble buffer
    "textbin/fun_00206860",
    # music_start_track_50000: The shared request-record setup is byte-exact;
    # selecting the per-track handle as h[track - 50000][D_0015ED88] reproduces
    # the retail row addressing.
    "textbin/audio/music/music_start_track_50000",
    # do_sky_gif_paging: Declaring the paging flag as an array extern routes its
    # address through a register, so cc1 emits the lui as its own instruction at
    # the top of the body and the load as a separate 3-operand lw at the branch,
    # which is retail shape; the previously recorded blocker said no flag or
    # local could do this.
    "textbin/rendering/sky/do_sky_gif_paging",
    # fun_00207e58: Letting GCC strength-reduce mask[i] itself moves the cursor
    # init out of the prologue where a register pin had hoisted it, and a
    # counted for-loop makes the second loop's counter a reload that sorts after
    # the base load; this also corrects the bank, which counted 0x40 and wrote
    # 256 bytes into the 64-byte buffer retail counts 0xF into.
    "textbin/fun_00207e58",
    # moved off the SN route: byte-identical on the game compiler
    # snd_send_iop_command_and_wait: copy the command payload, wait for the IOP,
    # call the sound RPC and poll for completion (wait loop entered at its test)
    "textbin/audio/rpc/snd_send_iop_command_and_wait",
    # moved off the SN route: byte-identical on the game compiler
    # snd_send_iop_command_no_wait: append a sound command to the current batch
    # (send immediately when idle), waiting for batch space
    "textbin/audio/rpc/snd_send_iop_command_no_wait",
    # moved off the SN route: byte-identical on the game compiler
    # fun_0012f208: promoted exact under SN cc1 + Ps2EeAs (padless policy
    # "none", 172/172 bytes, verified 2026-09-22).  The plain cc_sn route
    # drops the retail lui/addiu pair and compiles to 164 B, so the unit
    # must stay on the padless route.
    "storage/wad/wad_get_sectors",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001ebcf0: run and clear the deferred callback list
    "gameplay/camera/execute_camera_post_update_callbacks",
    # moved off the SN route: byte-identical on the game compiler
    # update_all_cameras: pick the best active camera of 48, run its mode
    # update, keep the previous position
    "textbin/gameplay/camera/update_all_cameras",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001ed360: verified exact on the registered route
    "textbin/fun_001ed360",
    # moved off the SN route: byte-identical on the game compiler
    # reset_gs_registers: queue the two GIF reset tags and reapply the display
    # mode
    "textbin/rendering/state/reset_gs_registers",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001f4650: run each registered callback with its argument
    "gameplay/callbacks/dispatch_callback_list_1",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001f46c8: run each registered callback with its argument
    "gameplay/callbacks/dispatch_callback_list_2",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001f4740: run each registered callback with its argument
    "gameplay/callbacks/dispatch_callback_list_3",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001f4808: run each registered callback with its argument
    "gameplay/callbacks/dispatch_callback_list_4",
    # moved off the SN route: byte-identical on the game compiler
    # draw_ui_frame: Declaring the callee void
    # func_001F52A0(s32,s32,s32,s32,u64) and mutating the x/y parameters in
    # place (x += 4; y -= 4) with the remaining offsets inline in the calls
    # reproduced the retail register allocation exactly.
    "textbin/ui/frames/draw_ui_frame",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001f7a30: table fill with bits 3 and 4 of the index swapped
    "rendering/texture/initialize_alpha_lookup_table",
    # moved off the SN route: byte-identical on the game compiler
    "ui/help/get_help_message_text",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001fed30: move a slot to the end of the most-recently-used byte list
    "textbin/fun_001fed30",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001fee38: index of an id in the 0xFFFF-terminated animation table
    "gameplay/animation/find_animation_definition_index",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001ff418: switch to the queued animation: copy the next fields and run
    # its callback
    "gameplay/animation/apply_pending_animation",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001ff500: look up an animation definition and copy id, index, flags
    # and frame count
    "gameplay/animation/load_animation_definition",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001ff658: name buffer: default text block-copied when the name fits,
    # then strcpy
    "ui/text/copy_text_to_shared_buffer",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00203640: register a moby class slot: map class->slot,
    # slot->class/moby/data, then init the moby
    "textbin/fun_00203640",
    # moved off the SN route: byte-identical on the game compiler
    # fun_002043b0: place the level buffer below the top of RAM, page- and
    # quadword-aligned, then load into it
    "world/streaming/load_level_chunk_from_disc",
    # moved off the SN route: byte-identical on the game compiler
    # find_id_in_terminated_table: index of an id in a zero-terminated table of
    # at most 20 entries
    "textbin/world/data/find_id_in_terminated_table",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00205278: separate s64 max/delta locals with a distinct s32 loop-1
    # index, and plain array indexing so EE-GCC merges both arrays into one
    # induction variable
    "textbin/fun_00205278",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00206710: a src pointer variable for the 0x70000000 base keeps
    # retail's constant in s3 and its base+index add order, `s = i % 0x10` in
    # its own variable reproduces the divmod copy (daddu v0,v1,zero) and
    # retail's add destinations, and (u32) casts on the nibble shift/and give
    # srl plus a bnel for `if (t != 0)`
    "textbin/fun_00206710",
    # moved off the SN route: byte-identical on the game compiler
    "textbin/fun_00206f50",
    # moved off the SN route: byte-identical on the game compiler
    # run 13 wave 3 (2026-09-14): same short-loop padding-NOP class; the
    # padless object is instruction-identical (49/49) and patha byte-equal.
    "textbin/fun_002073b8",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00207a18: is a screen point within 35 units of the reference point
    # (always true while the override flag is set)
    "textbin/fun_00207a18",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00208280: format a menu text entry, substituting %b with the entry's
    # weapon name
    "textbin/fun_00208280",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00208408: map a point into a view's screen space (view 106 uses the
    # rotated layout), scaled by 1/512
    "textbin/fun_00208408",
    # moved off the SN route: byte-identical on the game compiler
    # 2026-09-25: these five were promoted exact on a non-default route, so
    # the baseline build compiled them with the default cc_sn and the linked
    # ELF differed from retail in exactly 336 bytes, all inside them:
    #   fun_001fee88 164 B  fun_00208770 95 B  fun_00215390 40 B
    #   fun_0022ea08 98 B  fun_00239690  6 B
    # The route lives in the workbench registry; without the entry here the
    # per-unit gate and the full build measure different compilers.
    "textbin/fun_00208770",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00209168: gp-rel defeated with a >8-byte struct wrapper for D_0013D290
    # so retail's lui/%hi+%lo form is reproduced
    "textbin/fun_00209168",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00209298: validate the two DMA packet sizes in a header, then
    # initialise one A and twenty B packets
    "world/streaming/initialize_dma_packet_payloads",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00209370: read the level chunk, start the loader and run the chunk at
    # its stored offset
    "world/streaming/load_and_initialize_level_chunk",
    # moved off the SN route: byte-identical on the game compiler
    # fun_0020acc0: 16-bit CRC (poly 0x1F45 step) over at most 0x1800 bytes
    "storage/memory_card/data/calculate_crc16",
    # moved off the SN route: byte-identical on the game compiler
    # fun_0020b950: evaluate the level's menu entries (unlock conditions,
    # callbacks) and count the available ones
    "textbin/fun_0020b950",
    # moved off the SN route: byte-identical on the game compiler
    # fun_0020baf0: is an unlock condition met (kind 0-9 switch over the
    # progress tables)
    "textbin/fun_0020baf0",
    # moved off the SN route: byte-identical on the game compiler
    "gameplay/state/compute_interpolated_record_value",
    # moved off the SN route: byte-identical on the game compiler
    # detach_manipulator: unlink a manipulator from a moby list and clear it
    "textbin/gameplay/entities/detach_manipulator",
    # moved off the SN route: byte-identical on the game compiler
    # fun_0020cc18: first free or matching slot in a 16-entry table
    "gameplay/entities/find_or_allocate_id_slot",
    # moved off the SN route: byte-identical on the game compiler
    # fun_0020cca8: Typed helper prototypes, one scratch record, and the retail
    # float field preserve the SN match.
    "textbin/fun_0020cca8",
    # moved off the SN route: byte-identical on the game compiler
    # fun_0020d060: chain the GIF paging refs (or a NOP ref when paging is off)
    # around the texture upload
    "textbin/fun_0020d060",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00212d68: look up an id in the -1-terminated map and store its
    # value/extra for the current slot (global re-read, no local)
    "textbin/fun_00212d68",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00212ed8: Pinning the three live pseudos to retail hard registers with
    # GCC register variables (anims $4, n $5, v $2) - the idiom already promoted
    # 145 times in the game tree - reproduces the retail lw v0 / lbu a1 pair
    # exactly; the four residual rows were a reload-allocator choice no source
    # shape could move.
    "textbin/fun_00212ed8",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00213358: build a direction from two random angles and a computed
    # pitch
    "textbin/fun_00213358",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00213ed8: step a value toward a target by at most step, return the
    # remaining error
    "textbin/fun_00213ed8",
    # moved off the SN route: byte-identical on the game compiler
    # load_display_text_resource_entry: A 0x20-byte local whose tail half is a
    # union with a mode(TI) member gives the por+sq zero store, and an explicit
    # f32 scale local that is live across the middle call forces the f20
    # callee-saved save/restore and the 0x50 frame.
    "textbin/ui/text/load_display_text_resource_entry",
    # moved off the SN route: byte-identical on the game compiler
    # fun_002169c0: handle callback: store the id, bump the state or report the
    # saved position
    "textbin/fun_002169c0",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00216a20: handle callback: store the id, bump the state or report the
    # saved position
    "textbin/fun_00216a20",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00217048: pad setup: DBC and pad2 init, then open socket slot 0
    "textbin/fun_00217048",
    # moved off the SN route: byte-identical on the game compiler
    # fun_002191b8: leave the level: release owner objects, refresh the 14 level
    # handles, set state 20
    "textbin/fun_002191b8",
    # moved off the SN route: byte-identical on the game compiler
    # fun_0021c420: reload the scratchpad lighting words for the object's
    # current slot
    "textbin/fun_0021c420",
    # moved off the SN route: byte-identical on the game compiler
    # fun_0021ddf8: release the owner's unheld item handles, keeping some while
    # a 0x9999 tag is in range
    "textbin/fun_0021ddf8",
    # moved off the SN route: byte-identical on the game compiler
    # fun_0021e110: item HUD: bail if the level id is locked, release attached
    # handles, else draw the 0x4F4D label
    "textbin/fun_0021e110",
    # moved off the SN route: byte-identical on the game compiler
    # fun_0021ea48: spawn the 0x46E helper moby at the camera anchor and link it
    # to its owner
    "textbin/fun_0021ea48",
    # moved off the SN route: byte-identical on the game compiler
    "textbin/fun_0021f158",
    # moved off the SN route: byte-identical on the game compiler
    # fun_002216c0: collect the owned items of the four slot tables into the
    # menu list (icon, id, two values, slot index)
    "textbin/fun_002216c0",
    # moved off the SN route: byte-identical on the game compiler
    "textbin/fun_00225530",
    # moved off the SN route: byte-identical on the game compiler
    # textbin/fun_002267b8: originally promoted 2026-09-18 (commit 78cae12)
    # under SN, then moved to GAME_COMPILER_UNITS on 2026-09-20 (commit
    # 3dd9c02, "game-only SN parity") when the game-compiler briefly achieved
    # parity for it. That parity has since broken (the installed game-compiler
    # patch revision moved again) -- verified 2026-09-22 with objdiff-cli
    # against the real expected object: cc_game route 43.77778%, fresh SN
    # 100/100/100/100. Moved back to SN, which is a fixed binary compiler and
    # doesn't drift.
    "textbin/fun_002267b8",
    # moved off the SN route: byte-identical on the game compiler
    "textbin/fun_00226a70",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00227140: queue a DMA ref tag to the slot matrix, then upload the slot
    # entry (TagPtr reloads; D_00160360[2] gp-small)
    "textbin/fun_00227140",
    # moved off the SN route: byte-identical on the game compiler
    # dma_shrub_textures: chain the shrub texture DMA refs, track the peak
    # upload size
    "textbin/rendering/texture/dma_shrub_textures",
    # moved off the SN route: byte-identical on the game compiler
    # dma_tfrag_textures: chain the tfrag texture DMA refs, track the peak
    # upload size
    "textbin/rendering/texture/dma_tfrag_textures",
    # moved off the SN route: byte-identical on the game compiler
    # patch_tfrag_gifs: patch tfrag GIF tex words through the texture remap
    # table
    "textbin/rendering/patch_tfrag_gifs",
    # moved off the SN route: byte-identical on the game compiler
    # dma_tie_textures: chain the tie texture DMA refs, track the peak upload
    # size
    "textbin/rendering/texture/dma_tie_textures",
    # moved off the SN route: byte-identical on the game compiler
    "audio/decoder/audio_dec_create",
    # moved off the SN route: byte-identical on the game compiler
    # audio_dec_end_put: account for bytes consumed by the audio decoder,
    # finishing the preload after 40 blocks
    "textbin/audio/decoder/audio_dec_end_put",
    # moved off the SN route: byte-identical on the game compiler
    "textbin/video/decoder/buffers/copy_video_buffer_region",
    # moved off the SN route: byte-identical on the game compiler
    # vi_buf_reset: ViBuf: reset counters and timestamps, rebuild the D4 ref
    # chain and restart the DMA channel
    "textbin/video/decoder/vi_buf_reset",
    # moved off the SN route: byte-identical on the game compiler
    # vi_buf_put_ts: ViBuf: queue a timestamp (pts/dts) in the ring under the
    # semaphore
    "textbin/video/decoder/vi_buf_put_ts",
    # moved off the SN route: byte-identical on the game compiler
    # vo_buf_create: Volatile field writes and buffer-stride loop preserve
    # retail store order.
    "textbin/video/decoder/vo_buf_create",
    # moved off the SN route: byte-identical on the game compiler
    # vo_buf_inc_count: VoBuf: mark the write slot full, advance the ring under
    # DI/EI (volatile write/count)
    "textbin/video/decoder/vo_buf_inc_count",
    # varargs: the game compiler's ginclude headers are installed next to its
    # driver, so this unit no longer needs the SN toolchain's stdarg.h
    "sdk/debug/debug_print",
    # moved off the SN route: byte-identical on the game compiler
    # fun_001ffc30: queue a textured sprite GIF packet (TEX0 from the texture
    # bank, RGBAQ alpha, UV/XYZ corners) on the DMA tag list
    "textbin/fun_001ffc30",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00200080: queue a HUD sprite GIF packet sized from the texture table
    # (1<<log2 sizes as UV)
    "textbin/fun_00200080",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00200258: HUD sprite packet like fun_00200080 with a UV origin
    # (texture size in subpixels)
    "textbin/fun_00200258",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00200958: queue a textured HUD sprite GIF packet (TEX0 from the
    # texture, UV/XYZ2 corners, alpha)
    "textbin/fun_00200958",
    # moved off the SN route: byte-identical on the game compiler
    # fun_00200f90: GIF packet like fun_00200e08 with an explicit Z for both
    # XYZ2 corners
    "textbin/fun_00200f90",
}

# Per-unit extra flags for GAME_COMPILER_UNITS (suffix match, as SN_FLAG_UNITS).
GAME_COMPILER_FLAG_UNITS = {
    # -mastra-cygnus-cfg (patch 0049): these sources were matched while the
    # Cygnus sibcall pass still ran its CFG cleanup before the first jump pass
    # (it deletes the jump after the last return, so jump.c if-converts
    # `return 1; ... return 0;' tails).  Retail/SN do not; the flag keeps the
    # old behavior for them.  fun_0012eea8, fun_0012ef28, fun_00207300 and the
    # snd_stream_safe_cd_* entries.  Every entry here is load-bearing: removing
    # the flag changes allocated sections or relocations.
    "hud_heap_alloc": "-mastra-cygnus-cfg",
    # fun_0012eb20: retail's D_0015EC8C accesses are gp-relative in the body
    # (the .extern-ordering class); its call loop needs patch
    # 0046-r5900-pad-unfilled-loops (cc1 eb7a3497...).  100/100/100 and
    # full-ELF PASS on 2026-09-22.
    "snd_init_vag_streaming_ex": "-mastra-r5900-extern-buffer",
    "fun_00219fa0": "-mastra-r5900-extern-buffer",
    # fun_00221968: 100/100/100 on the game compiler only with
    # -fno-expensive-optimizations (the bank flag; without it 90.45).  Its
    # 2026-09-22 demotion measured cc_game without the flag (62.65).
    "fun_00221968": "-fno-expensive-optimizations",
    "snd_stream_safe_cd_break": "-mastra-r5900-extern-buffer -mastra-cygnus-cfg",
    "snd_stream_safe_cd_callback": "-mastra-r5900-extern-buffer -mastra-cygnus-cfg",
    "snd_stream_safe_cd_get_error": "-mastra-r5900-extern-buffer -mastra-cygnus-cfg",
    "snd_stream_safe_cd_read": "-mastra-r5900-extern-buffer -mastra-cygnus-cfg",
    # vu1_add_g_sregister needs only the address form: the game compiler already
    # builds without strict aliasing, so -fno-strict-aliasing changes nothing
    # here while -mno-split-addresses is required.
    "vu1_add_g_sregister": "-mno-split-addresses",
    "snd_stream_safe_cd_sync": "-fno-gcse -mastra-r5900-extern-buffer",
    "is_audio_ok": "-mno-split-addresses",
    "process_audio_stream": "-mno-split-addresses",
    "fun_0023a3b8": "-mno-split-addresses",
    "handle_mpeg_no_data": "-mno-split-addresses",
    "handle_end_image": "-mno-split-addresses",
    "wait_for_display_vsync": "-mno-split-addresses",
    "prepare_debug_profiler_render": "-mno-split-addresses",
    "put_disp_buffer": "-mno-split-addresses",
    "allocate_voice_for_bank_entry": "-mno-split-addresses",
    "force_help_message": "-mno-split-addresses",
    "fun_001f21c0": "-mno-split-addresses",
    "draw_menu_selection_marker": "-mastra-r5900-extern-buffer",
    "snd_reset_state_and_flush_commands": "-mastra-r5900-extern-buffer",
    "fun_00225490": "-fno-schedule-insns",
    "fun_0022c7e8": "-fno-schedule-insns",
    # fun_001f33b8 (-fno-schedule-insns) and fun_00221f58 (-G0) carry no entry:
    # both owners are still assembly wrappers, where an option cannot change the
    # wrapper's bytes. The shorter keys also always won first-suffix-match over
    # the longer "textbin/..." spellings, so those were dead as well. Re-add with
    # the measurement and the reason recorded here if a C body needs them.
    # moved off the SN route with its unit; the game compiler needs the same option
    "fun_00225530": "-mno-split-addresses",
    # moved off the SN route with its unit; the game compiler needs the same option
    "audio_dec_create": "-mno-split-addresses",
}

SN_FLAG_UNITS = {
    # snd_post_message: retail keeps the index in v1 and the base in v0; the
    # default prepass scheduler swaps them.  100/100/100 with
    # -fno-schedule-insns (pipeline-2026-09-13-11 wave 2).
    "snd_post_message": "-fno-schedule-insns",
    # fun_0021b6d8: exact-route compiler flags.  All four register pins are
    # required here; dropping any one of them changes the linked bytes.  This is
    # the last register-allocation workaround in the project and the only entry
    # no compiler improvement can retire on its own.
    "fun_0021b6d8": "-ffixed-4 -ffixed-5 -ffixed-6 -ffixed-7",
}

# Units whose retail objects carry compiler-emitted hazard NOPs that the
# bundled GNU assembler drops (FPU move-to-cop / compare hazards and the
# load-delay filler).  They compile with the SN cc1 but assemble with the SN
# toolchain's `ee/bin/Ps2EeAs.exe`, followed by a repo-owned normalization pass that removes only the
# assembler's section tail padding.  The list is explicit per unit: the
# assembler swap is proven per-object and must not drift to other units.
PADLESS_ASM_UNITS = {
    "textbin/fun_00239690",
    "textbin/fun_0022ea08",

    "math/random/random_float_between",
    "textbin/fun_001ff480",
    "textbin/fun_001eda60",
    # padless-release sweep 2026-09-12: both banks keep a dropped hazard
    # NOP/load-delay residual under the bundled GNU as and reach 100/100/100
    # with the padless route (fun_00225660 also needed the build's canonical
    # FUN_00225530 callee symbol).
    "textbin/fun_00225660",
    "rendering/dmac_vif1_disable",
    # padless-release sweep follow-up (run 7 resume): empty-asm barrier on the
    # 64-bit call result pins the two outgoing spills (sd v0,8(sp)/sd v1,0(sp))
    # ahead of the addiu pair, 100/100/100.
    "ui/help/help_draw_prompt",
    # run 13: the bundled GNU as refuses the short-loop padding NOP because the
    # loop body contains a forward branch; SN cc1 + Ps2EeAs emits it and the
    # resulting object is instruction-identical to the retail target (106/106).
    "textbin/fun_002212b8",
    # run 14 mass-c: FUN_0022da68 had two compiler-emitted hazard NOPs dropped
    # by the bundled GNU as (padless object instruction-identical, 41/41;
    # patha byte-equal vs retail).
    "textbin/fun_0022da68",
    # run 14 mass-c: two loop padding NOPs dropped by the bundled GNU as
    # (padless object instruction-identical, 44/44; patha byte-equal vs retail).
    "textbin/fun_002242b8",
    # run 14 mass-c: -G0 removes the .lit4 float load and the FPU mtc1->c.le.s
    # hazard NOP is Ps2EeAs-emitted (padless object instruction-identical,
    # 48/48; patha byte-equal vs retail).
    "textbin/fun_00207300",
    # run 16 worker a: the two FPU mtc1->c.le.s hazard NOPs are Ps2EeAs-emitted
    # (padless object instruction-identical, 48/48; patha byte-equal vs retail).
    # The winning source also needs the goto-chain tail plus v0/v1 register pins.
    "textbin/fun_00207100",
    # run 16 worker b: the short-loop erratum padding NOPs (3 after the jal)
    # are Ps2EeAs-emitted; the padless object is instruction-identical (76/76)
    # and patha byte-equal (sha 1487c262...).  Needs SN_FLAG_UNITS
    # -mno-split-addresses for the absolute $at store pair.
    "video/display/wait_for_display_vsync",
    # run 16 worker b: cc_sn_padless is also the route that reproduces retail's
    # absolute same-register load of the scalar pointer global D_0016120C
    # (the cc_sn route emits %gp_rel); with the v0 pin on the second load the
    # object is instruction-identical (67/67) and patha byte-equal
    # (sha 508793fb...).  No SN flag needed.
    "audio/decoder/terminate_audio_system",
    # fun_001f5210: RGBA packed from four int arguments as u64 (GS register
    # style), then a GIF tag
    "rendering/packets/emit_rgba_draw_packet",
    # fun_001ff960: frame index of an animation, 0 when the frame or its data is
    # missing
    "gameplay/animation/find_valid_animation_frame_index",
    # fun_0020c880: current and next animation frame pointers of a moby
    "gameplay/animation/update_moby_animation_state",
    # video_dec_flush: append the 4-byte end code to the ring buffer, send it,
    # round the stream position
    "video/decoder/video_dec_flush",
    # fun_001f5138: fog/alpha GS registers around a full-screen sprite when
    # enabled
    "rendering/effects/draw_fogged_fullscreen_sprite",
    # fun_0022c830: camera roll from the look vector angle, scaled by the
    # clamped distance
    "textbin/fun_0022c830",
    # patch_moby_gifs: patch moby class GIF tex words through the texture remap
    # table
    "textbin/gameplay/entities/patch_moby_gifs",
    # fun_00202800: load packed screen points: shift x/y, convert u/v, clear
    # flags
    "textbin/fun_00202800",
    # vi_buf_stop_dma: ViBuf: stop the IPU DMA, save D4/D3 channel and IPU
    # registers after the FIFO drains
    "textbin/video/decoder/vi_buf_stop_dma",
    # fun_00232d00: bind the stash RPC server, read its IOP buffer and reset the
    # stash slots
    "textbin/fun_00232d00",
    # snd_bank_load_by_loc: padless route with no policy: the bank load resolves
    # once the sub-record pointer is a named local
    "textbin/audio/banks/snd_bank_load_by_loc",
    # saving_data_menu: byte arithmetic for the D_0013D290 entry, array/scalar
    # extern choices that keep the store in retail order, and a packed one-field
    # struct for the unaligned 64-bit copy
    "textbin/ui/menus/save_data/saving_data_menu",
    # fun_002135f0: padless route: the block header fields are read through one
    # struct so the scale stays in the load
    "textbin/fun_002135f0",
    # fun_00200c80: queue a GIF packet with one register pair and two XYZ2
    # corners (pixel or subpixel coordinates)
    "textbin/fun_00200c80",
    # fun_00200e08: same packet as fun_00200c80 with register 0x46
    "textbin/fun_00200e08",
    # snd_bank_load_from_ee_cb: load a sound bank over SIF RPC 0x57 once the
    # server is idle (scalar lui/at externs, gp .extern flags)
    "textbin/audio/banks/snd_bank_load_from_ee_cb",
    # fun_001f4fb8: Byte-exact on the padless route (both none and la-gprel
    # policies) with the mask as 0xFF000000FFULL, a local s32 temp for each
    # tested field so the test and the 5th u64 arg share one load, an s64 mask
    # local for the loop, and the 5th arg written as (u64)((s64)x << 0x20) >>
    # 0x20 to force the dsll32/dsrl32 sign-extension; native/sn top out at 97.5
    # because their assembler expands dli differently.
    "textbin/fun_001f4fb8",
    # fun_002028e0: Plain-C rewrite: the 8-byte sprite clear must be a struct
    # s64 field store (not a cast-pointer store) so reload.c coalesces the
    # post-call %hi/%lo reload into the loop-carried base copy, a separate index
    # variable for the group loop pins f->s3/i->s4, and `f->loaded = 1` before
    # the relocation stores fixes their schedule.
    "textbin/fun_002028e0",
    # init_view_context: Plain-C rewrite of the public NON_MATCHING body is
    # byte-exact on the padless route: the Display/Screen/View structs, the s16
    # half-width shifts, the float literals (32.0f, 745472.0f, 0.63f, 0.5f,
    # 4.0f, 524288.0f, 255.0f) and the two func_001FA6C0 calls reproduce every
    # store in retail order; only the padless assembler drops the single nop the
    # native route inserts after the 255.0f mtc1, so the source is already
    # optimal and the route is the only lever
    "textbin/rendering/view/init_view_context",
    # set_up_vis_gif_viewer: Registered route is padless+none (native scores
    # only 87.8): the packet high word is (u64)(u32)n << 32 taken from the 2nd
    # argument instead of w1 >> 32, and 0x20 is OR-ed with (w1 & 0x1C) in the
    # mode>=0 branch but with (prim << 6) in the two negative branches.
    "textbin/rendering/set_up_vis_gif_viewer",
    # fun_001ffe18: queue a textured quad as a 4-vertex triangle strip GIF
    # packet (same texture bank lookup as fun_001ffc30)
    "textbin/fun_001ffe18",
    # fun_00200600: Decoding the listing dsll32 as a shift by N+32 on the R5900
    # (both packet 64-bit constants come out of the ori/dsll/ori chains),
    # assigning pos.x/pos.y first so f12/f13 stay out of extra callee-saves, and
    # keeping the two size args s32 to avoid the zero-extend pair, reproduced
    # retail exactly on the padless route; the registered sn route is 99.38.
    "textbin/fun_00200600",
    # parse_particle_textures: The a1/a3 induction-pointer swap was the ORDER OF
    # INCREMENTS: p is a walked front-end pointer read through *p, p = p + 1
    # sits in the for-increment clause after i = i + 1 so the loop bottom RTL
    # orders [counter][p walk], and the table stays indexed so its base
    # materialises in the preheader. 45 earlier shapes had missed it.
    "textbin/rendering/texture/parse_particle_textures",
    # fun_00221460: A dead `p = m->items;` statement that cc1 deletes still
    # perturbs the local hard-register order into retail's, and declaring
    # func_001F6530 void removes the unused-return pseudo so its argument copies
    # emit in retail's order a2<-s0, a3<-v0, a1<-s2. NOTE:
    # src/assembly/textbin/fun_001fd748.c still declares that callee as s32 in
    # another translation unit.
    "textbin/fun_00221460",
    # fun_0021b6d8: Byte-exact only with per-unit -ffixed flags: local_alloc
    # ranks argument registers above v1/v0 for short-lived pseudos, so the two
    # != -1 condition pseudos land in a3 and v1 where retail uses v1 and v0. 70
    # source shapes and all seven cc1 builds leave the pseudo set unchanged, and
    # a register-variable equivalent fails because cc1 splits a single-use
    # temporary out of its pinned variable. NOTE: promotion writes an
    # SN_FLAG_UNITS entry in the game configure.py.
    "textbin/fun_0021b6d8",
}


def _unit_flag(unit: str) -> str:
    for suffix, flags in EE_GCC_FLAG_UNITS.items():
        if unit.endswith(suffix):
            return flags
    return ""


def _unit_patched_flag(unit: str) -> str:
    for suffix, flags in EE_GCC_PATCHED_FLAG_UNITS.items():
        if unit.endswith(suffix):
            return flags
    return ""


def _unit_policy(unit: str) -> str:
    for suffix, policy in PADLESS_POLICY_UNITS.items():
        if unit.endswith(suffix):
            return policy
    return "none"


def _unit_sn_flag(unit: str) -> str:
    for suffix, flags in SN_FLAG_UNITS.items():
        if unit.endswith(suffix):
            return flags
    return ""


def _unit_uses_sn(unit: str) -> bool:
    if unit in SDK_COMPILER_UNITS:
        return False
    # Textbin-origin C units use SN only when the retail bytes use sq/lq saves.
    # _retail_save_style() is the per-unit fingerprint; origin alone is not
    # enough because sd/ld and save-less leaves stay on EE-GCC 2.9-style routes.
    # Most original owners remain under textbin/, while semantically relocated
    # owners are listed in TEXTBIN_ORIGIN_UNITS. Assembly-backed textbin wrappers
    # remain under assembly/ and keep their native EE-GCC assembler oracle.
    return unit.startswith("textbin/") or unit in TEXTBIN_ORIGIN_UNITS


def _retail_save_style(elf: bytes, vram: int, size: int) -> str:
    """Fingerprint a unit's retail prologue save style from the boot ELF.

    Maps vaddr to file offset for the flat .main text (vram 0x100080 at file
    0x1000). Returns 'sq' if sq/lq dominates, 'sd' if sd/ld dominates, else
    'none' for no callee-save accesses in the first 4KB.
    """
    text_vram = 0x100080
    text_file = 0x1000
    sq = lq = sd = ld = 0
    for off in range(vram, vram + min(size, 4096), 4):
        foff = text_file + (off - text_vram)
        if foff + 4 > len(elf) or foff < 0:
            break
        word = struct.unpack_from("<I", elf, foff)[0]
        op = word >> 26
        if op == 0x1E:
            sq += 1
        elif op == 0x1F:
            lq += 1
        elif op == 0x3F:
            sd += 1
        elif op == 0x37:
            ld += 1
    if sq + lq >= sd + ld and sq + lq > 0:
        return "sq"
    if sd + ld > 0:
        return "sd"
    return "none"


LANGUAGES = {
    "SCUS_971.99": "us",
}

BASENAME = "SCUS_971.99"
LD_PATH = f"{BASENAME}.ld"
ELF_PATH = f"build/{BASENAME}"
MAP_PATH = f"build/{BASENAME}.map"
PRE_ELF_PATH = f"build/{BASENAME}.elf"

OBJDIFF_CATEGORY = {"id": "us", "name": "Ratchet & Clank (USA)"}

# Configuration names become file paths and compiler command fragments.
# Restrict them to the project's alphabet and forbid path escapes so a
# malformed or hostile row cannot write outside the build workspace.
_UNIT_NAME_RE = re.compile(r"^[A-Za-z0-9_][A-Za-z0-9_./-]*$")


def _check_unit_name(name: str) -> None:
    parts = [part for part in name.split("/") if part not in ("", ".")]
    if (
        not _UNIT_NAME_RE.match(name)
        or not parts
        or any(part == ".." for part in parts)
    ):
        raise SystemExit(
            f"unsafe unit name in build configuration: {name!r} "
            "(allowed: letters, digits, '_', '.', '/', '-'; no '..')"
        )


def validate_config_names(node: Any) -> None:
    """Reject configuration names that could escape the build tree."""
    if isinstance(node, dict):
        for key, value in node.items():
            if key == "name" and isinstance(value, str):
                _check_unit_name(value)
            validate_config_names(value)
    elif isinstance(node, list):
        if len(node) >= 3 and isinstance(node[1], str) and isinstance(node[2], str):
            _check_unit_name(node[2])
        for item in node:
            validate_config_names(item)


@contextlib.contextmanager
def suppress_stdout_stderr():
    null_fds = [os.open(os.devnull, os.O_RDWR) for _ in range(2)]
    save_fds = [os.dup(1), os.dup(2)]
    os.dup2(null_fds[0], 1)
    os.dup2(null_fds[1], 2)
    try:
        yield
    finally:
        os.dup2(save_fds[0], 1)
        os.dup2(save_fds[1], 2)
        for fd in null_fds + save_fds:
            os.close(fd)


def get_compiler_command(command: str) -> Path:
    compiler_dir = Path("tools") / "cc" / COMPILER
    ee_dir = compiler_dir / "lib" / "gcc-lib" / "ee"
    ee_compiler_dirname = next(os.walk(ee_dir))[1][0]

    commands = {
        "ee-gcc": compiler_dir / "bin" / "ee-gcc",
        "cpp": compiler_dir / "lib" / "gcc-lib" / "ee" / ee_compiler_dirname / "cpp",
    }

    return commands[command]


def make_compiler_cmd(config_dir: Path, src_path: Path) -> tuple[str, str]:
    rel_root = Path(os.path.relpath(ROOT, config_dir))
    game_cc_dir = f"{rel_root}/tools/cc/{COMPILER}/bin"

    common_includes = (
        f"-I{src_path.parent / 'src'} "
        f"-I{src_path.parent / 'include'} "
        f"-Iinclude "
        f"-Wa,-I{src_path.parent / 'include'} -Wa,-I{src_path.parent}"
    )

    compile_cmd = (
        f"{game_cc_dir}/ee-gcc -c {common_includes} {LANG_DEFINE} {COMPILER_FLAGS}"
    )

    return compile_cmd, common_includes


def sn_compiler_configured() -> bool:
    return (
        bool(SN_TOOLCHAIN_ROOT)
        and (Path(SN_TOOLCHAIN_ROOT) / "bin/ee-gcc.exe").is_file()
    )


def _game_compiler_root() -> Path:
    """Locate the reconstructed game compiler.

    The repository keeps it under tools/compilers/game-compiler; the baseline
    staging workspace copies the whole compilers tree to tools/cc, and
    GAME_COMPILER_ROOT overrides both.
    """
    if GAME_COMPILER_ROOT:
        return Path(GAME_COMPILER_ROOT)
    for candidate in (ROOT / "tools/compilers/game-compiler", ROOT / "tools/cc/game-compiler"):
        if (candidate / "ee-gcc").is_file():
            return candidate
    return ROOT / "tools/compilers/game-compiler"


def game_compiler_configured() -> bool:
    root = _game_compiler_root()
    return (root / "ee-gcc").is_file() and (root / "cc1").is_file()


def _unit_game_flag(unit: str) -> str:
    for suffix, flags in GAME_COMPILER_FLAG_UNITS.items():
        if unit.endswith(suffix):
            return flags
    return ""


def ee_gcc_patched_configured() -> bool:
    return bool(EE_GCC_PATCHED_ROOT) and (Path(EE_GCC_PATCHED_ROOT) / "xgcc").is_file()


def _win_path(value: str) -> str:
    """Convert a WSL mount path to the form the Windows driver needs.

    The drive letter is derived from the mount itself, so any /mnt/<drive>
    mount works regardless of the actual drive letter on a given machine.
    """
    match = re.match(r"^/mnt/([A-Za-z])/(.*)$", value)
    if match:
        return f"{match.group(1).upper()}:/{match.group(2)}".replace("/", "\\")
    return value.replace("/", "\\")


def _unit_from_object(object_path: Path) -> str:
    """Derive the unit name from the ninja object path.

    build/src/<unit>.c.o -> <unit>. Handles both the absolute baseline path
    and the config-relative path.
    """
    parts = list(Path(object_path).parts)
    if "src" in parts:
        parts = parts[parts.index("src") + 1 :]
    joined = "/".join(parts)
    if joined.endswith(".c.o"):
        joined = joined[: -len(".c.o")]
    return joined


# C aliases in promoted sources: ALIAS __attribute__((alias("TARGET"))).
# The oracle fallback keeps the bytes but not the aliases, so they are handed
# to the linker (PROVIDE, only when nothing else defines them).
_ALIAS_RE = re.compile(
    r"([A-Za-z_]\w*)\s*__attribute__\s*\(\(\s*alias\s*\(\s*\"([^\"]+)\"\s*\)\s*\)\)"
)


def _alias_symbols(source: Path) -> set[tuple[str, str]]:
    """(alias, target) pairs declared in a source file."""
    text = source.read_text(errors="replace")
    return {(match.group(1), match.group(2)) for match in _ALIAS_RE.finditer(text)}


PADLESS_ASM_HELPER = r'''#!/usr/bin/env python3
"""Normalize SN cc1 output for Ps2EeAs and drop section tail padding.

`normalize IN OUT` rewrites GNU `alias = function` assignments into
co-located labels (Ps2EeAs rejects the assignment form).
`finish PADDED OUT` removes only the `.text` section tail padding after the
last sized function and adds empty `.data`/`.bss` sections so object
comparison sees the GNU layout.  Every check fails closed; no target bytes,
addresses or expected lengths are inputs.
"""
import re
import struct
import sys


def normalize_aliases(text):
    labels = set(re.findall(r"^\s*([\w.$]+):\s*$", text, re.M))
    pattern = re.compile(r"^\s*([\w.$]+)\s*=\s*([\w.$]+)\s*$", re.M)
    aliases = {}
    for match in pattern.finditer(text):
        alias, target = match.groups()
        if target in labels:
            aliases.setdefault(target, []).append(alias)
    text = pattern.sub(lambda m: "" if m[2] in aliases else m[0], text)
    for target, names in aliases.items():
        text = re.sub(r"^(\s*)" + re.escape(target) + r":\s*$",
                      lambda m: "".join(name + ":\n" for name in names) + target + ":",
                      text, count=1, flags=re.M)
    return text


def unpad(data):
    if data[:7] != b"\x7fELF\x01\x01\x01" or struct.unpack_from("<H", data, 16)[0] != 1:
        raise ValueError("expected a little-endian ELF32 relocatable object")
    shoff = struct.unpack_from("<I", data, 32)[0]
    shsize, count, names_index = struct.unpack_from("<HHH", data, 46)
    if shsize != 40 or shoff + count * shsize > len(data):
        raise ValueError("invalid section table")
    headers = [struct.unpack_from("<10I", data, shoff + i * shsize) for i in range(count)]
    nh = headers[names_index]
    names = data[nh[4]:nh[4] + nh[5]]
    text_indices = [i for i, h in enumerate(headers)
                    if names[h[0]:].split(b"\0")[0] == b".text"]
    if len(text_indices) != 1:
        raise ValueError("expected exactly one .text section")
    index = text_indices[0]
    text = headers[index]
    if text[1] != 1 or text[2] & 6 != 6 or text[4] + text[5] > len(data):
        raise ValueError("invalid executable .text section")
    symbols = []
    for h in headers:
        if h[1] == 2:
            if h[9] != 16 or h[5] % 16:
                raise ValueError("invalid symbol table")
            for offset in range(h[4], h[4] + h[5], 16):
                sym = struct.unpack_from("<IIIBBH", data, offset)
                if sym[5] == index:
                    symbols.append(sym)
    funcs = [s for s in symbols if s[3] & 15 == 2 and s[2]]
    if not funcs:
        raise ValueError("no sized function symbols; cannot infer code extent")
    end = max(s[1] + s[2] for s in funcs)
    padding = text[5] - end
    if padding < 0 or padding >= max(text[8], 1) or end % 4:
        raise ValueError("function extent does not explain section tail padding")
    if any(data[text[4] + end:text[4] + text[5]]):
        raise ValueError("nonzero bytes after final function")
    if any(s[1] > end or (s[1] == end and s[3] & 15 not in (0, 3))
           or (s[3] & 15 != 3 and s[1] + s[2] > end) for s in symbols):
        raise ValueError("symbol refers to removed padding")
    for h in headers:
        if h[1] in (4, 9) and h[7] == index:
            stride = 12 if h[1] == 4 else 8
            if h[9] != stride or h[5] % stride:
                raise ValueError("invalid relocation table")
            if any(struct.unpack_from("<I", data, offset)[0] >= end
                   for offset in range(h[4], h[4] + h[5], stride)):
                raise ValueError("relocation refers to removed padding")
    result = bytearray(data)
    struct.pack_into("<I", result, shoff + index * shsize + 20, end)
    return bytes(result)


def add_empty_sections(data):
    shoff = struct.unpack_from("<I", data, 32)[0]
    shsize, count, names_index = struct.unpack_from("<HHH", data, 46)
    headers = [list(struct.unpack_from("<10I", data, shoff + i * shsize)) for i in range(count)]
    nh = headers[names_index]
    names = bytearray(data[nh[4]:nh[4] + nh[5]])
    present = {bytes(names[h[0]:]).split(b"\0")[0] for h in headers}
    added = []
    for name, kind in ((b".data", 1), (b".bss", 8)):
        if name in present:
            continue
        headers.append([len(names), kind, 3, 0, len(data), 0, 0, 0, 1, 0])
        names.extend(name + b"\0")
        added.append(name.decode())
    if not added:
        return data
    result = bytearray(data)
    nh[4], nh[5] = len(result), len(names)
    result.extend(names)
    result.extend(b"\0" * (-len(result) % 4))
    struct.pack_into("<I", result, 32, len(result))
    struct.pack_into("<H", result, 48, len(headers))
    for h in headers:
        result.extend(struct.pack("<10I", *h))
    return bytes(result)


def apply_at_store_policy(assembly):
    import re
    if re.search(r"\.set[ \t]+noat", assembly):
        return assembly
    output = []
    pending = False
    for line in assembly.splitlines(keepends=True):
        if re.match(r"^[ \t]*li[ \t]+\$1[ \t]*,[ \t]*\S+[ \t]*(?:#.*)?$", line):
            output.append("\t.set\tnoat\n")
            pending = True
            output.append(line)
        elif pending and re.match(r"^[ \t]*sw[ \t]+\$?\w+[ \t]*,[^#\n]*\(\$1\)", line):
            output.append(line)
            output.append("\t.set\tat\n")
            pending = False
        else:
            output.append(line)
    if pending:
        raise SystemExit("at-store policy: li $1 without a following store through $1")
    return "".join(output)


def apply_la_gprel_policy(assembly):
    """Hoist `.extern` size directives for `la`-only small-data symbols.

    Ps2EeAs is a single-pass assembler: a bare symbol reference is relaxed to
    gp-relative only when the symbol's `.extern NAME, SIZE` directive has
    already been seen, while cc1 emits every directive at end of file.  A
    symbol that is only ever the address operand of `la` therefore expands to
    lui+addiu instead of retail's single `addiu $r,$gp,%gprel(NAME)`.  Only
    symbols that (a) declare a size in 1..8 and (b) never appear as a memory
    operand are moved; hoisting memory-operand externs over-relaxes accesses
    that retail keeps absolute (measured regression).
    """
    import re
    extern_line = re.compile(r"^[ \t]*\.extern[ \t]+([\w.$]+)[ \t]*,[ \t]*(\d+)[ \t]*(?:#.*)?$")
    la_line = re.compile(r"^[ \t]*la[ \t]+\$?\w+[ \t]*,[ \t]*([\w.$]+)[ \t]*(?:#.*)?$")
    label_line = re.compile(r"^[ \t]*([\w.$]+):")
    lines = assembly.splitlines(keepends=True)
    # SN's Windows driver writes CRLF; strip the CR for matching only.
    stripped = [line.rstrip("\r\n") for line in lines]
    entries = []
    for index, line in enumerate(stripped):
        match = extern_line.match(line)
        if match:
            entries.append((index, match.group(1), int(match.group(2))))
    if not entries:
        return assembly
    extern_indices = {index for index, _, _ in entries}
    labels = {m.group(1) for m in (label_line.match(line) for line in stripped) if m}
    la_lines = {index for index, line in enumerate(stripped) if la_line.match(line)}
    hoisted = set()
    for index, name, size in entries:
        if name in labels or not 1 <= size <= 8:
            continue
        pattern = re.compile(r"(?<![\w.$])" + re.escape(name) + r"(?![\w.$])")
        mentions = [i for i, line in enumerate(stripped) if pattern.search(line) and i not in extern_indices]
        if not mentions or any(mention not in la_lines for mention in mentions):
            continue
        if min(mentions) >= index:
            continue  # directive already precedes its first use
        hoisted.add(name)
    if not hoisted:
        return assembly
    moved = [line for index, name, _ in entries if name in hoisted for line in [lines[index]]]
    body = [line for index, line in enumerate(lines)
            if not (index in extern_indices and extern_line.match(stripped[index]).group(1) in hoisted)]
    insert_at = next((i for i, line in enumerate(stripped) if line.strip() == ".text"), 0)
    return "".join(body[:insert_at] + moved + body[insert_at:])


def main(argv):
    if len(argv) not in (4, 5):
        raise SystemExit("usage: padless-asm.py normalize|finish IN OUT [POLICY]")
    mode, source, destination = argv[1:4]
    policy = argv[4] if len(argv) == 5 else "none"
    data = open(source, "rb").read()
    if mode == "normalize":
        assembly = normalize_aliases(data.decode())
        if policy == "at-store":
            assembly = apply_at_store_policy(assembly)
        elif policy == "la-gprel":
            assembly = apply_la_gprel_policy(assembly)
        elif policy != "none":
            raise SystemExit("unknown assembler policy: " + policy)
        open(destination, "w").write(assembly)
    elif mode == "finish":
        open(destination, "wb").write(add_empty_sections(unpad(data)))
    else:
        raise SystemExit("unknown mode: " + mode)


if __name__ == "__main__":
    main(sys.argv)
'''


def clean(config_dir: Path):
    for file in (
        ".splache",
        "build.ninja",
        ".ninja_log",
        "permuter_settings.toml",
        "objdiff.json",
        "undefined_syms_auto.txt",
        "padless-asm.py",
        LD_PATH,
    ):
        (config_dir / file).unlink(missing_ok=True)

    for folder in ("asm", "assets", "build", "expected"):
        shutil.rmtree(config_dir / folder, ignore_errors=True)


def write_permuter_settings(config_dir: Path, compiler_cmd: str):
    with open(config_dir / "permuter_settings.toml", "w", encoding="utf-8") as f:
        f.write(
            f"""compiler_command = "{compiler_cmd} -D__GNUC__"
assembler_command = "{CROSS}as -march=r5900 -mabi=eabi -Iinclude"
compiler_type = "gcc"

[preserve_macros]

[decompme.compilers]
"tools/cc/{COMPILER}/bin/ee-gcc" = "{COMPILER}"
"""
        )


def build_stuff(
    config_dir: Path,
    config: dict[str, Any],
    linker_entries: list[LinkerEntry],
):
    src_path = Path(config["options"]["src_path"])
    compile_cmd, common_includes = make_compiler_cmd(config_dir, src_path)

    elf_bytes = b""
    retail_elf = config_dir / BASENAME
    if retail_elf.is_file():
        try:
            elf_bytes = retail_elf.read_bytes()
        except OSError:
            elf_bytes = b""

    built_objects: Set[Path] = set()

    def build(
        object_paths: Union[Path, list[Path]],
        src_paths: list[Path],
        task: str,
        variables: dict[str, str] = {},
        implicit_outputs: list[str] = [],
    ):
        if not isinstance(object_paths, list):
            object_paths = [object_paths]

        object_paths = [Path(str(entry).replace("$", "$$")) for entry in object_paths]
        src_paths = [Path(str(entry).replace("$", "$$")) for entry in src_paths]

        object_strs = [str(obj) for obj in object_paths]

        for object_path in object_paths:
            if object_path.suffix == ".o":
                built_objects.add(object_path)

            ninja.build(
                outputs=object_strs,
                rule=task,
                inputs=[str(s) for s in src_paths],
                variables=variables,
                implicit_outputs=implicit_outputs,
            )

    ninja = ninja_syntax.Writer(open(str(config_dir / "build.ninja"), "w"), width=9999)

    overlap_flag = "--no-check-sections " if config.get("_retail_link_layout") else ""
    ld_args = f"--no-warn-rwx-segments {overlap_flag}-EL -T undefined_syms.txt -T undefined_syms_auto.txt -T undefined_funcs_auto.txt -Map $mapfile -T $in -o $out"

    cpp = get_compiler_command("cpp")
    rel_root = Path(os.path.relpath(ROOT, config_dir))

    ninja.rule(
        "as",
        description="as $in",
        command=(
            f"{rel_root}/{cpp} {common_includes} {LANG_DEFINE} "
            f"$in -o - | "
            f"{CROSS}as -no-pad-sections -EL -march=5900 -mabi=eabi -I{src_path.parent / 'include'} -o $out"
        ),
    )

    ninja.rule(
        "cc",
        description="cc $in",
        command=f"{compile_cmd} $in $extra -o $out && {CROSS}strip $out -N dummy-symbol-name -R .mdebug",
    )

    if not game_compiler_configured():
        # The reconstructed game compiler is mandatory, not optional.  Without
        # it, every unit in GAME_COMPILER_UNITS silently falls through to
        # another rule (patched/SN/EE-GCC 2.9), which changes codegen for 297
        # units without any error - the repository ignores tools/, so a fresh
        # clone reroutes them unnoticed.  Failing here keeps the build honest
        # and makes the per-unit SN entries for cc_game owners unreachable, so
        # they can be deleted instead of sitting as dead configuration.
        raise SystemExit(
            "the reconstructed game compiler is required: "
            f"{_game_compiler_root()} has no ee-gcc/cc1.\n"
            "Install it at tools/compilers/game-compiler (the baseline "
            "workspace stages it under tools/cc/game-compiler) or point "
            "GAME_COMPILER_ROOT at it. The patch stack that rebuilds it from "
            "the pinned source archive is in patches/sce-991111b; see "
            "docs/building.md."
        )

    if game_compiler_configured():
        game_root = _game_compiler_root()
        ninja.rule(
            "cc_game",
            description="cc_game $in",
            command=(
                f"{game_root}/ee-gcc -c -I{game_root}/include {common_includes} "
                f"{LANG_DEFINE} {COMPILER_FLAGS} "
                f"$in $extra -o $out && {CROSS}strip $out -N dummy-symbol-name -R .mdebug"
            ),
        )

    if sn_compiler_configured():
        sn_root = Path(SN_TOOLCHAIN_ROOT)
        sn_repo = ROOT
        sn_driver = str(sn_root / "bin/ee-gcc.exe")
        sn_lib = _win_path(str(sn_root / "lib/gcc-lib/ee/2.95.2"))
        sn_eebin = _win_path(str(sn_root / "ee/bin"))
        sn_inc = _win_path(str(sn_root / "lib/gcc-lib/ee/2.95.2/include"))
        sn_repo_inc = _win_path(str(sn_repo / "include"))
        # The SN driver is a Windows PE: it can only read/write native
        # Windows paths, so this rule stages the source under the real
        # repository (a WSL mount) and copies the object back into the
        # staging build.
        ninja.rule(
            "cc_sn",
            description="cc_sn $in",
            command=(
                f"mkdir -p $sn_work && cp $in $sn_work/cand.c && "
                f"'{sn_driver}' -c '-B{sn_lib}\\' '-B{sn_eebin}\\' "
                f"-I'{sn_inc}' -I'{sn_repo_inc}' "
                f"-DBUILD_US_VERSION -DMATCHING_DECOMP -O2 -g2 $extra "
                f"'$sn_work_win/cand.c' -o '$sn_work_win/cand.o' && "
                f"cp $sn_work/cand.o $out && {CROSS}strip $out -N dummy-symbol-name -R .mdebug"
            ),
        )

        # SN cc1 + Ps2EeAs: the bundled GNU assembler drops
        # compiler-emitted hazard NOPs, while Ps2EeAs materializes them and
        # pads `.text` to its section alignment.  The generated helper rewrites
        # GNU alias assignments to labels and trims only that padding.
        (config_dir / "padless-asm.py").write_text(PADLESS_ASM_HELPER)
        ee_assembler = str(Path(SN_TOOLCHAIN_ROOT) / "ee/bin/Ps2EeAs.exe")
        ninja.rule(
            "cc_sn_padless",
            description="cc_sn_padless $in",
            command=(
                f"mkdir -p $sn_work && cp $in $sn_work/cand.c && "
                f"'{sn_driver}' -S '-B{sn_lib}\\' '-B{sn_eebin}\\' "
                f"-I'{sn_inc}' -I'{sn_repo_inc}' "
                f"-DBUILD_US_VERSION -DMATCHING_DECOMP -O2 -g2 $extra "
                f"'$sn_work_win/cand.c' -o '$sn_work_win/cand.s' && "
                f"{sys.executable} padless-asm.py normalize $sn_work/cand.s $sn_work/cand-final.s $policy && "
                f"'{ee_assembler}' -o '$sn_work_win/cand-padded.o' '$sn_work_win/cand-final.s' && "
                f"{sys.executable} padless-asm.py finish $sn_work/cand-padded.o $out && "
                f"{CROSS}strip $out -N dummy-symbol-name -R .mdebug"
            ),
        )

        # Patched public 991111 cc1 (R5900 quad saves + classic mult/mflo) with
        # the same alias normalization + Ps2EeAs + padless finish.  Native
        # driver, so only the assembler step needs Windows paths.
        if ee_gcc_patched_configured():
            patched_root = Path(EE_GCC_PATCHED_ROOT)
            patched_driver = str(patched_root / "xgcc")
            patched_include = str(ROOT / "include")
            ninja.rule(
                "cc_ee_gcc_patched",
                description="cc_ee_gcc_patched $in",
                command=(
                    f"mkdir -p $pat_work && cp $in $pat_work/cand.c && "
                    f"'{patched_driver}' -S -B'{patched_root}/' -I'{patched_include}' "
                    f"-DBUILD_US_VERSION -DMATCHING_DECOMP -O2 -g2 $extra "
                    f"$pat_work/cand.c -o $pat_work/cand.s && "
                    f"{sys.executable} padless-asm.py normalize $pat_work/cand.s $pat_work/cand-final.s $policy && "
                    f"'{ee_assembler}' -o '$pat_work_win/cand-padded.o' '$pat_work_win/cand-final.s' && "
                    f"{sys.executable} padless-asm.py finish $pat_work/cand-padded.o $out && "
                    f"{CROSS}strip $out -N dummy-symbol-name -R .mdebug"
                ),
            )

    ninja.rule(
        "ld",
        description="link $out",
        command=f"{CROSS}ld {ld_args} -T oracle-aliases.txt",
    )

    ninja.rule(
        "verify_boot",
        description="verify reconstructed boot ELF $in",
        command="cmp -s $in && touch $out",
    )

    # Fallback for patched-profile units when the profile is absent: build
    # them from the retail oracle instead of their C.
    ninja.rule(
        "oracle_obj",
        description="oracle $out",
        command="cp $in $out",
    )

    ninja.rule(
        "elf",
        description="elf $out",
        command=f"{CROSS}objcopy $in $out -O binary",
    )

    # The patched route needs both the profile and the SN assembler.
    patched_route = ee_gcc_patched_configured() and sn_compiler_configured()
    oracle_fallback_units: list[str] = []

    for entry in linker_entries:
        seg = entry.segment

        if seg.type[0] == ".":
            continue

        if entry.object_path is None:
            continue

        if isinstance(seg, splat.segtypes.common.c.CommonSegC):
            entry.src_paths = [
                Path("..", "..") / src_file for src_file in entry.src_paths
            ]
            unit = _unit_from_object(entry.object_path)
            style = "none"
            if sn_compiler_configured() and elf_bytes:
                vram = getattr(seg, "vram_start", None)
                if vram:
                    style = _retail_save_style(
                        elf_bytes, int(vram), max(int(seg.size or 0), 4)
                    )
            # Per-unit compiler by retail save style: sq/lq textbin code is
            # SN; sd/ld and save-less leaves stay on the inherited EE-GCC 2.9 pin
            # (they were matched there and SD-style textbin breaks under SN).
            use_sn = sn_compiler_configured() and (
                unit in SN_COMPILER_UNITS or (_unit_uses_sn(unit) and style == "sq")
            )
            use_patched = patched_route and unit in EE_GCC_PATCHED_UNITS
            use_game = game_compiler_configured() and unit in GAME_COMPILER_UNITS
            if use_game:
                game_extra = _unit_game_flag(unit)
                variables = {}
                if game_extra:
                    variables["extra"] = f"{game_extra} "
                build(entry.object_path, entry.src_paths, "cc_game", variables=variables)
            elif use_patched:
                pat_work = str(ROOT / "build/patched-work/units" / unit)
                flags = _unit_patched_flag(unit)
                variables = {
                    "pat_work": pat_work,
                    "pat_work_win": _win_path(pat_work),
                    "extra": f"{flags} " if flags else "",
                    "policy": _unit_policy(unit),
                }
                build(
                    entry.object_path,
                    entry.src_paths,
                    "cc_ee_gcc_patched",
                    variables=variables,
                )
            elif not patched_route and unit in EE_GCC_PATCHED_UNITS:
                # No patched profile: build from the retail oracle; the C is
                # verified when the profile is available.
                oracle_fallback_units.append(unit)
                build(
                    entry.object_path,
                    [Path("expected/obj") / f"{unit}.c.o"],
                    "oracle_obj",
                )
            elif sn_compiler_configured() and unit in PADLESS_ASM_UNITS:
                sn_work = str(sn_repo / "build/sn-work/units" / unit)
                sn_extra = _unit_sn_flag(unit)
                variables = {
                    "sn_work": sn_work,
                    "sn_work_win": _win_path(sn_work),
                    "policy": _unit_policy(unit),
                }
                if sn_extra:
                    variables["extra"] = f"{sn_extra} "
                build(
                    entry.object_path,
                    entry.src_paths,
                    "cc_sn_padless",
                    variables=variables,
                )
            elif use_sn:
                sn_work = str(sn_repo / "build/sn-work/units" / unit)
                sn_extra = _unit_sn_flag(unit)
                variables = {
                    "sn_work": sn_work,
                    "sn_work_win": _win_path(sn_work),
                }
                if sn_extra:
                    variables["extra"] = f"{sn_extra} "
                build(entry.object_path, entry.src_paths, "cc_sn", variables=variables)
            else:
                extra = _unit_flag(unit)
                variables = {"extra": f"{extra} "} if extra else {}
                build(entry.object_path, entry.src_paths, "cc", variables=variables)

        elif isinstance(
            seg,
            (
                splat.segtypes.common.asm.CommonSegAsm,
                splat.segtypes.common.data.CommonSegData,
                splat.segtypes.common.databin.CommonSegDatabin,
                splat.segtypes.common.rodatabin.CommonSegRodatabin,
                splat.segtypes.common.textbin.CommonSegTextbin,
                splat.segtypes.common.sbss.CommonSegSbss,
                splat.segtypes.common.bin.CommonSegBin,
            ),
        ):
            build(entry.object_path, entry.src_paths, "as")

        else:
            print(f"ERROR: Unsupported build segment type {seg.type}")
            sys.exit(1)

    fallback_path = config_dir / "oracle-fallback-units.json"
    alias_path = config_dir / "oracle-aliases.txt"
    alias_entries: set[tuple[str, str]] = set()
    if oracle_fallback_units:
        fallback_path.write_text(
            json.dumps(
                {
                    "schema": "rnc-oracle-fallback-v1",
                    "reason": "EE_GCC_PATCHED_ROOT is not configured",
                    "units": sorted(oracle_fallback_units),
                },
                indent=2,
            )
            + "\n"
        )
        for unit in oracle_fallback_units:
            source = (config_dir / ".." / ".." / "src" / f"{unit}.c").resolve()
            if source.is_file():
                alias_entries.update(_alias_symbols(source))
        print(
            f"EE_GCC_PATCHED_ROOT not configured: {len(oracle_fallback_units)} "
            "unit(s) will be built from the retail oracle "
            "(see docs/patched-toolchain.md)"
        )
    else:
        fallback_path.unlink(missing_ok=True)
    alias_lines = [
        "/* Generated by configure.py: aliases for oracle-built patched-profile",
        " * units, provided only when nothing else defines them. */",
    ]
    alias_lines.extend(
        f"PROVIDE({alias} = {target});" for alias, target in sorted(alias_entries)
    )
    alias_path.write_text("\n".join(alias_lines) + "\n")

    ninja.build(
        PRE_ELF_PATH,
        "ld",
        LD_PATH,
        implicit=[str(obj) for obj in built_objects],
        variables={"mapfile": MAP_PATH},
    )

    ninja.build(
        ELF_PATH,
        "elf",
        PRE_ELF_PATH,
    )

    ninja.build(
        ELF_PATH + ".ok",
        "verify_boot",
        [ELF_PATH, BASENAME],
    )

    write_permuter_settings(config_dir, compile_cmd)


def rename_locals(base_path: Path):
    for asm_file in base_path.rglob("*.s"):
        data = asm_file.read_text()
        data = re.sub(r"__local_\d+", "", data)
        asm_file.write_text(data)


def fix_gp_rel_stores(asm_root: Path) -> int:
    """Normalize the gp-relative store spelling for the frozen assembler.

    Splat emits ``sw $r, %gp_rel(sym)($28)`` for small-data stores.  The
    pinned EE 2.9 assembler rejects the ``%gp_rel`` operator on stores
    ("Bad expression"), while the hand-written oracles used
    ``.extern sym, 4`` plus a bare ``sym`` operand, which expands to the
    same R_MIPS_GPREL16 relocation.  Only generated per-function
    ``expected/asm`` files are touched; expected objects come from the
    whole-unit ``.c.s`` output through binutils and are unaffected.
    """
    pattern = re.compile(
        r"(?P<indent>[ \t]*)(?P<op>sw|swc1|sd|sdc1|sh|sb)(?P<spacing>\s+)"
        r"(?P<reg>\$[a-z0-9]+),\s*%gp_rel\((?P<sym>[A-Za-z0-9_]+)\)\(\$28\)"
    )
    fixed = 0
    for asm_file in asm_root.rglob("*.s"):
        text = asm_file.read_text()
        symbols: list[str] = []

        def rewrite(match: re.Match[str]) -> str:
            symbol = match.group("sym")
            if symbol not in symbols:
                symbols.append(symbol)
            return (
                f"{match.group('indent')}{match.group('op')}{match.group('spacing')}"
                f"{match.group('reg')}, {symbol}"
            )

        updated = pattern.sub(rewrite, text)
        if updated == text:
            continue
        missing = [symbol for symbol in symbols if f".extern {symbol}," not in updated]
        if missing:
            updated = "".join(f".extern {symbol}, 4\n" for symbol in missing) + updated
        asm_file.write_text(updated)
        fixed += 1
    return fixed


def make_asm(config_path: Path, config: dict[str, Any]):
    with tempfile.TemporaryDirectory(dir=config_path, prefix="tmp_") as tmp_dir:
        tmp_path = Path(tmp_dir)

        yaml_path = tmp_path / "config.yaml"
        asm_path = tmp_path / "asm" / "nonmatchings"
        dst_path = (config_path / "expected" / "asm").resolve().relative_to(ROOT)

        if dst_path.exists():
            print(f"expected asm dir '{dst_path}' already exists")
            return

        config = copy.deepcopy(config)
        for key in (
            "target_path",
            "undefined_funcs_auto_path",
            "undefined_syms_auto_path",
            "symbol_addrs_path",
            "extensions_path",
        ):
            if config["options"].get(key):
                config["options"][key] = "../" + str(config["options"][key])
        config["options"]["asm_path"] = "asm"
        config["options"]["src_path"] = "src"
        config["options"]["build_path"] = "build"
        config["options"]["asset_path"] = "assets"

        new_segments: list[Any] = []
        segments: list[Any] = config["segments"]
        for segment in segments:
            if isinstance(segment, list):
                new_segments.append(segment)
            elif isinstance(segment, dict) and segment["name"] == "main":
                new_subsegments: list[Any] = []
                subsegments = cast(list[Any], segment["subsegments"])
                for subsegment in subsegments:
                    if isinstance(subsegment, list):
                        if subsegment[1] == "asm":
                            subsegment[1] = "c"
                        new_subsegments.append(subsegment)
                    elif isinstance(subsegment, dict):
                        subsegment["type"] = subsegment["type"].strip(".")
                        if subsegment["type"] == "rodata":
                            subsegment["type"] = ".rodata"
                        new_subsegments.append(subsegment)
                segment["subsegments"] = new_subsegments
                new_segments.append(segment)
        config["segments"] = new_segments

        with yaml_path.open(mode="w") as yaml_file:
            yaml.dump(config, yaml_file, default_flow_style=False)

        with suppress_stdout_stderr():
            split.main([yaml_path], modes=["all"], verbose=False)

        rename_locals(asm_path)

        dst_path.parent.mkdir(parents=True, exist_ok=True)
        shutil.copytree(asm_path, dst_path, dirs_exist_ok=True)

        rewritten = fix_gp_rel_stores(dst_path)
        if rewritten:
            print(f"normalized gp-relative stores in {rewritten} expected asm files")

        print(f"expected asm extracted to '{dst_path}'")

        for subseg in new_segments[1]["subsegments"]:
            if isinstance(subseg, list) and subseg[1] == "c":
                subseg[1] = "asm"
                subseg[2] += ".c"

        config["options"]["asm_jtbl_label_macro"] = "llabel"

        with yaml_path.open(mode="w") as yaml_file:
            yaml.dump(config, yaml_file, default_flow_style=False)

        shutil.rmtree(tmp_path / "asm")
        (tmp_path / ".splache").unlink()

        with suppress_stdout_stderr():
            split.main([yaml_path], modes=["all"], verbose=False)

        rename_locals(asm_path)

        dst_path = dst_path.parent / "obj"
        tmp_obj_path = tmp_path / "obj"
        tmp_asm_dir = tmp_path / "asm"

        rel_root = Path(os.path.relpath(ROOT, tmp_path))
        cpp = f"{rel_root}/{get_compiler_command('cpp')}"
        up_includes = " ".join(
            shlex.quote(f"-I{part}")
            for part in (f"{rel_root}/src", f"{rel_root}/include", "include")
        )

        for asm_file in tmp_asm_dir.rglob("*.c.s"):
            asm_file_rel = asm_file.relative_to(tmp_path)
            obj_file_rel = Path("obj") / asm_file.relative_to(tmp_asm_dir).with_suffix(
                ".o"
            )
            obj_file = tmp_obj_path / obj_file_rel.relative_to("obj")
            obj_file.parent.mkdir(parents=True, exist_ok=True)
            asm_include = shlex.quote(f"-Wa,-I{rel_root}/include")
            assembler_include = shlex.quote(f"-I{rel_root}/include")
            subprocess.run(
                f"{shlex.quote(cpp)} {up_includes} {asm_include} "
                f"{shlex.quote(str(asm_file_rel))} -o - | "
                f"{CROSS}as -no-pad-sections -EL -march=5900 -mabi=eabi "
                f"{assembler_include} -o {shlex.quote(str(obj_file_rel))}",
                shell=True,
                cwd=tmp_path,
            )

        shutil.copytree(tmp_obj_path, dst_path, dirs_exist_ok=True)

        print(f"expected obj built to '{dst_path}'")


def generate_objdiff_configuration(config_path: Path, config: dict[str, Any]):
    segments: list[Any] = config["segments"]

    tu_to_diff: list[str] = []

    for segment in segments:
        if not (isinstance(segment, dict) and segment["name"] == "main"):
            continue

        subsegments = cast(list[Any], segment["subsegments"])

        for subsegment in subsegments:
            if isinstance(subsegment, list):
                _, subs_type, subs_name = cast(tuple[int, str, str], subsegment)
            elif isinstance(subsegment, dict):
                subs_type = cast(str, subsegment["type"])
                subs_name = cast(str, subsegment["name"])
            else:
                raise RuntimeError("invalid subsegment type")

            if subs_type in ("asm", "c"):
                tu_to_diff.append(subs_name)

    units: list[dict[str, Any]] = []

    for tu_name in tu_to_diff:
        target_path = Path("expected", "obj", tu_name).with_suffix(".c.o")
        base_path = Path("build", "src", tu_name).with_suffix(".c.o")

        unit: dict[str, Any] = {
            "name": tu_name,
            "target_path": str(target_path),
            "base_path": str(base_path),
            "metadata": {"progress_categories": [OBJDIFF_CATEGORY["id"]]},
        }

        units.append(unit)

    objdiff_json: dict[str, Any] = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "true",
        "custom_args": [],
        "build_target": False,
        "build_base": False,
        "watch_patterns": [],
        "units": units,
        "progress_categories": [OBJDIFF_CATEGORY],
    }

    objdiff_path = config_path / "objdiff.json"

    with objdiff_path.open(mode="w") as fw:
        json.dump(objdiff_json, fw, indent=2)

    print(f"Wrote objdiff configuration ({len(units)} units) to {objdiff_path}")


def fix_assets(config_dir: Path, config: dict[str, Any]):
    asset_path = Path(config["options"]["asset_path"])
    asset_rel_path = (config_dir / asset_path).resolve().relative_to(ROOT)

    for asm_file in (config_dir / "asm").rglob("*.s"):
        data_asm: str = asm_file.read_text()
        data_asm, count = re.subn(
            rf'\.incbin "{asset_rel_path}/', '.incbin "assets/', data_asm
        )
        if count > 0:
            asm_file.write_text(data_asm)


def fix_linkerscript(config: dict[str, Any], linkerscript_path: Path):
    section_subalign = cast(dict[str, int], config.get("_section_subalign", {}))

    re_subsegment_line = re.compile(
        r"^(?P<indent> +)build/(?:asm|src)(?:/data)?/(?P<name>.*)\.[sc]\.o\(\.(?P<section>.+)\);$"
    )
    re_section_line = re.compile(
        r"^(?P<indent> +)\.(?P<section>[^ ]+) .* SUBALIGN\((?P<subalign>[0-9]+)\)$"
    )

    patched_lines: list[str] = []
    current_section_subalign = 8

    with open(linkerscript_path, mode="r") as fh:
        for line in fh:
            if match := re_subsegment_line.match(line):
                indent = cast(str, match["indent"])

                if match["section"] == "text":
                    patched_lines.append(
                        f"{indent}. = ALIGN(., {current_section_subalign});\n"
                    )

            if match := re_section_line.match(line):
                section = cast(str, match["section"])
                subalign = cast(str, match["subalign"])

                if section in section_subalign:
                    current_section_subalign = int(section_subalign[section])
                    current_subalign = f"SUBALIGN({subalign})"
                    fixed_subalign = f"SUBALIGN({section_subalign[section]})"
                    line = line.replace(current_subalign, fixed_subalign)
                else:
                    current_section_subalign = int(subalign)

            patched_lines.append(line)

    with open(linkerscript_path, mode="w") as fh:
        fh.writelines(patched_lines)

    if config.get("_retail_link_layout"):
        apply_retail_link_layout(config, linkerscript_path)


def apply_retail_link_layout(config: dict[str, Any], linkerscript_path: Path):
    """Place the preserved inputs at their retail file offsets.

    The linked ELF is an intermediate container.  Objcopy uses these LMAs to
    reconstruct the retail boot ELF, beginning with the preserved 0x1000-byte
    ELF header and ending with the preserved section metadata.

    ``RODATA_OVERLAYS`` lets a recovered C unit own the small `.rodata` that
    originally lived inside the preserved ``core_rdata`` region.  The compiled
    object's ``.rodata`` section is placed at the retail VMA/file offset the
    bytes occupied; identical content makes the output byte-identical while the
    source no longer depends on the preserved blob for those bytes.
    """

    overlays = {name: (vram, at) for name, (vram, at) in RODATA_OVERLAYS.items()}
    text = linkerscript_path.read_text()
    entry_re = re.compile(
        r"^\s*(build/(?:asm|src)(?:/data)?/.*\.[sc]\.o\(\.text\);)$",
        re.MULTILINE,
    )
    text_entries = entry_re.findall(text)
    if not text_entries or text_entries[0] != "build/asm/data/vutext.s.o(.text);":
        raise ValueError("generated linker script has an unexpected text layout")

    main_segment = next(
        segment
        for segment in config["segments"]
        if isinstance(segment, dict) and segment.get("name") == "main"
    )
    text_offsets = [
        int(segment[0])
        for segment in main_segment["subsegments"]
        if len(segment) >= 2 and segment[1] in ("c", "textbin")
    ]
    if len(text_entries) != len(text_offsets):
        raise ValueError(
            f"generated linker script has {len(text_entries)} text entries, "
            f"but config has {len(text_offsets)} text subsegments"
        )

    text_sections = []
    for index, (entry, file_offset) in enumerate(zip(text_entries, text_offsets)):
        vram = file_offset - 0x1000 + 0x100080
        if index == 0:
            name = ".vutext"
        elif file_offset < 0x30400:
            name = f"core.text.{index:04d}"
        else:
            name = f".text.{index:04d}"
        # Three ranges intentionally overlap their predecessor in the source
        # map.  Put that short chain in a second PHDR so ld preserves the AT()
        # values instead of moving later sections forward.
        alternate_lane_offsets = {0x192B8, 0x262DC, 0x262E0}
        phdr = "text_alt" if file_offset in alternate_lane_offsets else "text"
        text_sections.append(
            f"    {name} 0x{vram:X} : AT(0x{file_offset:X}) SUBALIGN(4)\n"
            "    {\n"
            f"        {entry}\n"
            f"    }} :{phdr}"
        )
    linked_text = "\n\n".join(text_sections)

    # Recovered C units may own small .rodata regions that retail kept inside
    # the preserved core_rdata blob.  Emit an overlay section for each
    # configured unit that matches the overlay map, keyed by the configured
    # unit name so both the assembly-backed and the normalized/promoted name
    # resolve to the same retail bytes.
    c_units: list[str] = []
    for segment in config["segments"]:
        if not (isinstance(segment, dict) and segment.get("name") == "main"):
            continue
        for subsegment in segment.get("subsegments", []):
            if (
                isinstance(subsegment, list)
                and len(subsegment) >= 3
                and subsegment[1] == "c"
            ):
                c_units.append(str(subsegment[2]))
    rodata_overlay_sections = []
    for unit in c_units:
        for suffix, (vram, at) in overlays.items():
            if unit.endswith(suffix):
                rodata_overlay_sections.append(
                    f"    {suffix.lstrip('_')}.rdata 0x{vram:X} : AT(0x{at:X}) SUBALIGN(4)\n"
                    "    {\n"
                    f"        build/src/{unit}.c.o(.rodata);\n"
                    "    } :data_alt"
                )
    rodata_overlay = (
        "\n\n".join(rodata_overlay_sections) if rodata_overlay_sections else ""
    )
    script = f"""ENTRY(entry)
PHDRS
{{
    header PT_LOAD FLAGS(6);
    text PT_LOAD FLAGS(5);
    text_alt PT_LOAD FLAGS(5);
    data PT_LOAD FLAGS(6);
    data_alt PT_LOAD FLAGS(6);
    tail PT_LOAD FLAGS(6);
}}

SECTIONS
{{
    _gp = 0x166C00;

    .elf_header 0 : AT(0) SUBALIGN(8)
    {{
        build/asm/data/elf_header.s.o(.data);
    }} :header

{linked_text}

{rodata_overlay}

    core.data 0x12F480 : AT(0x30400) SUBALIGN(4)
    {{
        build/asm/data/core_data.s.o(.data);
    }} :data

    core.rdata 0x152200 : AT(0x53180) SUBALIGN(4)
    {{
        build/asm/data/core_rdata.s.o(.data);
    }} :data

    core.bss 0x154100 (NOLOAD) : SUBALIGN(4)
    {{
        build/asm/data/core_bss.bss.s.o(.bss);
    }} :data

    core.lit 0x15EC80 : AT(0x5FC00) SUBALIGN(4)
    {{
        build/asm/data/core_lit.s.o(.data);
    }} :data

    .lit 0x15EF00 : AT(0x5FE80) SUBALIGN(4)
    {{
        build/asm/data/lit.s.o(.data);
    }} :data

    .bss 0x161280 (NOLOAD) : SUBALIGN(4)
    {{
        build/asm/data/bss.bss.s.o(.bss);
    }} :data

    .data 0x165480 : AT(0x66400) SUBALIGN(4)
    {{
        build/asm/data/data.s.o(.data);
    }} :data

    lvl.vtbl 0x1E8B80 : AT(0xE9B00) SUBALIGN(4)
    {{
        build/asm/data/lvl_vtbl.s.o(.data);
    }} :data

    lvl.camvtbl 0x1E8C00 : AT(0xE9B80) SUBALIGN(4)
    {{
        build/asm/data/lvl_camvtbl.s.o(.data);
    }} :data

    lvl.sndvtbl 0x1E8C80 : AT(0xE9C00) SUBALIGN(4)
    {{
        build/asm/data/lvl_sndvtbl.s.o(.data);
    }} :data

    .reg_info 0x300000 : AT(0x13E2E0) SUBALIGN(4)
    {{
        build/asm/data/reg_info.s.o(.data);
    }} :tail

    .dvp_overlays 0x300018 : AT(0x13E2F8) SUBALIGN(4)
    {{
        build/asm/data/dvp_overlays.s.o(.data);
    }} :tail

    .dvp_overlay_string_table 0x312080 : AT(0x150360) SUBALIGN(4)
    {{
        build/asm/data/dvp_overlay_string_table.s.o(.data);
    }} :tail

    .section_strings 0x3127F7 : AT(0x150AD7) SUBALIGN(1)
    {{
        build/asm/data/section_strings.s.o(.data);
    }} :tail

    /DISCARD/ :
    {{
        *(*);
    }}
}}
"""
    linkerscript_path.write_text(script)


def main():
    class ArgsProtocol:
        YAML_FILE: Path
        clean: bool
        make_asm: bool

    parser = argparse.ArgumentParser(description="Configure the project")
    parser.add_argument(
        "YAML_FILE",
        type=Path,
        nargs="?",
        default=Path("config/us/rnc1.us.yaml"),
        help="yaml file to configure the project",
    )
    parser.add_argument(
        "-c",
        "--clean",
        help="Clean extraction and build artifacts",
        action="store_true",
    )
    parser.add_argument(
        "--make-asm",
        help="Extract assembly for each function into 'expected/' subfolder",
        action="store_true",
    )
    args = cast(ArgsProtocol, parser.parse_args())

    config = splat_load_yaml(
        [args.YAML_FILE],
        modes=["all"],
        verbose=False,
        disassemble_all=False,
    )
    validate_config_names(config["segments"])

    basename = config["options"]["basename"]
    config_dir = Path(args.YAML_FILE).parent

    if basename not in LANGUAGES:
        supported_elfs = (
            f"{set(f'{elf} ({lang})' for elf, lang in LANGUAGES.items())}".replace(
                "'", ""
            )
        )
        print(f"unsupported game ELF. Supported versions are: {supported_elfs}")
        exit(1)

    if args.clean:
        clean(config_dir)

    split.main([args.YAML_FILE], modes=["all"], verbose=False)

    fix_assets(config_dir, config)

    linkerscript_path = (config_dir / LD_PATH).resolve().relative_to(ROOT)
    assert linkerscript_path.is_file(), f"{linkerscript_path} not found"

    fix_linkerscript(config, linkerscript_path)

    linker_entries = split.linker_writer.entries

    build_stuff(config_dir, split.config, linker_entries)

    if args.make_asm:
        make_asm(config_dir, config)

    generate_objdiff_configuration(config_dir, config)


if __name__ == "__main__":
    main()
