#!/usr/bin/env python3
"""Extract the level overlays from your own disc image, then their assembly.

    python3 scripts/overlay-extract.py --iso game.iso   # records + asm
    python3 scripts/overlay-extract.py                  # asm only, from the records

Writes only gitignored files under config/us/overlays/ (docs/overlays.md):

    level_NN/{lit,bss,data,vtbl,camvtbl,sndvtbl,text}.bin, manifest.json
    asm/FUN_LNN_xxxxxxxx.s   (one per shared/level function, INCLUDE_ASM target)

Every level is checked against config/overlays/us/level-NN.json and a pinned
SHA-256 before anything is written. Nothing from the disc goes into Git.
Methods from rac1-decomp tools/overlay_dump.py and overlay_asm.py (MIT).
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import shutil
import struct
import sys
from pathlib import Path
from typing import Optional

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rnc_overlays as ov  # noqa: E402

WORK = ov.ROOT / "build/overlay-asm"
OUT = ov.ASM_DIR
ASSIGN_RE = re.compile(r"^\s*([A-Za-z_.$][\w.$]*)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;")

# ---------------------------------------------------------------- records from the ISO

ISO_SIZE = 4214095872
LEVELS = json.loads(r"""[{"index": 0, "name": "Veldin Tutorial", "offset": 3862337536, "size": 14315520, "entry": 2382888, "elf_sha256": "8f1e00c8be75635db809679890fb5c128ba208e780461699f73e3a5344356ec9", "sections": [[".lit", 1, 1437440, 12016], [".bss", 8, 1449472, 16816], [".data", 1, 1466368, 541360], ["lvl.vtbl", 1, 2007808, 1284], ["lvl.camvtbl", 1, 2009216, 160], ["lvl.sndvtbl", 1, 2009472, 24], [".text", 1, 2009600, 1073872]]}, {"index": 1, "name": "Novalis", "offset": 3876661248, "size": 19714048, "entry": 2464832, "elf_sha256": "567937f89174f055f8e895839049ea6304f18117b2344222833cd32ba770bc02", "sections": [[".lit", 1, 1437440, 13160], [".bss", 8, 1450624, 16816], [".data", 1, 1467520, 677456], ["lvl.vtbl", 1, 2145024, 2340], ["lvl.camvtbl", 1, 2147456, 160], ["lvl.sndvtbl", 1, 2147712, 56], [".text", 1, 2147840, 1117192]]}, {"index": 2, "name": "Aridia", "offset": 3896383488, "size": 17532928, "entry": 2389880, "elf_sha256": "671ff28b640800f45711de680b3284e781d36f503bfc3f5025d92a895ddbc112", "sections": [[".lit", 1, 1437440, 13472], [".bss", 8, 1451008, 16816], [".data", 1, 1467904, 608608], ["lvl.vtbl", 1, 2076544, 1908], ["lvl.camvtbl", 1, 2078464, 160], ["lvl.sndvtbl", 1, 2078720, 40], [".text", 1, 2078848, 1065136]]}, {"index": 3, "name": "Kerwan", "offset": 3913924608, "size": 18432000, "entry": 2309888, "elf_sha256": "76017c751b5b6eae44c915e85a08f03c4d3f099abf37a215e6ac598e7706dce5", "sections": [[".lit", 1, 1437440, 12168], [".bss", 8, 1449728, 16816], [".data", 1, 1466624, 513824], ["lvl.vtbl", 1, 1980544, 2220], ["lvl.camvtbl", 1, 1982848, 200], ["lvl.sndvtbl", 1, 1983104, 32], [".text", 1, 1983232, 1099584]]}, {"index": 4, "name": "Eudora", "offset": 3932364800, "size": 17016832, "entry": 2328088, "elf_sha256": "451acb09d1bac17d2e5a2bd94140e0440143ae2dacac8e7b6b65f077e1e70584", "sections": [[".lit", 1, 1437440, 12384], [".bss", 8, 1449856, 16848], [".data", 1, 1466752, 485232], ["lvl.vtbl", 1, 1952000, 2100], ["lvl.camvtbl", 1, 1954176, 180], ["lvl.sndvtbl", 1, 1954432, 16], [".text", 1, 1954560, 1144680]]}, {"index": 5, "name": "Rilgar", "offset": 3949389824, "size": 19685376, "entry": 2550768, "elf_sha256": "2f639ecd58b4c9a0f94456f67a472c8a7772f91a7fbcea3dcd6b3ef6fc45246e", "sections": [[".lit", 1, 1437440, 12996], [".bss", 8, 1450496, 16816], [".data", 1, 1467392, 731232], ["lvl.vtbl", 1, 2198656, 1944], ["lvl.camvtbl", 1, 2200704, 180], ["lvl.sndvtbl", 1, 2200960, 56], [".text", 1, 2201088, 1136472]]}, {"index": 6, "name": "Nebula G34", "offset": 3969083392, "size": 20260864, "entry": 2443496, "elf_sha256": "59521b07ae0b02afd6de454fd52f0e1e89e93ca0ee8f6e0e0766a14a254cc3ed", "sections": [[".lit", 1, 1437440, 13952], [".bss", 8, 1451392, 16816], [".data", 1, 1468288, 635824], ["lvl.vtbl", 1, 2104192, 2124], ["lvl.camvtbl", 1, 2106368, 180], ["lvl.sndvtbl", 1, 2106624, 8], [".text", 1, 2106752, 1143832]]}, {"index": 7, "name": "Umbris", "offset": 3989352448, "size": 16994304, "entry": 2544256, "elf_sha256": "157a4bee4669fb14be0af05407d7ffc40e59eef117f3c53d87bf3aa09392cc42", "sections": [[".lit", 1, 1437440, 12072], [".bss", 8, 1449600, 16816], [".data", 1, 1466496, 703552], ["lvl.vtbl", 1, 2170112, 2292], ["lvl.camvtbl", 1, 2172416, 160], ["lvl.sndvtbl", 1, 2172672, 24], [".text", 1, 2172800, 1155584]]}, {"index": 8, "name": "Batalia", "offset": 4006354944, "size": 17315840, "entry": 2422808, "elf_sha256": "895e591c79ac25068465a05f1ea7b3763a78848802c994a97c26c2c56f8c7bf9", "sections": [[".lit", 1, 1437440, 14060], [".bss", 8, 1451520, 16816], [".data", 1, 1468416, 614560], ["lvl.vtbl", 1, 2083072, 2352], ["lvl.camvtbl", 1, 2085504, 180], ["lvl.sndvtbl", 1, 2085760, 48], [".text", 1, 2085888, 1167120]]}, {"index": 9, "name": "Gaspar", "offset": 4023678976, "size": 18102272, "entry": 2510104, "elf_sha256": "ba407330fec6983f46c0dbd3f7a0c088f96936e86e6244927b0e9e32d1b8591a", "sections": [[".lit", 1, 1437440, 12368], [".bss", 8, 1449856, 16816], [".data", 1, 1466752, 668048], ["lvl.vtbl", 1, 2134912, 1968], ["lvl.camvtbl", 1, 2136960, 160], ["lvl.sndvtbl", 1, 2137216, 32], [".text", 1, 2137344, 1110160]]}, {"index": 10, "name": "Orxon", "offset": 4041789440, "size": 17788928, "entry": 2328224, "elf_sha256": "781ff91426d7f3c3daea35d51be522dc83a0f974d62a33ebe3ac2d826bd8ea8d", "sections": [[".lit", 1, 1437440, 13088], [".bss", 8, 1450624, 16816], [".data", 1, 1467520, 487696], ["lvl.vtbl", 1, 1955328, 2160], ["lvl.camvtbl", 1, 1957504, 180], ["lvl.sndvtbl", 1, 1957760, 24], [".text", 1, 1957888, 1163104]]}, {"index": 11, "name": "Pokitaru", "offset": 4059586560, "size": 19558400, "entry": 2535384, "elf_sha256": "0fd802f1618749b340ce0e16a144c1d7ca537a04adc470801da7afcc125e1135", "sections": [[".lit", 1, 1437440, 14472], [".bss", 8, 1452032, 16880], [".data", 1, 1468928, 741088], ["lvl.vtbl", 1, 2210048, 1872], ["lvl.camvtbl", 1, 2211968, 140], ["lvl.sndvtbl", 1, 2212224, 32], [".text", 1, 2212352, 1113416]]}, {"index": 12, "name": "Hoven", "offset": 4079153152, "size": 19509248, "entry": 2481688, "elf_sha256": "9be7f791c88be7fb241ae0437795eba1148d4847e40e8e758dc1d3e7832f0446", "sections": [[".lit", 1, 1437440, 13152], [".bss", 8, 1450624, 16816], [".data", 1, 1467520, 681728], ["lvl.vtbl", 1, 2149248, 1752], ["lvl.camvtbl", 1, 2151040, 160], ["lvl.sndvtbl", 1, 2151296, 48], [".text", 1, 2151424, 1100048]]}, {"index": 13, "name": "Oltanis Orbit", "offset": 4098670592, "size": 19509248, "entry": 2428584, "elf_sha256": "95425c15c284cd4a76a12aee2a8fd8764cb61273b9f98e52041e1fc3f0aec4c6", "sections": [[".lit", 1, 1437440, 12744], [".bss", 8, 1450240, 16816], [".data", 1, 1467136, 587040], ["lvl.vtbl", 1, 2054272, 2340], ["lvl.camvtbl", 1, 2056704, 160], ["lvl.sndvtbl", 1, 2056960, 24], [".text", 1, 2057088, 1200936]]}, {"index": 14, "name": "Oltanis", "offset": 4118188032, "size": 19249152, "entry": 2414520, "elf_sha256": "41f1b1fb2c612238c88829def40f7ddf4b63ed33f24737717140c04939b47df5", "sections": [[".lit", 1, 1437440, 13780], [".bss", 8, 1451264, 16816], [".data", 1, 1468160, 617312], ["lvl.vtbl", 1, 2085504, 1920], ["lvl.camvtbl", 1, 2087424, 200], ["lvl.sndvtbl", 1, 2087680, 48], [".text", 1, 2087808, 1164920]]}, {"index": 15, "name": "Quartu", "offset": 4137445376, "size": 19716096, "entry": 2314792, "elf_sha256": "129bb08dff642d05cb47fa60d8349b94de782a6db837fef10f40ebdc287f05ed", "sections": [[".lit", 1, 1437440, 13536], [".bss", 8, 1451008, 16816], [".data", 1, 1467904, 512064], ["lvl.vtbl", 1, 1980032, 2124], ["lvl.camvtbl", 1, 1982208, 200], ["lvl.sndvtbl", 1, 1982464, 24], [".text", 1, 1982592, 1147752]]}, {"index": 16, "name": "Kalebo III", "offset": 4157169664, "size": 18726912, "entry": 2364160, "elf_sha256": "4006c02ad3f26a032112c3ab81ea34ac05f288cb0e4fef1e91662a30dcec0473", "sections": [[".lit", 1, 1437440, 12964], [".bss", 8, 1450496, 16880], [".data", 1, 1467392, 533728], ["lvl.vtbl", 1, 2001152, 1896], ["lvl.camvtbl", 1, 2003072, 180], ["lvl.sndvtbl", 1, 2003328, 32], [".text", 1, 2003456, 1113216]]}, {"index": 17, "name": "Veldin Orbit", "offset": 4175904768, "size": 17387520, "entry": 2360096, "elf_sha256": "06e9ab7ae7786b899a4264854adf742f0b7ffaa77789ea25dfaf1eabec812b42", "sections": [[".lit", 1, 1437440, 14224], [".bss", 8, 1451776, 16816], [".data", 1, 1468672, 526128], ["lvl.vtbl", 1, 1994880, 2028], ["lvl.camvtbl", 1, 1996928, 160], ["lvl.sndvtbl", 1, 1997184, 16], [".text", 1, 1997312, 1164488]]}, {"index": 18, "name": "Veldin Finale", "offset": 4193300480, "size": 19351552, "entry": 2391824, "elf_sha256": "2f91a356e208965be55aed0637a86abf0164875e716c58e8d30cd3e5ad1b4ef6", "sections": [[".lit", 1, 1437440, 14488], [".bss", 8, 1452032, 16816], [".data", 1, 1468928, 574128], ["lvl.vtbl", 1, 2043136, 2016], ["lvl.camvtbl", 1, 2045184, 160], ["lvl.sndvtbl", 1, 2045440, 48], [".text", 1, 2045568, 1141312]]}]""")

# Section types accepted in the chain: 1 = PROGBITS payload, 8 = NOBITS (bss).
ACCEPT_TYPES = (1, 8)

# Donor names by position, matching the preview rebuild and wadx.py.
SECTION_NAMES = [".lit", ".bss", ".data", "lvl.vtbl", "lvl.camvtbl", "lvl.sndvtbl", ".text"]

_HEADER = struct.Struct("<iiII")  # dest_address, copy_size, section_type, entry_point


def find_section_chain(data: bytes, entry_hint: Optional[int] = None):
    """Find the best chain of section images in ``data``.

    Returns a list of ``(file_offset, dest_address, copy_size, section_type)``
    sorted by file offset.  A chain is a maximal run of headers where each
    header starts exactly where the previous payload ends and all headers share
    the same entry point.
    """
    cands = []
    for off in range(0, len(data) - _HEADER.size, 4):
        dest, size, typ, entry = _HEADER.unpack_from(data, off)
        if typ not in ACCEPT_TYPES:
            continue
        if not (0x100000 <= dest <= 0x8000000):
            continue
        if not (0 <= size <= 0x4000000):
            continue
        if off + _HEADER.size + (size if typ == 1 else 0) > len(data):
            continue
        cands.append((off, dest, size, typ, entry))

    by_start = {c[0]: c for c in cands}
    best_chain, best_score = [], -1
    for c in cands:
        chain = [c]
        cur = c
        while True:
            nxt = by_start.get(cur[0] + _HEADER.size + cur[2])
            if not nxt or nxt[4] != c[4]:
                break
            chain.append(nxt)
            cur = nxt
        score = len(chain) * 4 + (100 if any(x[2] > 0x80000 for x in chain) else 0)
        if score > best_score or (score == best_score and len(chain) > len(best_chain)):
            best_chain, best_score = chain, score

    if entry_hint is not None:
        best_chain = [c for c in cands if c[4] == entry_hint]
    return [(off, dest, size, typ) for off, dest, size, typ, _ in sorted(best_chain)]


def read_sections(data: bytes, chain, entry_hint: Optional[int] = None):
    """Return ``(sections, entry)`` from a chain.

    ``sections`` is a list of ``(name, sh_type, flags, addr, size, payload)``
    tuples: payload is ``b""`` for NOBITS sections.
    """
    sections = []
    for i, (off, dest, size, typ) in enumerate(chain):
        name = SECTION_NAMES[i] if i < len(SECTION_NAMES) else f".unknown_{i}"
        if typ == 8:
            sections.append((name, 8, 0x3, dest, size, b""))
            continue
        payload = data[off + _HEADER.size:off + _HEADER.size + size]
        flags = 0x6 if name == ".text" else 0x3
        sections.append((name, 1, flags, dest, size, payload))

    if entry_hint is not None:
        entry = entry_hint
    else:
        header_off = chain[-1][0]
        entry = struct.unpack_from("<I", data, header_off + 12)[0]
    return sections, entry


def build_overlay_elf(sections, entry: int) -> bytes:
    """Rebuild a 32-bit little-endian MIPS ELF from section tuples.

    One PT_LOAD per PROGBITS section; section addresses are the destination
    addresses from the chain headers.  Returns the ELF bytes.
    """
    n = len(sections)
    phnum = sum(1 for s in sections if s[1] == 1 and s[4] > 0)

    ehsize, phentsize, shentsize = 52, 32, 40
    phoff = ehsize
    data_start = phoff + phentsize * phnum

    offsets = []
    cur = data_start
    for name, stype, flags, addr, size, payload in sections:
        if stype == 8 or size == 0:
            offsets.append((0, 0))
            continue
        cur = (cur + 63) & ~63
        offsets.append((cur, size))
        cur += size
    file_end = cur
    shoff = (file_end + 63) & ~63

    out = bytearray(shoff + shentsize * (n + 2))
    e_ident = b"\x7fELF" + bytes([1, 1, 1, 0]) + b"\x00" * 8
    struct.pack_into("<16sHHIIIIIHHHHHH", out, 0,
                     e_ident, 2, 8, 1, entry, phoff, shoff, 0,
                     ehsize, phentsize, phnum, shentsize, n + 2, n + 1)

    ph = phoff
    for i, (name, stype, flags, addr, size, payload) in enumerate(sections):
        if stype != 1 or size == 0:
            continue
        body, _ = offsets[i]
        struct.pack_into("<IIIIIIII", out, ph,
                         1, body, addr, addr, size, size, 7 if name == ".text" else 6, 0x1000)
        ph += phentsize

    for i, (name, stype, flags, addr, size, payload) in enumerate(sections):
        if stype == 8 or size == 0:
            continue
        body, _ = offsets[i]
        out[body:body + size] = payload[:size]

    shstr = b"\x00" + b"\x00".join(s[0].encode() for s in sections) + b"\x00"
    shstr_off = shoff + shentsize * (n + 2)
    shstr_name = len(shstr)
    shstr += b".shstrtab\x00"

    for i, (name, stype, flags, addr, size, payload) in enumerate(sections):
        body, _ = offsets[i]
        name_off = shstr.find(name.encode() + b"\x00")
        struct.pack_into("<IIIIIIIIII", out, shoff + (i + 1) * shentsize,
                         name_off, stype, flags, addr, body, size, 0, 0,
                         64 if stype != 8 else 4, 0)
    struct.pack_into("<IIIIIIIIII", out, shoff + (n + 1) * shentsize,
                     shstr_name, 3, 0, 0, shstr_off, len(shstr), 0, 0, 1, 0)
    out += shstr
    return bytes(out)


def extract_records(iso: Path) -> int:
    if iso.stat().st_size != ISO_SIZE:
        sys.exit(f"{iso}: not the supported USA disc image (size {iso.stat().st_size}, expected {ISO_SIZE})")
    root = ov.RECORDS_DIR
    errors = []
    with open(iso, "rb") as f:
        for lvl in LEVELS:
            f.seek(lvl["offset"])
            data = f.read(lvl["size"])
            chain = find_section_chain(data)
            sections, entry = read_sections(data, chain)
            got = [[s[0], s[1], s[3], s[4]] for s in sections]
            elf_sha = hashlib.sha256(build_overlay_elf(sections, entry)).hexdigest()
            if got != lvl["sections"] or entry != lvl["entry"] or elf_sha != lvl["elf_sha256"]:
                errors.append(lvl["index"])
                print(f"level {lvl['index']:02d} {lvl['name']:<15} MISMATCH", flush=True)
                continue
            d = root / f"level_{lvl['index']:02d}"
            d.mkdir(parents=True, exist_ok=True)
            records = []
            for rec_name, (off, dest, size, typ) in zip(ov.RECORD_NAMES, chain):
                payload = data[off + 16: off + 16 + size]
                (d / f"{rec_name}.bin").write_bytes(payload)
                records.append({"name": rec_name, "address": dest, "bytes": size, "type": typ,
                                "wad_offset": off, "sha256": hashlib.sha256(payload).hexdigest()})
            (d / "manifest.json").write_text(json.dumps(
                {"level": lvl["index"], "name": lvl["name"], "entry_point": entry,
                 "iso_offset": lvl["offset"], "records": records}, indent=1) + "\n")
            print(f"level {lvl['index']:02d} {lvl['name']:<15} OK", flush=True)
    if errors:
        print(f"levels {errors} do not match the supported release", file=sys.stderr)
        return 1
    return 0


# ---------------------------------------------------------------- assembly

SUFFIX = "_OVLAUTOGEN"

SYM_RE = re.compile(rf"\b(D_|FUN_|func_|jtbl_|jlabel_)([0-9A-Fa-f]{{8}}){re.escape(SUFFIX)}\b")
LOCAL_RE = re.compile(rf"(\.L[0-9A-Fa-f]+){re.escape(SUFFIX)}")
SIZE_RE = re.compile(r"^\.size\s+(\S+),\s*\.\s*-\s*\S+", re.M)


# ---------------------------------------------------------------- inputs

def build_level_data(rows: dict[str, ov.Row]):
    """level -> {address: (name, size)} for every place, and
    name -> (level, address, size, kind) for the canonical place of every
    shared/level row."""
    level_entries: dict[int, dict[int, tuple[str, int]]] = {}
    targets = {}
    for row in rows.values():
        lv0, a0 = row.canonical
        if row.kind in ("shared", "level"):
            targets[row.name] = (lv0, a0, row.size, row.kind)
        for lv, addr in row.places:
            entries = level_entries.setdefault(lv, {})
            if addr in entries:
                raise SystemExit(f"level {lv:02d}: duplicate function start at {addr:08X} "
                                 f"({entries[addr][0]} and {row.name})")
            entries[addr] = (row.name, row.size)
    return level_entries, targets


def build_boundaries(entries, text_addr, text_end):
    """[(address, name_or_None, size)] tiling [text_addr, text_end)."""
    out, cur = [], text_addr
    for addr, (name, size) in sorted(entries.items()):
        if addr < cur:
            raise SystemExit(f"overlapping function start at {addr:08X} (cur {cur:08X})")
        if addr > cur:
            out.append((cur, None, addr - cur))
        out.append((addr, name, size))
        cur = addr + size
    if cur < text_end:
        out.append((cur, None, text_end - cur))
    elif cur > text_end:
        raise SystemExit(f"function runs past end of text: {cur:08X} > {text_end:08X}")
    return out


def write_level_inputs(boundaries, text_addr, text_end, level_dir):
    level_dir.mkdir(parents=True, exist_ok=True)
    csv_path, sym_path = level_dir / "splits.csv", level_dir / "symbols.txt"
    with csv_path.open("w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["0", format(text_addr, "X"), ".text"])
        for addr, name, _size in boundaries:
            fname = f"gap{addr:08X}" if name is None else f"f{addr:08X}"
            w.writerow([format(addr - text_addr, "X"), format(addr, "X"), fname])
        w.writerow([format(text_end - text_addr, "X"), format(text_end, "X"), ".end"])
    with sym_path.open("w") as f:
        for addr, name, size in boundaries:
            if name is not None:
                f.write(f"{name} = 0x{addr:X}; // type:func size:0x{size:X}\n")
    return csv_path, sym_path


def resident_symbols() -> list[tuple[int, str, int, str]]:
    """(address, name, size, kind) of everything resident the executable
    names: its C units below 0x15EF00 (FUN_xxxxxxxx) and the data symbols of
    the config (symbol_addrs.txt, undefined_syms*.txt, the baseline's
    generated undefined_syms_auto.txt when present)."""
    out = []
    for _owner, vram, size in ov.exe_units():
        if vram < ov.RESIDENT_MAX:
            out.append((vram, f"FUN_{vram:08x}", size, "func"))
    seen = {a for a, _n, _s, _k in out}
    config = ov.ROOT / "config/us"
    files = [config / "symbol_addrs.txt", config / "undefined_syms.txt",
             config / "undefined_funcs_auto.txt",
             ov.ROOT / "build/baseline/config/us/undefined_syms_auto.txt"]
    for path in files:
        if not path.is_file():
            continue
        for line in path.read_text().splitlines():
            m = ASSIGN_RE.match(line)
            if not m:
                continue
            addr = int(m.group(2), 0)
            if addr in seen or not 0x100000 <= addr < ov.RESIDENT_MAX:
                continue
            kind = "func" if "type:func" in line else "data"
            if kind == "data" and (m.group(1).startswith("FUN_") or m.group(1).startswith("func_")):
                kind = "func"
            out.append((addr, m.group(1), 4, kind))
            seen.add(addr)
    return sorted(out)


def write_resident_symbols(path: Path) -> int:
    entries = resident_symbols()
    with path.open("w") as f:
        for addr, name, size, kind in entries:
            f.write(f"{name} = 0x{addr:08X}; // type:{kind} size:0x{size:X}\n")
    return len(entries)


# ---------------------------------------------------------------- pass 2 data symbols

_HI_RE = re.compile(r"lui\s+(\$\w+),\s*\(0x([0-9A-Fa-f]+)\s*>>\s*16\)")
_LOAD_STORE_OPS = {
    "lb", "lbu", "lh", "lhu", "lw", "lwu", "ld", "lq",
    "sb", "sh", "sw", "sd", "sq",
    "lwc1", "swc1", "ldc1", "sdc1", "lwc2", "swc2", "lqc2", "sqc2",
}
_LABEL_LINE_RE = re.compile(r"^\s*\.L[0-9A-Fa-f]+")
_PAIR_RE = re.compile(
    r"\*/\s+(\w+)\s+\$\w+,\s*(?:(-?0x[0-9A-Fa-f]+)|\(0x([0-9A-Fa-f]+) & 0xFFFF\))\((\$\w+)\)\s*$")


def scan_for_data_symbols(lv: int, split_dir: Path, text_addr: int) -> dict[int, str]:
    found: dict[int, str] = {}
    for path in split_dir.glob("**/*.s"):
        lines = path.read_text().splitlines()
        for i, line in enumerate(lines):
            m = _HI_RE.search(line)
            if not m:
                continue
            reg, hi_val = m.group(1), int(m.group(2), 16)
            for j in range(i + 1, min(i + 3, len(lines))):
                line2 = lines[j]
                if _LABEL_LINE_RE.match(line2):
                    break
                m2 = _PAIR_RE.search(line2)
                if m2 and m2.group(1) in _LOAD_STORE_OPS and m2.group(4) == reg:
                    if m2.group(3) is not None:
                        addr = int(m2.group(3), 16)
                    else:
                        addr = (hi_val + int(m2.group(2), 16)) & 0xFFFFFFFF
                    if 0x100000 <= addr < ov.RESIDENT_MAX:
                        found[addr] = f"D_{addr:08X}"
                    elif ov.RESIDENT_MAX <= addr < text_addr:
                        found[addr] = f"D_L{lv:02d}_{addr:08X}"
                    break
    return found


# ---------------------------------------------------------------- jump tables

_JTBL_REF_RE = re.compile(r"jtbl_(?:L\d{2}_)?[0-9A-Fa-f]{8}(?:" + re.escape(SUFFIX) + r")?")
_JTBL_ADDR_RE = re.compile(r"jtbl_(?:L\d{2}_)?([0-9A-Fa-f]{8})")


def read_jump_table_entries(lv: int, jtbl_addr: int) -> list[int]:
    """Entries (absolute addresses) of the table at jtbl_addr, read from the
    level's data (or lit) record; stops at the first word outside the text."""
    text_base, text = ov.text_record(lv)
    for rec_name in ("data", "lit"):
        rec = ov.record(lv, rec_name)
        if not (rec["address"] <= jtbl_addr < rec["address"] + rec["bytes"]):
            continue
        raw, offset, entries = rec["data"], jtbl_addr - rec["address"], []
        while offset + 4 <= len(raw):
            word = int.from_bytes(raw[offset:offset + 4], "little")
            if not (text_base <= word < text_base + len(text)):
                break
            entries.append(word)
            offset += 4
        if entries:
            return entries
    return []


