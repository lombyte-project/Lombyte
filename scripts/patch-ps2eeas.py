#!/usr/bin/env python3
"""Turn off the divbug padding of SN's EE assembler ps2eeas 1.9.25.758.

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
PATCHED_SHA = "457d75293b5aeb64b95ffb37ba21dbd1507e176b428ca6892b3069d1b0d1a27e"
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
