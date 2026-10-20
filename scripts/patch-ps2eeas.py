#!/usr/bin/env python3
"""Prepare SN's EE assembler ps2eeas 1.9.25.758 for matching builds.

ps2eeas 1.9.25.758 (ProDG 3.01) puts up to two NOPs in front of a `div`,
`divu` or `div.s` that follows a branch or a label too closely, and refuses
such a `div.s` in a branch delay slot under `.set nomacro` ("Macro expansion
is disabled"). The retail code never had that padding: the executable has 346
div words and the 19 level overlays 26,539, and not one is preceded by the
`nop; nop` the workaround would leave, although many sit right after a label
or a branch. The assembler retail used did not apply it, so this one must not
either.

The patch is 6 bytes in the check that runs before every div opcode
(VA 0x40BD06): the branch and label checks are skipped, the SYNC.P check
after them is kept. Everything else the assembler does (short loop padding,
FPU hazard NOPs, the gp relaxation) is untouched.

The PE stack reservation is also raised from 1 MiB to 64 MiB. Large overlay
inputs overflow the original Windows guest stack under both Wibo and Wine;
the Linux process stack limit does not change that reservation. This changes
only loader metadata, not assembler instructions or section contents.

Usage:
    patch-ps2eeas.py IN.exe OUT.exe

IN must be the pinned ps2eeas 1.9.25.758 (SHA-256 below); OUT is checked
against the expected patched SHA-256.
"""
from __future__ import annotations

import hashlib
import struct
import sys
from pathlib import Path

ORIGINAL_SHA = "cb5adda955e64626564212ef7e0c1434708c4e1ef423344a92ec8033306ed3aa"
PATCHED_SHA = "be3032d8cdd0ceecffec4ec2e043db3ca4b5b2fc1c781d17e5b3838684b8952d"
IMAGE_BASE = 0x400000          # .text: file offset == RVA
SITE = 0x40BD06                # mov ecx, [0x430258] (last branch position)
TARGET = 0x40BDF5              # mov eax, [0x430260] (the SYNC.P check)
ORIGINAL = bytes.fromhex("8b0d58024300")


def patch(data: bytes) -> bytes:
    out = bytearray(data)
    offset = SITE - IMAGE_BASE
    if out[offset:offset + 6] != ORIGINAL:
        raise SystemExit("patch-ps2eeas: unexpected bytes at the divbug check")
    # push edi (the skipped block pushes it, the SYNC.P block pops it); jmp TARGET
    out[offset:offset + 6] = b"\x57\xe9" + struct.pack("<i", TARGET - (SITE + 6))
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe:pe + 4] != b"PE\0\0" or struct.unpack_from("<H", data, pe + 4)[0] != 0x14C:
        raise SystemExit("patch-ps2eeas: expected an i386 PE image")
    optional = pe + 24
    if struct.unpack_from("<H", data, optional)[0] != 0x10B:
        raise SystemExit("patch-ps2eeas: expected a PE32 optional header")
    stack_reserve = optional + 72
    if struct.unpack_from("<II", data, stack_reserve) != (0x100000, 0x1000):
        raise SystemExit("patch-ps2eeas: unexpected stack reservation or commit size")
    struct.pack_into("<I", out, stack_reserve, 64 * 1024 * 1024)
    return bytes(out)


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit(__doc__)
    source, target = Path(sys.argv[1]), Path(sys.argv[2])
    data = source.read_bytes()
    if hashlib.sha256(data).hexdigest() != ORIGINAL_SHA:
        raise SystemExit(f"patch-ps2eeas: {source} is not ps2eeas 1.9.25.758")
    patched = patch(data)
    if hashlib.sha256(patched).hexdigest() != PATCHED_SHA:
        raise SystemExit("patch-ps2eeas: patched assembler has an unexpected SHA-256")
    target.write_bytes(patched)
    target.chmod(0o755)


if __name__ == "__main__":
    main()
