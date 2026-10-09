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
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any, Set, Union, cast

import ninja_syntax
import yaml

import splat
import splat.scripts.split as split
from splat.segtypes.linker_entry import LinkerEntry
from splat.util.conf import load as splat_load_yaml

ROOT = Path.cwd()

# —— Toolchain and paths ——

# The two compilers of the retail build: tools/compilers/game-compiler (the
# reconstructed Sony/Cygnus 2.9-ee-991111b, game code) and
# tools/compilers/sdk-compiler (the vendored EE-GCC 2.9-991111-01, the SDK
# libraries).  The build stages tools/compilers as tools/cc.
SDK_COMPILER = "sdk-compiler"

# decomp.me id of the SDK compiler, for the permuter settings.
SDK_COMPILER_DECOMPME = "ee-gcc2.9-991111-01"

CROSS = "mips-ps2-decompals-"

COMPILER_FLAGS = "-DMATCHING_DECOMP -O2 -g2 -gstabs"

LANG_DEFINE = "-DBUILD_US_VERSION"

# Overrides for the game compiler location (default tools/compilers/game-compiler,
# rebuilt from patches/sce-991111b).
GAME_COMPILER_ROOT = os.environ.get("GAME_COMPILER_ROOT", "").strip()

# Retiring toolchains, needed only by ROUTE_EXCEPTIONS below: the SN tree
# (cc_sn, cc_sn_padless and the Ps2EeAs assembler) and the locally built
# patched 991111 cc1 (cc_ee_gcc_patched, docs/patched-toolchain.md).
SN_TOOLCHAIN_ROOT = os.environ.get("SN_TOOLCHAIN_ROOT", "").strip()

EE_GCC_PATCHED_ROOT = os.environ.get("EE_GCC_PATCHED_ROOT", "").strip()

LANGUAGES = {
    "SCUS_971.99": "us",
}

BASENAME = "SCUS_971.99"

LD_PATH = f"{BASENAME}.ld"

# The script ld actually runs: LD_PATH with every per-object `.text` statement
# rewritten to a unique input-section name (see write_fast_linkerscript).
FAST_LD_PATH = f"{BASENAME}.fast.ld"

ELF_PATH = f"build/{BASENAME}"

MAP_PATH = f"build/{BASENAME}.map"

PRE_ELF_PATH = f"build/{BASENAME}.elf"

OBJDIFF_CATEGORY = {"id": "us", "name": "Ratchet & Clank (USA)"}

OVERLAYS_SRC = Path("src/overlays")

OVERLAYS_BUILD = Path("build/overlays")

# —— Compiler routes ——

# The retail executable links the SDK libraries (newlib, libkernl, libsif,
# libcdvd, libmpeg, ...) as one block ahead of the game code, and the two
# blocks were built by different compilers: the SDK libraries by sdk-compiler,
# the game by game-compiler.  A unit's compiler is therefore a property of where
# retail placed it, not of which compiler happened to match it first.
#
# Measured 2026-09-27 on a clean gate workspace with the common configuration
# (no per-unit flags): 1258 of 1331 C units build byte-identically on the
# compiler this rule gives them.
GAME_TEXT_START = 0x12D8F8  # first game function; the SDK block ends at 0x12D8F0

# Units that do not build byte-identically on their provenance compiler yet,
# mapped to the build.ninja rule that still reproduces them.  This table is the
# remaining debt of the compiler convergence: an entry is deleted once its unit
# builds on the provenance compiler with the common configuration; new entries
# are not added.
ROUTE_EXCEPTIONS = {
    # Game code still built by the SN compiler (cc1 2.95.2).  SN is not a compiler of
    # the retail build; these are the units the game compiler does not reproduce
    # yet with the common configuration.
    "audio/rpc/snd_post_message": "cc_sn",
    # snd_send_current_batch: send the current sound command batch over SIF RPC
    # and flip to the other buffer
    "audio/rpc/snd_send_current_batch": "cc_sn",
    # fun_00208030: expand a 4bpp coverage map through the 16-entry weight table
    # into a 1bpp threshold mask (4 source rows per output row)
    "ui/menus/fun_00208030": "cc_sn",
    "ui/menus/update_menu_cycle_selection": "cc_sn",
    # Game code still built by SN cc1 plus the SN assembler Ps2EeAs (the padless route).
    "rendering/packets/emit_rgba_draw_packet": "cc_sn_padless",
    # parse_particle_textures: The a1/a3 induction-pointer swap was the ORDER OF
    # INCREMENTS: p is a walked front-end pointer read through *p, p = p + 1
    # sits in the for-increment clause after i = i + 1 so the loop bottom RTL
    # orders [counter][p walk], and the table stays indexed so its base
    # materialises in the preheader. 45 earlier shapes had missed it.
    "rendering/texture/parse_particle_textures": "cc_sn_padless",
    # fun_00202800: load packed screen points: shift x/y, convert u/v, clear
    # flags
    "world/loaders/unpack_point_records": "cc_sn_padless",
    # fun_002028e0: Plain-C rewrite: the 8-byte sprite clear must be a struct
    # s64 field store (not a cast-pointer store) so reload.c coalesces the
    # post-call %hi/%lo reload into the loop-carried base copy, a separate index
    # variable for the group loop pins f->s3/i->s4, and `f->loaded = 1` before
    # the relocation stores fixes their schedule.
    "world/loaders/relocate_sky_definition": "cc_sn_padless",
    # set_up_vis_gif_viewer: Registered route is padless+none (native scores
    # only 87.8): the packet high word is (u64)(u32)n << 32 taken from the 2nd
    # argument instead of w1 >> 32, and 0x20 is OR-ed with (w1 & 0x1C) in the
    # mode>=0 branch but with (prim << 6) in the two negative branches.
    "rendering/set_up_vis_gif_viewer": "cc_sn_padless",
    "gameplay/animation/update_moby_animation_state": "cc_sn_padless",
    # patch_moby_gifs: patch moby class GIF tex words through the texture remap
    # table
    "gameplay/entities/patch_moby_gifs": "cc_sn_padless",
    # fun_00221460: A dead `p = m->items;` statement that cc1 deletes still
    # perturbs the local hard-register order into retail's, and declaring
    # func_001F6530 void removes the unused-return pseudo so its argument copies
    # emit in retail's order a2<-s0, a3<-v0, a1<-s2. NOTE:
    # src/assembly/textbin/fun_001fd748.c still declares that callee as s32 in
    # another translation unit.
    "ui/menus/fun_00221460": "cc_sn_padless",
    # fun_00232d00: bind the stash RPC server, read its IOP buffer and reset the
    # stash slots
    "storage/cd/fun_00232d00": "cc_sn_padless",
    # SDK code still built by the patched 991111 compiler (plus the SN assembler).
    # Retail uses classic mult/mflo; the frozen trees emit the R5900 rd-form.
    # 100/100/100 + patha linked-byte equal (0x12D3A0), 2026-09-12.
    "sdk/time/bcd_to_time": "cc_ee_gcc_patched",
    # _pictureCodingExtension: the absolute IPU_CTRL volatile stores fill the
    # _nextBit call delay slots through $at (lui $1 before the call, sw in
    # the slot), which the patched profile does for every unit.
    "sdk/library/picturecodingextension": "cc_ee_gcc_patched",
}