def ensure_local_label(lines: list[str], addr: int) -> None:
    tag = f"{addr:08X}"
    label_re = re.compile(rf"^\s*\.L{tag}(?:{re.escape(SUFFIX)})?:\s*$")
    if any(label_re.match(line) for line in lines):
        return
    instr_re = re.compile(rf"^\s*/\* [0-9A-Fa-f]+ {tag} [0-9A-Fa-f]{{8}} \*/")
    for i, line in enumerate(lines):
        if instr_re.match(line):
            lines.insert(i, f".L{tag}:\n")
            return
    raise ValueError(f"address {tag} is not an instruction in this function")


def resolve_jump_tables(content: str, lv: int, func_start: int, func_end: int):
    names = sorted(set(_JTBL_REF_RE.findall(content)))
    if not names:
        return content
    lines = content.splitlines(keepends=True)
    blocks = []
    for qualified in names:
        addr = int(_JTBL_ADDR_RE.search(qualified).group(1), 16)
        entries = read_jump_table_entries(lv, addr)
        if not entries:
            return None
        try:
            for e in entries:
                if func_start <= e < func_end:
                    ensure_local_label(lines, e)
        except ValueError:
            return None
        table = "".join(f"    .word .L{e:08X}\n" if func_start <= e < func_end
                        else f"    .word 0x{e:08X}\n" for e in entries)
        name = f"jtbl_L{lv:02d}_{addr:08X}"
        blocks.append(f"dlabel {name}\n{table}")
    content = "".join(lines).rstrip("\n") + "\n"
    content += ".section .rodata\n.align 3\n" + "".join(blocks) + ".section .text\n"
    # every spelling of the table's name in the code becomes the qualified one
    for qualified in names:
        addr = int(_JTBL_ADDR_RE.search(qualified).group(1), 16)
        content = content.replace(qualified, f"jtbl_L{lv:02d}_{addr:08X}")
    return content


