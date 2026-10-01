#!/usr/bin/env python3
"""Rebuild the boot ELF into the Ratchet & Clank disc image.

The PS2 disc is a single-track ISO9660 with exactly three files on the
filesystem (SYSTEM.CNF, SCUS_971.99, IOPRP243.IMG); the remaining ~4.2 GB is
raw, LBA-addressed game data outside the filesystem. That raw data must be
preserved byte-for-byte — it cannot be extracted and repacked. The only
recompiled piece is the boot ELF, so rebuilding the ISO means patching the
SCUS_971.99 extent in place.

Because the matching decompilation reproduces the boot ELF byte-for-byte
(full-ELF SHA gate), the patched ISO is byte-identical to the original.

Usage:
  python3 rebuild-iso.py [--iso dumps/....iso] [--elf PATH] [--out build/....iso]

If --elf is omitted, the script looks for the reconstructed boot ELF in the
baseline workspace (build/baseline/config/us/build/SCUS_971.99 in the checkout,
or under BASELINE_ROOT when set); run `make elf` first to produce it. For a dry
check of the patching machinery you may pass config/us/SCUS_971.99 (the retail
executable, byte-identical to the built ELF).
"""

from __future__ import annotations

import argparse
import hashlib
import os
import pathlib
import struct
import sys

ROOT = pathlib.Path(__file__).resolve().parent
RETAIL_SHA = "e050581032e4bb3f20341307da5b69b76f1574910519155380ea771e55c3c0c9"


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def find_boot_extent(iso: bytes, want_name: str = "SCUS_971.99;1") -> tuple[int, int]:
    """Return (LBA, size) of the boot ELF file record from the ISO9660 PVD."""
    if iso[0x8000 : 0x8000 + 6] != b"\x01CD001":
        raise SystemExit(
            "input is not an ISO9660 image (missing CD001 PVD at sector 16)"
        )
    root_record = iso[0x8000 + 156 : 0x8000 + 190]
    root_lba = struct.unpack_from("<I", root_record, 2)[0]
    root_size = struct.unpack_from("<I", root_record, 10)[0]
    off = root_lba * 2048
    o = 0
    end = off + root_size
    while off + o < end:
        rec_len = iso[off + o]
        if rec_len == 0:
            o += 1
            continue
        name_len = iso[off + o + 32]
        name = iso[off + o + 33 : off + o + 33 + name_len].decode("latin1")
        if name == want_name:
            lba = struct.unpack_from("<I", iso, off + o + 2)[0]
            size = struct.unpack_from("<I", iso, off + o + 10)[0]
            return lba, size
        o += rec_len
    raise SystemExit(
        f"boot ELF record {want_name!r} not found in ISO9660 root directory"
    )


def built_elf_candidates() -> list[pathlib.Path]:
    override = os.environ.get("BASELINE_ROOT", "").strip()
    candidates = [ROOT / "build/SCUS_971.99"]
    if override:
        baseline = pathlib.Path(override).expanduser()
        candidates.append(baseline / "config/us/build/SCUS_971.99")
        candidates.append(baseline / "build/SCUS_971.99")
    candidates.append(ROOT / "build/baseline/config/us/build/SCUS_971.99")
    candidates.append(ROOT / "build/baseline/build/SCUS_971.99")
    return candidates


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "--iso",
        type=pathlib.Path,
        default=None,
        help="original disc image (default: first *.iso in dumps/)",
    )
    ap.add_argument(
        "--elf",
        type=pathlib.Path,
        default=None,
        help="built boot ELF (default: search build/ and baseline workspace)",
    )
    ap.add_argument(
        "--out",
        type=pathlib.Path,
        default=None,
        help="output ISO path (default: build/Ratchet & Clank (USA) - rebuilt.iso)",
    )
    ap.add_argument(
        "--force",
        action="store_true",
        help="patch even when the built ELF is not byte-identical to retail (experimental; may not boot)",
    )
    args = ap.parse_args()

    iso_path = args.iso
    if iso_path is None:
        dumps = ROOT / "dumps"
        candidates = sorted(dumps.glob("*.iso"))
        if not candidates:
            raise SystemExit("no *.iso found under dumps/; pass --iso")
        iso_path = candidates[0]

    elf_path = args.elf
    if elf_path is None:
        elf_path = next((p for p in built_elf_candidates() if p.is_file()), None)
        if elf_path is None:
            raise SystemExit(
                "no built boot ELF found. Run `make elf` first (it leaves the ELF "
                "in the baseline workspace), or pass --elf."
            )
    if not elf_path.is_file():
        raise SystemExit(f"built ELF not found: {elf_path}")

    out_path = args.out or (ROOT / "build" / "Ratchet & Clank (USA) - rebuilt.iso")

    iso = iso_path.read_bytes()
    built = elf_path.read_bytes()
    built_sha = sha256_bytes(built)
    print(f"original ISO : {iso_path} ({len(iso)} bytes)")
    print(f"built ELF    : {elf_path} ({len(built)} bytes) sha256 {built_sha}")

    lba, size = find_boot_extent(iso)
    extent_off = lba * 2048
    if extent_off + size > len(iso):
        raise SystemExit(
            f"REFUSING: boot extent LBA {lba} ({size} bytes at offset 0x{extent_off:X}) "
            f"lies outside the ISO image ({len(iso)} bytes)"
        )
    if size != len(built):
        print(
            f"WARNING: built ELF size {len(built)} != ISO record size {size}; "
            "the ISO record must be resized in the directory (not supported yet)"
        )
    print(f"boot extent  : LBA {lba} (offset 0x{extent_off:X}), record size {size}")

    original_extent = iso[extent_off : extent_off + size]
    original_sha = sha256_bytes(original_extent)
    print(f"original ELF : sha256 {original_sha}")
    if original_sha != RETAIL_SHA:
        print("WARNING: original ISO boot ELF does not match known retail sha256")

    if original_sha == built_sha:
        print(
            "built ELF is byte-identical to the disc's boot ELF -> rebuilt ISO == original ISO"
        )
    else:
        print(
            "built ELF differs from the disc ELF; the extent will be overwritten "
            f"({size} bytes at offset 0x{extent_off:X})"
        )

    if original_sha != built_sha and not args.force:
        raise SystemExit(
            "REFUSING: the built boot ELF is not byte-identical to the retail disc ELF "
            "(full-ELF gate FAIL). Per-function objdiff can be 100% while the linked "
            "layout differs (function addresses drift), so patching a non-identical ELF "
            "would produce a non-bootable ISO. Fix the link-layout reproduction first; "
            "use --force only to create a deliberately experimental image."
        )

    out_path.parent.mkdir(parents=True, exist_ok=True)
    patched = bytearray(iso)
    if len(built) <= size:
        patched[extent_off : extent_off + len(built)] = built
    else:
        raise SystemExit("built ELF larger than the ISO record; resizing unsupported")
    out_path.write_bytes(bytes(patched))
    print(f"wrote {out_path}")

    # round-trip verification
    back = out_path.read_bytes()
    verify_lba, verify_size = find_boot_extent(back)
    back_elf = back[verify_lba * 2048 : verify_lba * 2048 + verify_size]
    back_sha = sha256_bytes(back_elf)
    ok = back_sha == built_sha
    print(
        f"verify      : rebuilt ISO boot ELF sha256 {back_sha} matches built ELF: {ok}"
    )
    if original_sha == built_sha:
        print("result      : byte-identical to the original disc image")
    print("ISO rebuild complete (boot path).")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