# Per-unit assembler policies applied by the generated padless-asm.py helper.
PADLESS_POLICY_UNITS = {}

# Per-unit extra compiler flags for the native EE-GCC 2.9 units whose
# exact codegen requires a different scheduling model.  Keyed by the configured
# owner path (exact match): a unit renamed or moved out of assembly/ must have
# its entry renamed with it.
# sce_sif_init_iop_heap: retail tail (lui v0; sw; move v0) is byte-exact only
# under -fno-schedule-insns; applying it globally to all EE-GCC 2.9 units changes
# scePad2Read and other already-exact siblings.
SDK_COMPILER_FLAG_UNITS = {
    "sdk/rpc/sce_sif_init_iop_heap": "-fno-schedule-insns",
    # No -fno-edge-lcm entry remains: the six that did (draw_debug_profiler,
    # fun_0022f778, draw_dialog_text, memcard_update_state, sound_update,
    # setup_fs_aa_buffer) belong to units that are still assembly wrappers,
    # where the option cannot change a byte. Those units build on the 991111-01
    # route, where -fno-edge-lcm is a real requirement for their C once a body
    # replaces the wrapper: re-add it then, with the measurement and the reason
    # recorded in this comment.
}

# Per-unit extra flags for GAME_COMPILER_UNITS (exact owner path, as SN_FLAG_UNITS).
GAME_COMPILER_FLAG_UNITS = {
}

SN_FLAG_UNITS = {
    # snd_post_message: retail keeps the index in v1 and the base in v0; the
    # default prepass scheduler swaps them.  100/100/100 with
    # -fno-schedule-insns (pipeline-2026-09-13-11 wave 2).
    "audio/rpc/snd_post_message": "-fno-schedule-insns",
}