# ---------------------------------------------------------------- branches

_BRANCH_LINE_RE = re.compile(
    r"^(?P<pre>\s*/\* [0-9A-Fa-f]+ )(?P<vaddr>[0-9A-Fa-f]{8})(?P<mid> )(?P<bytes>[0-9A-Fa-f]{8})"
    r"(?P<post> \*/)(?P<sp>\s+)(?P<text>\S.*\S|\S)[ \t]*$")
_REL_BRANCH_OPCODES = {0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17}
_REGIMM_BRANCH_RT = {0x00, 0x01, 0x02, 0x03, 0x10, 0x11, 0x12, 0x13}


def branch_target(word: int, vaddr: int) -> int | None:
    opcode = (word >> 26) & 0x3F

    def rel():
        imm = word & 0xFFFF
        if imm & 0x8000:
            imm -= 0x10000
        return (vaddr + 4 + imm * 4) & 0xFFFFFFFF

    if opcode in _REL_BRANCH_OPCODES:
        return rel()
    if opcode == 0x01 and ((word >> 16) & 0x1F) in _REGIMM_BRANCH_RT:
        return rel()
    if opcode in (0x11, 0x12) and ((word >> 21) & 0x1F) == 0x08:  # COP1/COP2 BC
        return rel()
    return None


