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
from collections import Counter, defaultdict
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


def _candidates(level: int, start: int, size: int) -> list[int]:
    """The deltas the level's code reads [start, start + size) at, most
    votes first, then its neighbours': a stale %hi pairs with a later %lo
    now and then and adds a false one, so each is tried and the bytes decide."""
    moved = deltas().get(level, {})
    votes: Counter = Counter()
    for a, ds in moved.items():
        if start <= a < start + size:
            votes.update(ds)
    out = [d for d, _n in votes.most_common()]
    # data no copied function reads moves with its neighbours
    below = [a for a in moved if a < start]
    above = [a for a in moved if a >= start + size]
    for near in (max(below, default=None), min(above, default=None)):
        if near is not None:
            out += sorted(moved[near] - set(out))
    return out


def proven_copies() -> set[tuple[int, int, int]]:
    """{(level, address, size)}: copies of executable data in C, equal in the level."""
    places = ov.places_index()
    out = set()
    for _unit, vma, size in c_data_entries():
        mine = boot_bytes(vma, size)
        for level in ov.level_ids():
            for d in _candidates(level, vma, size):
                theirs = ov.level_data_bytes(level, vma + d, size)
                if theirs is None:
                    continue
                if theirs == mine or _pointers_match(level, mine, theirs, d, places.get(level, {})):
                    out.add((level, vma + d, size))
                    break
    return out


def _moved(level: int, a: int, b: int) -> bool:
    """The level's code reads the executable's data at A at B, or A lies
    between two addresses its code moves by the same amount."""
    moved = deltas().get(level, {})
    if b - a in moved.get(a, ()):
        return True
    below = [x for x in moved if x < a]
    above = [x for x in moved if x > a]
    if not below or not above:
        return False
    return moved[max(below)] == moved[min(above)] == {b - a}


SAME_SHAPE = {0x03, 0x09, 0x0D, 0x0F} | set(range(0x20, 0x40))   # jal, addiu, ori, lui, loads/stores


def _same_code(level: int, a: int, b: int, name: str) -> bool:
    """The level's catalogued function NAME at B is the executable's code at
    A: the same instructions, only call targets and %hi/%lo halves moved."""
    size = ov.read_catalogue()[name].size
    mine, theirs = boot_bytes(a, size), ov.level_text_bytes(level, b, size)
    for w, v in zip(ov.words(mine), ov.words(theirs)):
        if w == v:
            continue
        if w >> 26 != v >> 26 or w >> 26 not in SAME_SHAPE:
            return False
        if w >> 26 != 0x03 and w >> 16 != v >> 16:
            return False
    return True


def _pointers_match(level: int, mine: bytes, theirs: bytes, d: int, code: dict[int, str]) -> bool:
    """Every differing word is a pointer the level moved: data by D (or as
    its code reads it), a function to its copy in this level."""
    if len(mine) % 4:
        return False
    for i in range(0, len(mine), 4):
        a = int.from_bytes(mine[i:i + 4], "little")
        b = int.from_bytes(theirs[i:i + 4], "little")
        if a == b:
            continue
        if DATA_LOW <= a < DATA_HIGH and (b == a + d or _moved(level, a, b)):
            continue
        row = ov.read_catalogue().get(f"FUN_{a:08x}")
        if row is not None and b in {p for lv, p in row.places if lv == level}:
            continue
        if a >= DATA_HIGH and b in code and _same_code(level, a, b, code[b]):
            continue
        return False
    return True