# Level overlay units (src/overlays/<dir>/<file>) built with SN cc1 2.95.2
# instead of the game compiler: retail level code matches SN where the game
# compiler cannot (the hero giants' 128-bit zero stores, reload registers).
# Add a unit only when every C function in it stays exact under SN.
# Kept sorted, one per line: scripts/sn/tryfn.py edits this block.
OVERLAY_SN_UNITS = {
    "l01/gameplay/entities/002b96e0.c",
    "l01/gameplay/entities/002f9810.c",
    "l01/gameplay/hero/00233de0.c",
    "l01/gameplay/hero/0023cf98.c",
    "l01/gameplay/hero/00242930.c",
    "l02/gameplay/entities/002400c8.c",
    "l02/gameplay/entities/002e0dc0.c",
    "l02/gameplay/hero/0021b698.c",
    "l02/gameplay/hero/0022b728.c",
    "l02/gameplay/vendor/002ebf20.c",
    "l03/gameplay/entities/00292578.c",
    "l03/gameplay/entities/002c9eb8.c",
    "l03/gameplay/hero/00205830.c",
    "l03/gameplay/hero/00216648.c",
    "l03/gameplay/hero/0021c668.c",
    "l03/runtime/startup/0022c728.c",
    "l04/gameplay/entities/001f3038.c",
    "l04/gameplay/entities/0024c4f0.c",
    "l04/gameplay/entities/0029eb20.c",
    "l04/gameplay/entities/002ca420.c",
    "l05/gameplay/entities/002d1688.c",
    "l05/gameplay/entities/0030d6a0.c",
    "l05/gameplay/hero/00239fc0.c",
    "l05/gameplay/hero/00244a70.c",
    "l05/gameplay/hero/0024cee8.c",
    "l05/gameplay/hero/00255960.c",
    "l06/gameplay/entities/002b4770.c",
    "l06/gameplay/entities/002fd1a0.c",
    "l06/gameplay/hero/002356a0.c",
    "l06/gameplay/hero/0023b440.c",
    "l07/gameplay/entities/002cd2b0.c",
    "l07/gameplay/entities/00312948.c",
    "l07/gameplay/vendor/0031d2e0.c",
    "l07/ui/map/00270248.c",
    "l08/gameplay/entities/002b8228.c",
    "l08/gameplay/entities/002deee0.c",
    "l08/gameplay/entities/002e9b70.c",
    "l08/gameplay/hero/00230b38.c",
    "l08/gameplay/hero/002370d8.c",
    "l08/gameplay/vendor/00305138.c",
    "l08/rendering/00279f00.c",
    "l09/gameplay/entities/0021e538.c",
    "l10/gameplay/entities/00295a38.c",
    "l10/gameplay/vendor/002df270.c",
    "l10/gameplay/vendor/002ea1f0.c",
    "l11/gameplay/entities/002cb668.c",
    "l11/gameplay/hero/0023c7a0.c",
    "l11/gameplay/hero/0024db50.c",
    "l11/gameplay/hero/00253a18.c",
    "l11/gameplay/vendor/0030c788.c",
    "l11/gameplay/vendor/00315968.c",
    "l12/gameplay/entities/002bf140.c",
    "l12/gameplay/entities/002ec720.c",
    "l12/gameplay/hero/0022de30.c",
    "l12/gameplay/hero/002400d0.c",
    "l12/gameplay/hero/002461f0.c",
    "l12/gameplay/vendor/003028c8.c",
    "l13/gameplay/entities/002c13b0.c",
    "l13/gameplay/entities/002ea8c8.c",
    "l13/gameplay/vendor/003058a8.c",
    "l13/rendering/002b8320.c",
    "l14/gameplay/entities/002460f8.c",
    "l14/gameplay/entities/002d6358.c",
    "l14/gameplay/entities/002df080.c",
    "l14/gameplay/entities/002fded0.c",
    "l14/gameplay/hero/0022ff58.c",
    "l14/gameplay/hero/00235600.c",
    "l15/gameplay/entities/0029aff0.c",
    "l15/gameplay/hero/002044c8.c",
    "l15/gameplay/hero/00216c38.c",
    "l15/gameplay/hero/0021d040.c",
    "l15/gameplay/vendor/002e73c0.c",
    "l16/gameplay/entities/002a3f38.c",
    "l16/gameplay/entities/002cfc00.c",
    "l16/gameplay/hero/002097a0.c",
    "l16/gameplay/hero/0021e398.c",
    "l16/gameplay/hero/002270c8.c",
    "l16/gameplay/vendor/002e3190.c",
    "l17/gameplay/entities/002a8da0.c",
    "l17/gameplay/hero/0021e530.c",
    "l17/gameplay/hero/002258a8.c",
    "l17/gameplay/vendor/002e87d8.c",
    "l17/gameplay/vendor/002ea418.c",
    "l17/gameplay/vendor/002f26d0.c",
    "l17/ui/help/002020a8.c",
    "l18/gameplay/entities/002a7220.c",
    "l18/gameplay/hero/00227dd0.c",
    "l18/gameplay/hero/0022e8f8.c",
    "l18/gameplay/vendor/002efb88.c",
    "l18/gameplay/vendor/002f88e8.c",
    "shared/audio/voices/00276368.c",
    "shared/gameplay/animation/00235878.c",
    "shared/gameplay/camera/001eb188.c",
    "shared/gameplay/camera/001fc008.c",
    "shared/gameplay/entities/00278fd8.c",
    "shared/gameplay/entities/00291918.c",
    "shared/gameplay/entities/002937a0.c",
    "shared/gameplay/entities/00295100.c",
    "shared/gameplay/entities/00297d10.c",
    "shared/gameplay/entities/0029f990.c",
    "shared/gameplay/entities/002a09a0.c",
    "shared/gameplay/entities/002a4038.c",
    "shared/gameplay/entities/002aa670.c",
    "shared/gameplay/entities/002aee30.c",
    "shared/gameplay/entities/002b17d8.c",
    "shared/gameplay/entities/002b2100.c",
    "shared/gameplay/entities/002b3840.c",
    "shared/gameplay/entities/002b8c08.c",
    "shared/gameplay/entities/002b94d0.c",
    "shared/gameplay/entities/002bffa8.c",
    "shared/gameplay/entities/002c8440.c",
    "shared/gameplay/entities/002c8830.c",
    "shared/gameplay/entities/002cfcb8.c",
    "shared/gameplay/entities/002d6810.c",
    "shared/gameplay/entities/002d7f88.c",
    "shared/gameplay/entities/002f6328.c",
    "shared/gameplay/hero/00221310.c",
    "shared/gameplay/hero/00229b70.c",
    "shared/gameplay/state/0024eec0.c",
    "shared/gameplay/state/0027b268.c",
    "shared/gameplay/vendor/002e22d8.c",
    "shared/gameplay/vendor/002e3de8.c",
    "shared/gameplay/vendor/002e5e38.c",
    "shared/gameplay/vendor/003015d0.c",
    "shared/gameplay/vendor/0030b618.c",
    "shared/gameplay/vendor/0030e690.c",
    "shared/gameplay/vendor/00316f48.c",
    "shared/math/interpolation/00257ef0.c",
    "shared/math/interpolation/0026d930.c",
    "shared/math/rotations/002899f8.c",
    "shared/math/vectors/0025c230.c",
    "shared/rendering/00269290.c",
    "shared/rendering/002712b8.c",
    "shared/rendering/0027f660.c",
    "shared/rendering/commands/0020bc88.c",
    "shared/rendering/sky/00288ec0.c",
    "shared/ui/help/001fe778.c",
    "shared/ui/help/0021d0a0.c",
    "shared/ui/help/00231d08.c",
    "shared/ui/help/00237488.c",
    "shared/ui/menus/002497f8.c",
    "shared/ui/menus/0027f448.c",
    "shared/ui/text/001fb470.c",
    "shared/ui/text/002377b8.c",
}

# —— Retail link layout ——

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
    "sdk/debug/printfloat": (0x152798, 0x53718),  # its three f64 literals (0.1, 0.1, 1e6)
    "fun_0021fdc8": (0x1E87F0, 0xE9770),  # stream-state switch table
    # vfprintf_r: blanks/zeroes, the xdigs strings, the short literals (kept in
    # .rodata via section attributes, in source order) and its switch table.
    "runtime/newlib/vfprintf_r": (0x1524E0, 0x53460),
    "dispatch_game_state_update": (0x1E8960, 0xE98E0),  # retail switch table
    "gameplay/missions/check_mission_condition": (0x1E8390, 0xE9310),  # unlock-condition switch table
    "draw_menu_preview_objects": (0x1E87A0, 0xE9720),  # item-handle release switch table
    "update_resident_gameplay_state": (0x1E8930, 0xE98B0),  # gameplay-state switch table
    "fun_00222768": (0x1E8860, 0xE97E0),  # switch table
    "camera_activation_check_priority": (0x1E7730, 0xE86B0),  # camera-mode switch table
    "ui/help/draw_help": (0x1E7A70, 0xE89F0),  # switch table (PAL import)
    "ui/help/dismiss_help": (0x1E7A20, 0xE89A0),  # switch table (PAL import)
    # A switch's jump table is a literal pool that retail placed at a fixed VMA
    # (jtbl_001528E0 = 0x1528E0); the expected object references it by that
    # splat symbol, so the compiled .rodata has to land at the same VMA and
    # file offset for the relocation to resolve content-equal. Same shape as
    # _getpic above. This only fixes the PLACEMENT, so it unblocks objdiff
    # pairing; it does not by itself make the unit match.
    "_sceFs_Rcv_Intr": (0x1528E0, 0x53860),  # retail switch table (jtbl_001528E0)
    "fun_00216c48": (0x1E86A0, 0xE9620),  # retail switch table (jtbl_001E86A0)
    "draw_sky_shells": (0x1E8910, 0xE9890),  # switch table
    "fun_002223f0": (0x1E8810, 0xE9790),  # switch table
    "fun_00237ed0": (0x1E8A90, 0xE9A10),  # switch table
    "update_help_state": (0x1E7A40, 0xE89C0),  # switch table
    "memcard_update_state": (0x1E8200, 0xE9180),  # switch table
    "init_once": (0x1E7B30, 0xE8AB0),  # switch table
    "fun_001e8d08": (0x1E7640, 0xE85C0),  # switch table
    "fun_00213928": (0x1E84E0, 0xE9460),  # switch table
    "fun_00204428": (0x1E7CE0, 0xE8C60),  # switch table
    "mode_freeze_init": (0x1E78D0, 0xE8850),  # switch table
    "fun_0021abf8": (0x1E8770, 0xE96F0),  # menu action switch table
    "draw_dialog_text": (0x1E7920, 0xE88A0),  # switch table
}