def neutralize_far_branches(content, lv, func_start, func_end, level_entries, targets):
    """A branch whose retail target is outside this function becomes a `.word`
    of retail's encoding: the assembler cannot express a PC-relative
    relocation to another object, and a `j` into the middle of a neighbour
    (a `.L` label that is not defined here) would stay unresolved. A `j`/`jal`
    to a function start keeps its symbol."""
    out, changed = [], 0
    for line in content.splitlines(keepends=True):
        m = _BRANCH_LINE_RE.match(line)
        if not m:
            out.append(line)
            continue
        vaddr = int(m.group("vaddr"), 16)
        word = int.from_bytes(bytes.fromhex(m.group("bytes")), "little")
        target = branch_target(word, vaddr)
        text = m.group("text")
        if target is None and (word >> 26) == 0x02 and " .L" in text:
            target = ((word & 0x3FFFFFF) << 2) | (vaddr & 0xF0000000)
        if target is None or func_start <= target < func_end:
            out.append(line)
            continue
        comment = text.strip()
        nl = "\n" if line.endswith("\n") else ""
        out.append(f"{m.group('pre')}{m.group('vaddr')}{m.group('mid')}{m.group('bytes')}"
                   f"{m.group('post')}{m.group('sp')}.word 0x{word:08X} /* {comment} */{nl}")
        changed += 1
    return "".join(out), changed


