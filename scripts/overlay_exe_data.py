"""Copies of the executable's data in C that each level holds.

A level program is linked with the executable's game units, so its `.data`
holds a copy of their data at another address. Where the executable's code
reads a data object at A, the same function's copy in level N reads A + d;
d comes from comparing the two codes' %hi/%lo pairs. The copy is proven when
the level's bytes equal the executable's bytes (which `make elf` proves equal
to the C), each pointer translated to the level the same way.
"""
from __future__ import annotations

import ast
from collections import defaultdict
from functools import lru_cache

import rnc_overlays as ov

ELF = ov.ROOT / "config/us/SCUS_971.99"
TABLES = ("RODATA_OVERLAYS", "SDATA_OVERLAYS", "DATA_OVERLAYS")
DATA_LOW, DATA_HIGH = 0x130000, 0x200000   # the executable's data (core_data .. data)


@lru_cache(maxsize=None)
def elf_bytes() -> bytes:
    return ELF.read_bytes()


def boot_bytes(address: int, size: int) -> bytes:
    off = address - ov.EXE_DELTA
    return elf_bytes()[off:off + size]


def c_data_entries() -> list[tuple[str, int, int]]:
    """(unit, VMA, size) of every *_OVERLAYS entry in configure.py."""
    out = []
    for node in ast.parse((ov.ROOT / "configure.py").read_text()).body:
        if isinstance(node, ast.Assign) and getattr(node.targets[0], "id", None) in TABLES:
            out += [(unit, vma, size) for unit, (vma, _off, size) in ast.literal_eval(node.value).items()]
    return out


def _addresses(code: bytes) -> dict[int, int]:
    """Offset of each %lo use -> the address its %hi/%lo pair forms."""
    hi = {}
    out = {}
    for off in range(0, len(code) - 3, 4):
        w = int.from_bytes(code[off:off + 4], "little")
        op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
        if op == 0x0F:                      # lui
            hi[rt] = imm << 16
            continue
        if rs in hi and op in (0x09, 0x0D, 0x19) or rs in hi and op >= 0x20:
            lo = imm - 0x10000 if imm & 0x8000 and op != 0x0D else imm
            out[off] = (hi[rs] + lo) & 0xFFFFFFFF
            if op in (0x09, 0x0D, 0x19) and rt == rs:
                hi.pop(rs, None)
    return out


@lru_cache(maxsize=None)
def deltas() -> dict[int, dict[int, set[int]]]:
    """level -> {executable data address: {level address - it}}, from every
    function the levels copy from the executable."""
    out: dict[int, dict[int, set[int]]] = defaultdict(lambda: defaultdict(set))
    for row in ov.read_catalogue().values():
        if row.kind != "exe":
            continue
        address = int(row.name[-8:], 16)
        mine = _addresses(boot_bytes(address, row.size))
        for level, place in row.places:
            theirs = _addresses(ov.level_text_bytes(level, place, row.size))
            for off, a in mine.items():
                b = theirs.get(off)
                if b is not None and DATA_LOW <= a < DATA_HIGH and b != a:
                    out[level][a].add(b - a)
    return out


def _delta(level: int, start: int, size: int) -> int | None:
    found = {d for a, ds in deltas().get(level, {}).items() if start <= a < start + size for d in ds}
    return found.pop() if len(found) == 1 else None


def proven_copies() -> set[tuple[int, int, int]]:
    """{(level, address, size)}: copies of executable data in C, equal in the level."""
    places = ov.places_index()
    out = set()
    for _unit, vma, size in c_data_entries():
        mine = boot_bytes(vma, size)
        for level in ov.level_ids():
            d = _delta(level, vma, size)
            if d is None:
                continue
            theirs = ov.level_data_bytes(level, vma + d, size)
            if theirs is None:
                continue
            if theirs == mine or _pointers_match(level, mine, theirs, d, places.get(level, {})):
                out.add((level, vma + d, size))
    return out


def _pointers_match(level: int, mine: bytes, theirs: bytes, d: int, code: dict[int, str]) -> bool:
    """Every differing word is a pointer the level moved: data by D, a
    function to its copy in this level."""
    if len(mine) % 4:
        return False
    for i in range(0, len(mine), 4):
        a = int.from_bytes(mine[i:i + 4], "little")
        b = int.from_bytes(theirs[i:i + 4], "little")
        if a == b:
            continue
        if DATA_LOW <= a < DATA_HIGH and b == a + d:
            continue
        row = ov.read_catalogue().get(f"FUN_{a:08x}")
        if row is not None and b in {p for lv, p in row.places if lv == level}:
            continue
        return False
    return True