# Recovered C units that define the small-data variables their original
# translation unit owned.  Retail reaches such a variable gp-relative only
# where the assembler already knew its size, i.e. after the definition in
# the same file; a unit that defines it reproduces that and needs no
# `.extern`.  Key: configured unit-name suffix; value: (retail VMA, retail
# file offset) of the unit's `.sdata`, which retail kept inside the preserved
# small-data blobs (core.lit / .lit).  Placed like RODATA_OVERLAYS.
SDATA_OVERLAYS = {
    "audio/rpc/snd_returns": (0x15EC80, 0x5FC00),
    "rendering/debug/print_debug_text": (0x15F000, 0x5FF80),
    "runtime/resources/update_resource_counter": (0x15F8F8, 0x60878),
    "rendering/vu1_chain": (0x160EE0, 0x61E60),
}

# —— Code ——

def provenance_compiler(vram: int) -> str:
    return "sdk-compiler" if vram < GAME_TEXT_START else "game-compiler"

def _is_include_asm(config_dir: Path, source: Path) -> bool:
    """A pending unit: its C file only wraps splat's listing."""
    try:
        return "INCLUDE_ASM" in (config_dir / source).read_text(errors="replace")
    except OSError:
        return False

def unit_compiler(unit: str, vram: int) -> str:
    """The build.ninja rule a unit is compiled with."""
    return ROUTE_EXCEPTIONS.get(unit) or provenance_compiler(vram)

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
    compiler_dir = Path("tools") / "cc" / SDK_COMPILER
    ee_dir = compiler_dir / "lib" / "gcc-lib" / "ee"
    ee_compiler_dirname = next(os.walk(ee_dir))[1][0]

    commands = {
        "ee-gcc": compiler_dir / "bin" / "ee-gcc",
        "cpp": compiler_dir / "lib" / "gcc-lib" / "ee" / ee_compiler_dirname / "cpp",
    }

    return commands[command]