BRANCH_LINE = re.compile(r"^(\s*/\* [0-9A-F]+ [0-9A-F]{8} ([0-9A-F]{8}) \*/\s+)"
                         r"(b(?!reak)[a-z0-9.]*)(\s+)(.*?(\.L[0-9A-F]+))\s*$")
LABEL_LINE = re.compile(r"^\s*(\.L[0-9A-F]+):\s*$")


def word_backward_branches(content: str) -> tuple[str, int]:
    seen, out, n = set(), [], 0
    for line in content.split("\n"):
        if (m := LABEL_LINE.match(line)):
            seen.add(m.group(1))
        elif (m := BRANCH_LINE.match(line)) and m.group(6) in seen:
            word = int.from_bytes(bytes.fromhex(m.group(2)), "little")
            line = f"{m.group(1)}.word 0x{word:08X} /* {m.group(3)}{m.group(4)}{m.group(5)} */"
            n += 1
        out.append(line)
    return "\n".join(out), n


# ---------------------------------------------------------------- naming

HWADDR_RE = re.compile(r"%(hi|lo)\((?:D|FUN|func)_(?:L\d\d_)?([0-9A-Fa-f]{8})\)")


_VU_SPECIAL_RE = re.compile(r"\$(ACC|Q|I|R|P)\b")


def gas_vu_registers(content: str) -> str:
    """The game compiler's gas spells the VU special registers without the
    dollar (``vdiv Q, ...``, ``vmulax.xyzw ACC, ...``), as the executable's
    expected asm does; rabbitizer writes ``$Q``/``$ACC``."""
    return _VU_SPECIAL_RE.sub(r"\1", content)


def unname_hardware(content: str) -> str:
    def repl(m: re.Match) -> str:
        addr = int(m.group(2), 16)
        if addr < ov.RAM_END:
            return m.group(0)
        if m.group(1) == "hi":
            return f"0x{((addr + 0x8000) >> 16) & 0xFFFF:X}"
        lo = addr & 0xFFFF
        return f"0x{lo:X}" if lo < 0x8000 else f"-0x{0x10000 - lo:X}"
    return HWADDR_RE.sub(repl, content)


PAIRED_LO = re.compile(r"^(\s*/\*[^*]*\*/\s+)(l[bhwdq]u?|lwc1|lqc2|s[bhwdq]|swc1|sqc2|addiu)(\s+.*?)"
                       r"\(0x([0-9A-F]+) & 0xFFFF\)(.*)$")


def name_paired_addresses(content: str, level: int) -> str:
    named, out = {}, []
    for line in content.split("\n"):
        m = PAIRED_LO.match(line)
        if m and 0x100000 <= int(m.group(4), 16) < ov.RAM_END:
            addr = int(m.group(4), 16)
            named[addr] = ov.data_name(level, addr)
            line = f"{m.group(1)}{m.group(2)}{m.group(3)}%lo({named[addr]}){m.group(5)}"
        out.append(line)
    content = "\n".join(out)
    for addr, name in named.items():
        content = re.sub(rf"(lui\s+\$\w+, )\(0x{addr:X} >> 16\)", rf"\g<1>%hi({name})", content)
    return content