def make_compiler_cmd(config_dir: Path, src_path: Path) -> tuple[str, str]:
    rel_root = Path(os.path.relpath(ROOT, config_dir))
    sdk_cc_dir = f"{rel_root}/tools/cc/{SDK_COMPILER}/bin"

    common_includes = (
        f"-I{src_path.parent / 'src'} "
        f"-I{src_path.parent / 'include'} "
        f"-Iinclude "
        f"-Wa,-I{src_path.parent / 'include'} -Wa,-I{src_path.parent}"
    )

    compile_cmd = (
        f"{sdk_cc_dir}/ee-gcc -c {common_includes} {LANG_DEFINE} {COMPILER_FLAGS}"
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
    # Under wine the PE reads the Unix path directly (wine maps / to Z:),
    # and it cannot resolve a UNC path at all: keep the path unchanged.
    if _wine_binary():
        return value
    # A native WSL path (e.g. a BASELINE_ROOT on ext4) is reachable from
    # Windows only over UNC (\\wsl.localhost\<distro>\...).
    distro = os.environ.get("WSL_DISTRO_NAME")
    if distro and value.startswith("/"):
        return f"\\\\wsl.localhost\\{distro}{value}".replace("/", "\\")
    return value.replace("/", "\\")

def _wine_binary() -> str:
    """The wine binary the PE tools must run under, or "" to run them directly."""
    wine = os.environ.get("RNC_WINE")
    if wine is not None:
        return wine
    on_windows_drive = re.match(r"^/mnt/[A-Za-z]/", str(ROOT)) is not None
    return "" if on_windows_drive else (shutil.which("wine") or "")

def _windows_exe(path: str) -> str:
    """Spell a Windows tool (Ps2EeAs, the SN driver) for a ninja command.

    A checkout on a Windows drive under WSL (/mnt/<drive>/, see _win_path)
    runs the PE natively through WSL interop. Anywhere else the PE can only run
    under wine, so it is launched through an explicit `wine` rather than
    whatever binfmt_misc resolves: launched that way, Ps2EeAs exits 253
    printing nothing on some inputs (fun_001ec530, a div.s in a branch delay
    slot) that `wine Ps2EeAs.exe` assembles. RNC_WINE overrides the wine
    binary; an empty RNC_WINE runs the PE directly.
    """
    wine = _wine_binary()
    quoted = shlex.quote(path)
    return f"{shlex.quote(wine)} {quoted}" if wine else quoted

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
# The declaration may carry a parameter list between the name and the
# attribute, so the name is not always immediately followed by __attribute__.
_ALIAS_RE = re.compile(
    r"([A-Za-z_]\w*)\s*(?:\([^;{]*?\))?\s*__attribute__\s*\(\(\s*alias\s*\(\s*\"([^\"]+)\"\s*\)\s*\)\)"
)

def _alias_symbols(source: Path) -> set[tuple[str, str]]:
    """(alias, target) pairs declared in a source file."""
    text = source.read_text(errors="replace")
    return {(match.group(1), match.group(2)) for match in _ALIAS_RE.finditer(text)}

PADLESS_ASM_HELPER = r'''#!/usr/bin/env python3
"""Normalize SN cc1 output for Ps2EeAs and drop section tail padding.

`normalize IN OUT` rewrites GNU `alias = function` assignments into
co-located labels (Ps2EeAs rejects the assignment form).
`finish PADDED OUT [REFERENCE]` removes only the `.text` section tail padding
after the last sized function and adds empty `.data`/`.bss` sections so object
comparison sees the GNU layout.  With a REFERENCE object assembled by GNU as
from the same source, data sections Ps2EeAs rounded up to their alignment are
trimmed back to the reference size: data sizes do not depend on the
assembler, and the removed tail must be zero bytes nothing refers to.  Every
check fails closed; no target bytes, addresses or expected lengths are inputs.
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


def pin_labels_before_align(text):
    """Keep a local label that precedes a loop alignment at its own address.

    cc1 can emit `$La:` `.p2align 3` `$Lb:` when a branch target ends up just
    before an aligned loop head.  GNU as (and retail) leave `$La` before the
    alignment nops; Ps2EeAs moves it past them.  `$La = .` defines the label
    in place without that move.  Only a local label that is followed, after
    the alignment, by another label is rewritten.
    """
    skip = r"(?:[ \t]*(?:\.set[ \t]+\w+|\.loc[^\n]*|#[^\n]*)?\r?\n)*"
    pattern = re.compile(r"^(\$L\w+):[ \t]*(\r?\n" + skip + r"[ \t]*\.p2align[ \t]+\d[^\n]*\r?\n"
                         + skip + r"\$L\w+:)", re.M)
    return pattern.sub(lambda m: m.group(1) + " = ." + m.group(2), text)


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


def _sections(data):
    shoff = struct.unpack_from("<I", data, 32)[0]
    shsize, count, names_index = struct.unpack_from("<HHH", data, 46)
    headers = [struct.unpack_from("<10I", data, shoff + i * shsize) for i in range(count)]
    nh = headers[names_index]
    names = data[nh[4]:nh[4] + nh[5]]
    return shoff, shsize, headers, [names[h[0]:].split(b"\0")[0] for h in headers]


def trim_data_sections(data, reference):
    shoff, shsize, headers, names = _sections(data)
    _, _, ref_headers, ref_names = _sections(reference)
    ref_size = {n: h[5] for n, h in zip(ref_names, ref_headers) if h[1] == 1}
    result = bytearray(data)
    for index, (h, name) in enumerate(zip(headers, names)):
        if h[1] != 1 or h[2] & 4 or name not in ref_size or h[5] <= ref_size[name]:
            continue
        end = ref_size[name]
        if h[5] - end >= max(h[8], 1) or any(data[h[4] + end:h[4] + h[5]]):
            raise ValueError("%s: tail beyond the reference is not alignment padding" % name.decode())
        for s in headers:
            if s[1] == 2:
                for offset in range(s[4], s[4] + s[5], 16):
                    sym = struct.unpack_from("<IIIBBH", data, offset)
                    if sym[5] == index and sym[1] > end:
                        raise ValueError("symbol refers to removed padding")
            if s[1] in (4, 9) and s[7] == index:
                stride = 12 if s[1] == 4 else 8
                if any(struct.unpack_from("<I", data, offset)[0] >= end
                       for offset in range(s[4], s[4] + s[5], stride)):
                    raise ValueError("relocation refers to removed padding")
        struct.pack_into("<I", result, shoff + index * shsize + 20, end)
    return bytes(result)


def fix_gprel_externs(data):
    """Rebase GPREL16 addends against undefined symbols for GNU ld.

    Ps2EeAs relaxes a small-data access to $gp and writes the in-place
    addend as A - gp0 (gp0 = the .reginfo gp_value) for every symbol.  GNU
    ld adds gp0 back only for local symbols, so a relaxed access to an
    undefined symbol that has no `.extern` size is off by gp0 and overflows GPREL16 at link
    time.  GNU as writes such an addend without the -gp0 term, so add gp0
    back for exactly those relocations; the instruction bytes after linking
    are what Ps2EeAs meant.  No-op for objects without that case.
    """
    shoff, shsize, headers, names = _sections(data)
    gp0 = 0
    for h, name in zip(headers, names):
        if h[1] == 0x70000006:  # SHT_MIPS_REGINFO
            gp0 = struct.unpack_from("<i", data, h[4] + 20)[0]
    if not gp0:
        return data
    result = bytearray(data)
    for h in headers:
        if h[1] != 9 or h[9] != 8:
            continue
        symtab = headers[h[6]]
        target = headers[h[7]]
        for offset in range(h[4], h[4] + h[5], 8):
            r_offset, r_info = struct.unpack_from("<II", data, offset)
            if r_info & 0xFF != 7:  # R_MIPS_GPREL16
                continue
            sym = struct.unpack_from("<IIIBBH", data, symtab[4] + (r_info >> 8) * 16)
            # Only undefined symbols without an `.extern NAME, SIZE`: Ps2EeAs
            # writes a correct (gp0-free) addend for the sized ones.
            if sym[3] >> 4 == 0 or sym[5] != 0 or sym[2] != 0:
                continue
            where = target[4] + r_offset
            word = struct.unpack_from("<I", result, where)[0]
            addend = ((word & 0xFFFF) ^ 0x8000) - 0x8000
            addend += gp0
            if not -0x8000 <= addend < 0x8000:
                raise ValueError("GPREL16 addend out of range after rebase")
            struct.pack_into("<I", result, where, (word & 0xFFFF0000) | (addend & 0xFFFF))
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


def hoist_sda_externs(assembly, source):
    """Move `.extern NAME, SIZE` ahead of the code for every small-data symbol.

    SN cc1 writes its `.extern` directives at the end of the file and leaves
    the choice between $gp and lui to the assembler.  Ps2EeAs is single-pass:
    it relaxes a bare symbol access to $gp only when the size is already
    known.  The source says which globals retail reads through $gp: those
    declared `__attribute__((sda))` (the game compiler's spelling, ignored by
    SN cc1); their directives go first, the others stay where cc1 put them.
    """
    sda = set()
    for decl in re.findall(r"^extern[^;]*__attribute__\(\(sda\)\)[^;]*;", source, re.M):
        label = re.search(r'__asm__\s*\(\s*"([\w.$]+)"', decl)
        name = re.search(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*(?:__asm__|__attribute__)", decl)
        if label:
            sda.add(label.group(1))
        elif name:
            sda.add(name.group(1))
    lines = assembly.split("\n")
    first = [l for l in lines if re.match(r"\s*\.extern\s+([\w.$]+)\s*,", l)
             and re.match(r"\s*\.extern\s+([\w.$]+)", l).group(1) in sda]
    rest = [l for l in lines if l not in first]
    return "\n".join(first + rest)


def main(argv):
    if len(argv) == 5 and argv[1] == "externs":
        _, _, source, destination, c_source = argv
        assembly = hoist_sda_externs(open(source).read(), open(c_source, errors="replace").read())
        open(destination, "w").write(assembly)
        return
    if len(argv) not in (4, 5):
        raise SystemExit("usage: padless-asm.py normalize IN OUT [POLICY] | finish IN OUT [REFERENCE] | externs IN OUT SOURCE")
    mode, source, destination = argv[1:4]
    policy = argv[4] if len(argv) == 5 else "none"
    data = open(source, "rb").read()
    if mode == "normalize":
        assembly = pin_labels_before_align(normalize_aliases(data.decode()))
        if policy == "la-gprel":
            assembly = apply_la_gprel_policy(assembly)
        elif policy != "none":
            raise SystemExit("unknown assembler policy: " + policy)
        open(destination, "w").write(assembly)
    elif mode == "finish":
        data = fix_gprel_externs(unpad(data))
        if len(argv) == 5:
            data = trim_data_sections(data, open(argv[4], "rb").read())
        open(destination, "wb").write(add_empty_sections(data))
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
        FAST_LD_PATH,
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
"tools/cc/{SDK_COMPILER}/bin/ee-gcc" = "{SDK_COMPILER_DECOMPME}"
"""
        )

_TEXT_STATEMENT_RE = re.compile(r"^(\s*)(build/\S+?\.o)\(\.text\);$", re.MULTILINE)

def write_fast_linkerscript(config_dir: Path) -> list[tuple[str, str, str]]:
    """Write FAST_LD_PATH from LD_PATH; return (object, link copy, section) rows.

    GNU ld 2.40 resolves a statement that names its input file
    (`build/src/x.c.o(.text);`) by walking the whole input list, and the cost
    of the retail layout (one output section per object, ~2100 of them) grows
    about cubically: 117 s for the boot ELF, while `*(.text.NAME)` statements
    over the same objects link in 2 s (docs/SPEEDUP.md in the tools repo).

    So each object with a `.text` statement gets a link copy under
    build/link/ whose `.text` is renamed to a unique `.text.lnkNNNN`, the
    statement becomes `*(.text.lnkNNNN);`, every other statement naming that
    object names the copy, and the copies are loaded with INPUT(). LD_PATH
    itself is unchanged, so the tools that read the layout keep working; the
    linked bytes are identical (the gate hash proves it).
    """
    text = (config_dir / LD_PATH).read_text()
    rows: list[tuple[str, str, str]] = []

    def rename(match: re.Match) -> str:
        obj = match.group(2)
        section = f".text.lnk{len(rows):04d}"
        rows.append((obj, "build/link/" + obj.removeprefix("build/"), section))
        return f"{match.group(1)}*({section}); /* {obj} */"

    text = _TEXT_STATEMENT_RE.sub(rename, text)
    for obj, link, _ in rows:
        text = text.replace(f"{obj}(", f"{link}(")
    inputs = "".join(f"    {link}\n" for _, link, _ in rows)
    (config_dir / FAST_LD_PATH).write_text(f"INPUT(\n{inputs})\n\n" + text)
    return rows

def build_stuff(
    config_dir: Path,
    config: dict[str, Any],
    linker_entries: list[LinkerEntry],
):
    src_path = Path(config["options"]["src_path"])
    compile_cmd, common_includes = make_compiler_cmd(config_dir, src_path)

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
        "sdk-compiler",
        description="sdk-compiler $in",
        command=f"{compile_cmd} $in $extra -o $out && {CROSS}strip $out -N dummy-symbol-name -R .mdebug",
    )

    if not game_compiler_configured():
        # The reconstructed game compiler is mandatory, not optional.  Without
        # it, every unit in GAME_COMPILER_UNITS silently falls through to
        # another rule (patched/SN/EE-GCC 2.9), which changes codegen for 297
        # units without any error - the repository ignores tools/, so a fresh
        # clone reroutes them unnoticed.  Failing here keeps the build honest
        # and makes the per-unit SN entries for game-compiler owners unreachable, so
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

    if not sn_compiler_configured():
        raise SystemExit(
            "the game code is assembled by Ps2EeAs: set SN_TOOLCHAIN_ROOT to the "
            "SN tree that holds ee/bin/Ps2EeAs.exe (see docs/building.md)"
        )
    # The generated helper rewrites GNU alias assignments to labels for
    # Ps2EeAs and trims only the section tail padding Ps2EeAs adds.
    (config_dir / "padless-asm.py").write_text(PADLESS_ASM_HELPER)
    ee_assembler = _windows_exe(str(Path(SN_TOOLCHAIN_ROOT) / "ee/bin/Ps2EeAs.exe"))

    game_root = _game_compiler_root()
    # Game code: the game compiler's cc1, assembled by Ps2EeAs, as retail was.
    # Ps2EeAs pads short loops itself (its div padding is patched out at
    # install: retail has none, scripts/patch-ps2eeas.py), and the compile
    # carries no -g: the line-number labels it would add are "possible branch
    # destinations" to the assembler.  The game compiler's
    # GNU as assembles the same source only to tell `finish` the true size
    # of the data sections.
    ninja.rule(
        "game-compiler",
        description="game-compiler $in",
        command=(
            f"mkdir -p $gc_work && "
            f"{game_root}/ee-gcc -S -I{game_root}/include {common_includes} "
            f"{LANG_DEFINE} -DMATCHING_DECOMP -O2 $in $extra -o $gc_work/cand.s && "
            f"{sys.executable} padless-asm.py normalize $gc_work/cand.s $gc_work/cand-final.s none && "
            f"{ee_assembler} -o '$gc_work_win/cand-padded.o' '$gc_work_win/cand-final.s' && "
            f"{game_root}/as -mabi=eabi -o $gc_work/cand-ref.o $gc_work/cand-final.s && "
            f"{sys.executable} padless-asm.py finish $gc_work/cand-padded.o $out $gc_work/cand-ref.o && "
            f"{CROSS}strip $out -N dummy-symbol-name -R .mdebug"
        ),
    )
    # Pending units: an INCLUDE_ASM wrapper around splat's GNU-syntax listing,
    # assembled by the game compiler's own GNU as.
    ninja.rule(
        "include-asm",
        description="include-asm $in",
        command=(
            f"{game_root}/ee-gcc -c -I{game_root}/include {common_includes} "
            f"{LANG_DEFINE} {COMPILER_FLAGS} "
            f"$in $extra -o $out && {CROSS}strip $out -N dummy-symbol-name -R .mdebug"
        ),
    )

    if sn_compiler_configured():
        sn_root = Path(SN_TOOLCHAIN_ROOT)
        sn_repo = ROOT
        sn_driver = _windows_exe(str(sn_root / "bin/ee-gcc.exe"))
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
                f"{sn_driver} -c '-B{sn_lib}\\' '-B{sn_eebin}\\' "
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
        ninja.rule(
            "cc_sn_padless",
            description="cc_sn_padless $in",
            command=(
                f"mkdir -p $sn_work && cp $in $sn_work/cand.c && "
                f"{sn_driver} -S '-B{sn_lib}\\' '-B{sn_eebin}\\' "
                f"-I'{sn_inc}' -I'{sn_repo_inc}' "
                f"-DBUILD_US_VERSION -DMATCHING_DECOMP -O2 -g2 $extra "
                f"'$sn_work_win/cand.c' -o '$sn_work_win/cand.s' && "
                f"{sys.executable} padless-asm.py normalize $sn_work/cand.s $sn_work/cand-final.s $policy && "
                f"{ee_assembler} -o '$sn_work_win/cand-padded.o' '$sn_work_win/cand-final.s' && "
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
                    f"-DBUILD_US_VERSION -DMATCHING_DECOMP -O2 -g2 "
                    f"$pat_work/cand.c -o $pat_work/cand.s && "
                    f"{sys.executable} padless-asm.py normalize $pat_work/cand.s $pat_work/cand-final.s $policy && "
                    f"{ee_assembler} -o '$pat_work_win/cand-padded.o' '$pat_work_win/cand-final.s' && "
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
        "link_copy",
        description="link copy $out",
        command=f"{CROSS}objcopy --rename-section .text=$section $in $out",
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
            rule = unit_compiler(unit, int(seg.vram_start))
            if rule == "game-compiler" and _is_include_asm(config_dir, entry.src_paths[0]):
                build(entry.object_path, entry.src_paths, "include-asm")
            elif rule == "game-compiler":
                flags = GAME_COMPILER_FLAG_UNITS.get(unit, "")
                gc_work = str(ROOT / "build/game-work/units" / unit)
                variables = {"gc_work": gc_work, "gc_work_win": _win_path(gc_work)}
                if flags:
                    variables["extra"] = f"{flags} "
                build(entry.object_path, entry.src_paths, rule, variables=variables)
            elif rule == "sdk-compiler":
                flags = SDK_COMPILER_FLAG_UNITS.get(unit, "")
                variables = {"extra": f"{flags} "} if flags else {}
                build(entry.object_path, entry.src_paths, rule, variables=variables)
            elif rule == "cc_ee_gcc_patched" and not patched_route:
                # No patched profile: build from the retail oracle; the C is
                # verified when the profile is available.
                oracle_fallback_units.append(unit)
                build(
                    entry.object_path,
                    [Path("expected/obj") / f"{unit}.c.o"],
                    "oracle_obj",
                )
            elif rule == "cc_ee_gcc_patched":
                pat_work = str(ROOT / "build/patched-work/units" / unit)
                variables = {
                    "pat_work": pat_work,
                    "pat_work_win": _win_path(pat_work),
                    "policy": PADLESS_POLICY_UNITS.get(unit, "none"),
                }
                build(entry.object_path, entry.src_paths, rule, variables=variables)
            elif rule in ("cc_sn", "cc_sn_padless"):
                if not sn_compiler_configured():
                    raise SystemExit(
                        f"{unit} still needs the SN toolchain ({rule}); set "
                        "SN_TOOLCHAIN_ROOT (see ROUTE_EXCEPTIONS)"
                    )
                sn_work = str(ROOT / "build/sn-work/units" / unit)
                flags = SN_FLAG_UNITS.get(unit, "")
                variables = {
                    "sn_work": sn_work,
                    "sn_work_win": _win_path(sn_work),
                }
                if rule == "cc_sn_padless":
                    variables["policy"] = PADLESS_POLICY_UNITS.get(unit, "none")
                if flags:
                    variables["extra"] = f"{flags} "
                build(entry.object_path, entry.src_paths, rule, variables=variables)
            else:
                raise SystemExit(f"{unit}: unknown compiler rule {rule!r}")

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

    link_copies = []
    for obj, link, section in write_fast_linkerscript(config_dir):
        ninja.build(link, "link_copy", obj, variables={"section": section})
        link_copies.append(link)

    ninja.build(
        PRE_ELF_PATH,
        "ld",
        FAST_LD_PATH,
        implicit=[str(obj) for obj in built_objects] + link_copies + [LD_PATH],
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
    """Normalize the gp-relative store and FPU load spelling for the frozen assembler.

    Splat emits ``sw $r, %gp_rel(sym)($28)`` for small-data stores.  The
    pinned EE 2.9 assembler rejects the ``%gp_rel`` operator on stores and
    on ``lwc1``/``ldc1`` ("Bad expression"), while the hand-written oracles used
    ``.extern sym, 4`` plus a bare ``sym`` operand, which expands to the
    same R_MIPS_GPREL16 relocation.  Only generated per-function
    ``expected/asm`` files are touched; expected objects come from the
    whole-unit ``.c.s`` output through binutils and are unaffected.
    """
    pattern = re.compile(
        r"(?P<indent>[ \t]*)(?P<op>sw|swc1|sd|sdc1|sh|sb|lwc1|ldc1)(?P<spacing>\s+)"
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
        # The preserved small-data blobs carry no symbols of their own: the
        # variables in them are addressed through undefined_syms, and a unit
        # listed in SDATA_OVERLAYS defines its variables itself.
        if asm_file.name in ("core_lit.s", "lit.s"):
            data_asm, labels = re.subn(r"^glabel \S+\n", "", data_asm, flags=re.M)
            count += labels
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
    for unit in c_units:
        for suffix, (vram, at) in SDATA_OVERLAYS.items():
            if unit.endswith(suffix):
                rodata_overlay_sections.append(
                    f"    {suffix.replace('/', '.')}.sdata 0x{vram:X} : AT(0x{at:X}) SUBALIGN(4)\n"
                    "    {\n"
                    f"        build/src/{unit}.c.o(.sdata);\n"
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

def overlay_sn_units() -> set[str]:
    """src/overlays files built by SN cc1 2.95.2 (OVERLAY_SN_UNITS)."""
    return set(OVERLAY_SN_UNITS)

def overlay_sn_functions(units: set[str]) -> set[str]:
    """Names of the functions defined or stubbed in the SN-built files."""
    names = set()
    for unit in units:
        path = OVERLAYS_SRC / unit
        if path.is_file():
            names |= set(re.findall(r"\b(FUN_L\d\d_[0-9a-f]{8})\b", path.read_text(errors="replace")))
    return names

def build_overlays() -> Path:
    """Write build/overlays/build.ninja for the level overlays (docs/overlays.md).

    The objects are never linked; the executable's build does not see them.
    Each src/overlays file is built twice:

    - obj/<path>.c.o: as written, stubs included (the game compiler's driver
      and GNU as, the executable's include-asm route);
    - c/<path>.c.o: its C only (INCLUDE_ASM lines dropped), on the game route
      (cc1, Ps2EeAs) as retail was built; scripts/overlay_unit.py proves
      every function in it against the level text.

    stage/<name>.c.o is the same game route for one function staged by
    scripts/check-unit.py.
    """
    sources = sorted(OVERLAYS_SRC.glob("**/*.c"))
    if not sources:
        raise SystemExit(f"no overlay sources under {OVERLAYS_SRC}")
    if not game_compiler_configured():
        raise SystemExit(
            "the reconstructed game compiler is required: "
            f"{_game_compiler_root()} has no ee-gcc/cc1 (see docs/building.md)"
        )
    asm_dir = Path("config/us/overlays/asm")
    if not asm_dir.is_dir():
        raise SystemExit(
            f"{asm_dir} is missing: run `python3 scripts/overlay-extract.py --iso "
            "<your disc image>` (or ./setup.sh) first"
        )
    sn_root = Path(SN_TOOLCHAIN_ROOT or ROOT / "tools/compilers/ee-gcc-2.95.2")
    OVERLAYS_BUILD.mkdir(parents=True, exist_ok=True)
    (OVERLAYS_BUILD / "padless-asm.py").write_text(PADLESS_ASM_HELPER)
    game_root = _game_compiler_root()
    rel_root = os.path.relpath(ROOT, OVERLAYS_BUILD)
    includes = (f"-I{game_root}/include -I{rel_root}/src -I{rel_root}/include "
                f"-Wa,-I{rel_root}/include -Wa,-I{rel_root}")
    ee_assembler = _windows_exe(str(sn_root / "ee/bin/Ps2EeAs.exe"))
    ninja_path = OVERLAYS_BUILD / "build.ninja"
    ninja = ninja_syntax.Writer(open(str(ninja_path), "w"), width=9999)
    ninja.rule(
        "overlay-cc",
        description="overlay-cc $in",
        command=f"{game_root}/ee-gcc -c {includes} {LANG_DEFINE} {COMPILER_FLAGS} $in -o $out",
    )
    ninja.rule(
        "c-only",
        description="c-only $in",
        # a file of stubs only would leave no .text for padless-asm.py
        command="{ grep -v INCLUDE_ASM $in; echo 'void overlay_c_only_anchor(void) {}'; } > $out",
    )
    work = "${out}.work"
    ninja.rule(
        "overlay-game",
        description="overlay-game $in",
        command=(
            f"mkdir -p {work} && "
            f"{game_root}/ee-gcc -S {includes} {LANG_DEFINE} -DMATCHING_DECOMP -O2 $in -o {work}/cand.s && "
            f"{sys.executable} padless-asm.py normalize {work}/cand.s {work}/cand-final.s none && "
            f"{ee_assembler} -o '$work_win/cand-padded.o' '$work_win/cand-final.s' && "
            f"{game_root}/as -mabi=eabi -o {work}/cand-ref.o {work}/cand-final.s && "
            f"{sys.executable} padless-asm.py finish {work}/cand-padded.o $out {work}/cand-ref.o"
        ),
    )

    # Level code the game compiler does not reproduce but SN cc1 2.95.2 does
    # (OVERLAY_SN_UNITS): SN cc1, its externs ordered for Ps2EeAs, then the
    # same Ps2EeAs + padless finish as overlay-game.
    sn_driver = _windows_exe(str(sn_root / "bin/ee-gcc295.exe"))
    ninja.rule(
        "overlay-sn",
        description="overlay-sn $in",
        command=(
            f"mkdir -p {work} && cp $in {work}/cand.c && "
            f"cd {work} && {sn_driver} -S -I{ROOT}/src -I{ROOT}/include "
            f"{LANG_DEFINE} -DMATCHING_DECOMP -O2 cand.c -o cand.s && cd - >/dev/null && "
            f"{sys.executable} padless-asm.py normalize {work}/cand.s {work}/cand-norm.s none && "
            f"{sys.executable} padless-asm.py externs {work}/cand-norm.s {work}/cand-final.s $in && "
            f"{ee_assembler} -o '$work_win/cand-padded.o' '$work_win/cand-final.s' && "
            f"{game_root}/as -mabi=eabi -o {work}/cand-ref.o {work}/cand-final.s && "
            f"{sys.executable} padless-asm.py finish {work}/cand-padded.o $out {work}/cand-ref.o"
        ),
    )
    sn_units = overlay_sn_units()

    def game_edge(out: str, src: str, rule: str = "overlay-game") -> None:
        path = str((OVERLAYS_BUILD / out).resolve()) + ".work"
        ninja.build(outputs=[out], rule=rule, inputs=[src],
                    implicit=["padless-asm.py"], variables={"work_win": _win_path(path)})

    objects = []
    for src in sources:
        rel = src.relative_to(OVERLAYS_SRC)
        obj, c_only = f"obj/{rel}.o", f"c/{rel}"
        ninja.build(outputs=[obj], rule="overlay-cc", inputs=[f"{rel_root}/{src}"])
        ninja.build(outputs=[c_only], rule="c-only", inputs=[f"{rel_root}/{src}"])
        game_edge(f"{c_only}.o", c_only, "overlay-sn" if str(rel) in sn_units else "overlay-game")
        objects += [obj, f"{c_only}.o"]
    catalogue = Path("config/overlays/us/functions.tsv")
    sn_functions = overlay_sn_functions(sn_units)
    for line in catalogue.read_text().splitlines():
        parts = line.split("\t")
        if len(parts) > 1 and parts[1] in ("shared", "level"):
            game_edge(f"stage/{parts[0]}.c.o", f"stage/{parts[0]}.c",
                      "overlay-sn" if parts[0] in sn_functions else "overlay-game")
    ninja.build(outputs=["overlays"], rule="phony", inputs=objects)
    ninja.default(["overlays"])
    ninja.close()
    print(f"{len(sources)} overlay source files -> {ninja_path} (ninja -C {OVERLAYS_BUILD})")
    return ninja_path

def main():
    class ArgsProtocol:
        YAML_FILE: Path
        clean: bool
        make_asm: bool
        overlays: bool

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
    parser.add_argument(
        "--overlays",
        help="Only write build/overlays/build.ninja for the level overlay sources (docs/overlays.md)",
        action="store_true",
    )
    args = cast(ArgsProtocol, parser.parse_args())

    if args.overlays:
        build_overlays()
        return

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