GP_ACCESS = re.compile(r"^(\s*/\*[^*]*\*/\s+\S+\s+.*?)(-?0x[0-9A-Fa-f]+)\(\$28\)\s*$")


_HILO_LINE = re.compile(r"^(\s*/\* [0-9A-F]+ [0-9A-F]{8} ([0-9A-F]{8}) \*/\s+\S+\s+.*?)%(hi|lo)\(([^)]+)\)(.*)$")
_SYM_ADDR = re.compile(r"^(?:D_|FUN_|func_|jtbl_)(?:L\d{2}_)?([0-9A-Fa-f]{8})$")


def literalize_mislabeled_pairs(content: str) -> tuple[str, int]:
    """spimdisasm pairs a `lui` with the wrong `addiu` now and then and names
    both after a symbol whose low half is not what the bytes hold. Such a
    symbol (no `%lo` line of it carries its own low half) is replaced by the
    encoded immediates: `lui $r, 0xHHHH` and `addiu $r, $r, 0xLLLL`."""
    lines = content.split("\n")
    seen: dict[str, list[tuple[int, str, int]]] = {}
    for i, line in enumerate(lines):
        m = _HILO_LINE.match(line)
        if m:
            word = int.from_bytes(bytes.fromhex(m.group(2)), "little")
            seen.setdefault(m.group(4).split("+")[0].strip(), []).append((i, m.group(3), word & 0xFFFF))
    bad = set()
    for sym, uses in seen.items():
        a = _SYM_ADDR.match(sym)
        if a is None:
            continue
        addr = int(a.group(1), 16)
        los = [imm for _i, kind, imm in uses if kind == "lo"]
        if los and all(imm != (addr & 0xFFFF) for imm in los):
            bad.add(sym)
    n = 0
    for sym in bad:
        for i, kind, imm in seen[sym]:
            m = _HILO_LINE.match(lines[i])
            value = imm if kind == "hi" or imm < 0x8000 else imm - 0x10000
            lit = f"0x{value:X}" if value >= 0 else f"-0x{-value:X}"
            lines[i] = f"{m.group(1)}{lit}{m.group(5)}"
            n += 1
    return "\n".join(lines), n


def comment_gp_accesses(content: str, level: int) -> str:
    """A `$gp`-relative access keeps its retail offset (the assembler takes
    `%gp_rel` only on some instructions, and the bytes are what the proof
    compares) and gets the global's name in a comment: gp is 0x166C00 in every
    level (docs/overlays.md)."""
    out = []
    for line in content.split("\n"):
        m = GP_ACCESS.match(line)
        if m:
            addr = (ov.GP + int(m.group(2), 16)) & 0xFFFFFFFF
            line = f"{line}  /* {ov.data_name(level, addr)} */"
        out.append(line)
    return "\n".join(out)


def qualify(content: str, level: int) -> str:
    """Peel the autogen suffix off; an invented name in the level's own range
    gets the level qualifier; a FUN_ name follows the lower-case convention."""
    def repl(m: re.Match) -> str:
        prefix, addr_hex = m.group(1), m.group(2)
        addr = int(addr_hex, 16)
        if prefix in ("FUN_", "func_"):
            return f"FUN_L{level:02d}_{addr:08x}" if addr >= ov.RESIDENT_MAX else f"FUN_{addr:08x}"
        if addr >= ov.RESIDENT_MAX:
            return f"{prefix}L{level:02d}_{addr_hex.upper()}"
        return f"{prefix}{addr_hex.upper()}"
    content = SYM_RE.sub(repl, content)
    return LOCAL_RE.sub(r"\1", content)


# ---------------------------------------------------------------- spimdisasm

def run_spimdisasm(jobs: list[dict], resident: Path) -> None:
    from spimdisasm.singleFileDisasm.SingleFileDisasmInternals import (  # type: ignore
        getArgsParser, processArguments)
    parser = getArgsParser()
    for job in jobs:
        sym_files = [str(resident), job["symbols"]]
        if job.get("extra_symbols"):
            sym_files.append(job["extra_symbols"])
        argv = [job["text_bin"], job["out_whole"], "--vram", job["vram"], "--start", "0x0",
                "--end", job["end"], "--file-splits", job["splits"],
                "--split-functions", job["out_split"]]
        for sp in sym_files:
            argv += ["--symbol-addrs", sp]
        argv += ["--instr-category", "r5900", "--compiler", "EEGCC", "--endian", "little",
                 "--opcode-ljust", "10", "--custom-suffix", SUFFIX, "--no-glabel-count",
                 "--Mgpr-names", "numeric", "--Mfpr-names", "numeric",
                 "--no-use-fpccsr", "--no-cop0-named-registers", "--no-name-vars-by-section",
                 "--no-name-vars-by-type", "--no-name-vars-by-file", "--quiet"]
        import rabbitizer
        rabbitizer.config.pseudos_pseudoMove = False  # addu/daddu/or must stay distinct
        rc = processArguments(parser.parse_args(argv))
        if rc:
            sys.exit(f"level {job['level']:02d}: spimdisasm exited {rc}")
        print(f"level {job['level']:02d}: disassembled", flush=True)


# ---------------------------------------------------------------- main

def generate(levels: list[int] | None, keep_work: bool, word_backward: bool) -> int:
    rows = ov.read_catalogue()
    level_entries, targets = build_level_data(rows)
    ids = levels if levels is not None else ov.level_ids()
    if not keep_work:
        shutil.rmtree(WORK, ignore_errors=True)
    WORK.mkdir(parents=True, exist_ok=True)
    resident = WORK / "resident_symbols.txt"
    print(f"resident symbol table: {write_resident_symbols(resident)} entries")

    jobs, canonical_paths = [], {}
    for lv in ids:
        text_addr, text = ov.text_record(lv)
        text_end = text_addr + len(text)
        boundaries = build_boundaries(level_entries.get(lv, {}), text_addr, text_end)
        level_dir = WORK / f"level_{lv:02d}"
        csv_path, sym_path = write_level_inputs(boundaries, text_addr, text_end, level_dir)
        split_dir = level_dir / "split"
        jobs.append({"level": lv, "text_bin": str(ov.RECORDS_DIR / f"level_{lv:02d}/text.bin"),
                     "vram": hex(text_addr), "end": hex(len(text)), "splits": str(csv_path),
                     "symbols": str(sym_path), "out_whole": str(level_dir / "whole"),
                     "out_split": str(split_dir)})
        for addr, name, size in boundaries:
            if name is not None and name in targets and targets[name][:2] == (lv, addr):
                canonical_paths[name] = (split_dir / f"f{addr:08X}" / f"{name}.s", lv, size)

    print(f"pass 1/2: disassembling {len(jobs)} level(s) with spimdisasm ...", flush=True)
    run_spimdisasm(jobs, resident)
    total = 0
    for job in jobs:
        found = scan_for_data_symbols(job["level"], Path(job["out_split"]), int(job["vram"], 16))
        path = Path(job["symbols"]).parent / "extra_data_symbols.txt"
        path.write_text("".join(f"{name} = 0x{addr:08X}; // type:data size:0x4\n"
                                for addr, name in sorted(found.items())))
        job["extra_symbols"] = str(path)
        total += len(found)
    print(f"pass 2/2: {total} level-own data symbols from pass 1's leftovers, re-disassembling ...",
          flush=True)
    run_spimdisasm(jobs, resident)

    OUT.mkdir(parents=True, exist_ok=True)
    written, skipped_jtbl, missing, bad_size = [], [], [], []
    n_jtbl = n_far = n_back = n_lit = 0
    for name, (path, lv, size) in sorted(canonical_paths.items()):
        if not path.exists():
            missing.append(name)
            continue
        content = path.read_text()
        if "jlabel_" in content:
            skipped_jtbl.append(name)
            continue
        addr = targets[name][1]
        if "jtbl_" in content:
            resolved = resolve_jump_tables(content, lv, addr, addr + size)
            if resolved is None:
                skipped_jtbl.append(name)
                continue
            content = resolved
            n_jtbl += 1
        content = qualify(content, lv)
        content = unname_hardware(content)
        content = gas_vu_registers(content)
        content = name_paired_addresses(content, lv)
        content = comment_gp_accesses(content, lv)
        content, n = literalize_mislabeled_pairs(content)
        n_lit += n
        content, n = neutralize_far_branches(content, lv, addr, addr + size, level_entries, targets)
        n_far += n
        if word_backward:
            content, n = word_backward_branches(content)
            n_back += n
        if addr % 8:
            content = content.replace(".align 3\n", "", 1)
        content = ".set noat\n" + content
        if SUFFIX in content:
            print(f"warning: {name}: autogen suffix survived requalification", file=sys.stderr)
        (OUT / f"{name}.s").write_text(content)
        written.append(name)

    if skipped_jtbl:
        (WORK / "skipped_jumptables.txt").write_text("\n".join(skipped_jtbl) + "\n")
    print(f"wrote {len(written)} file(s) to {OUT.relative_to(ov.ROOT)}; "
          f"{n_jtbl} jump-table functions resolved; {n_lit} mislabeled pair lines literalized; {n_far} out-of-range branches and "
          f"{n_back} backward branches written as .word")
    if skipped_jtbl:
        print(f"skipped {len(skipped_jtbl)} function(s) with an unhandled jump table "
              f"({(WORK / 'skipped_jumptables.txt').relative_to(ov.ROOT)}): "
              + ", ".join(skipped_jtbl[:10]) + (", ..." if len(skipped_jtbl) > 10 else ""))
    if missing:
        print(f"MISSING (spimdisasm did not produce a file): {', '.join(missing[:20])}", file=sys.stderr)
    if bad_size:
        print(f"BAD SIZE: {', '.join(bad_size[:20])}", file=sys.stderr)
    return 1 if (missing or bad_size) else 0




def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--iso", type=Path, help="your USA disc image; omit to reuse the records")
    parser.add_argument("--levels", help="comma-separated level numbers (default: all)")
    parser.add_argument("--keep-work", action="store_true", help="keep build/overlay-asm")
    parser.add_argument("--word-backward-branches", action="store_true", help=argparse.SUPPRESS)
    args = parser.parse_args(argv)
    if args.iso and extract_records(args.iso):
        return 1
    if not ov.level_ids():
        sys.exit("no records in config/us/overlays/: run with --iso PATH first")
    if not (ov.ROOT / "build/baseline/config/us/undefined_syms_auto.txt").is_file():
        sys.exit("run `make elf` first: the listings name the executable's symbols it generates")
    levels = [int(x) for x in args.levels.split(",")] if args.levels else None
    return generate(levels, args.keep_work, args.word_backward_branches)


if __name__ == "__main__":
    raise SystemExit(main())
