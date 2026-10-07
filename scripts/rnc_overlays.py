"""Level overlays: records, catalogue and names (docs/overlays.md).

The records are cut from your own disc image by ``scripts/overlay-extract.py``
into ``config/us/overlays/level_NN/`` (gitignored, never committed).
"""
from __future__ import annotations

import json
import re
import struct
from functools import lru_cache
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RECORDS_DIR = ROOT / "config/us/overlays"
ASM_DIR = RECORDS_DIR / "asm"
CATALOGUE = ROOT / "config/overlays/us/functions.tsv"

RECORD_NAMES = ("lit", "bss", "data", "vtbl", "camvtbl", "sndvtbl", "text")
GP = 0x166C00              # _gp of the executable; every level inherits it
RESIDENT_MAX = 0x15EF00    # first address the level records replace
EXE_DELTA = 0xFF080        # vram - file offset of the executable's main segment
RAM_END = 0x02000000

OVERLAY_FUNC = re.compile(r"^FUN_L(\d{2})_([0-9A-Fa-f]{8})$")
OVERLAY_DATA = re.compile(r"^D_L(\d{2})_([0-9A-Fa-f]{8})$")
OVERLAY_JTBL = re.compile(r"^jtbl_L(\d{2})_([0-9A-Fa-f]{8})$")
EXE_FUNC = re.compile(r"^(?:FUN|func)_([0-9A-Fa-f]{8})$")


def data_name(level: int, address: int) -> str:
    return f"D_{address:08X}" if address < RESIDENT_MAX else f"D_L{level:02d}_{address:08X}"


def words(b: bytes) -> tuple:
    return struct.unpack(f"<{len(b) // 4}I", b)


# ---------------------------------------------------------------- records

def level_ids() -> list[int]:
    return sorted(int(p.name[6:]) for p in RECORDS_DIR.glob("level_??")
                  if (p / "manifest.json").is_file())


@lru_cache(maxsize=None)
def manifest(level: int) -> dict:
    return json.loads((RECORDS_DIR / f"level_{level:02d}/manifest.json").read_text())


def record(level: int, name: str) -> dict:
    """The record's manifest row plus its bytes (``data``)."""
    row = dict(next(r for r in manifest(level)["records"] if r["name"] == name))
    path = RECORDS_DIR / f"level_{level:02d}/{name}.bin"
    row["data"] = path.read_bytes() if row["type"] != 8 else b""
    return row


@lru_cache(maxsize=None)
def text_record(level: int) -> tuple[int, bytes]:
    rec = record(level, "text")
    return rec["address"], rec["data"]


def level_text_bytes(level: int, address: int, size: int) -> bytes:
    base, data = text_record(level)
    if not base <= address <= address + size <= base + len(data):
        raise ValueError(f"level {level:02d}: {address:#x}+{size} is outside its text")
    return data[address - base:address - base + size]


# ---------------------------------------------------------------- catalogue

class Row:
    __slots__ = ("name", "kind", "size", "places", "unit")

    def __init__(self, name, kind, size, places, unit=""):
        self.name, self.kind, self.size = name, kind, int(size)
        self.places = places          # [(level, address)] sorted
        self.unit = unit

    @property
    def canonical(self) -> tuple[int, int]:
        return self.places[0]


@lru_cache(maxsize=None)
def read_catalogue(path: Path = CATALOGUE) -> dict[str, Row]:
    rows = {}
    for line in path.read_text().splitlines():
        if not line or line.startswith("#"):
            continue
        parts = line.split("\t")
        name, kind, size, _fp, _n, places = parts[:6]
        rows[name] = Row(name, kind, size,
                         [(int(p[:2]), int(p[3:], 16)) for p in places.split(",")],
                         parts[6] if len(parts) > 6 else "")
    return rows


@lru_cache(maxsize=None)
def places_index() -> dict[int, dict[int, str]]:
    """level -> {address: name} for every place in the catalogue."""
    index: dict[int, dict[int, str]] = {}
    for row in read_catalogue().values():
        for lv, addr in row.places:
            index.setdefault(lv, {})[addr] = row.name
    return index


@lru_cache(maxsize=1)
def row_at() -> dict:
    """{(level, address): row} over every place of every catalogue row."""
    return {place: row for row in read_catalogue().values() for place in row.places}


def places_in_level(name: str, level: int) -> list[int]:
    row = read_catalogue().get(name)
    m = OVERLAY_FUNC.match(name)
    if row is None and m:
        # a name that is no longer a row of its own: one copy of another row
        row = row_at().get((int(m.group(1)), int(m.group(2), 16)))
    return sorted({a for lv, a in row.places if lv == level}) if row else []


# ---------------------------------------------------------------- the executable

ROW_RE = re.compile(r"^\s*-\s*\[(0x[0-9A-Fa-f]+)\s*,\s*([A-Za-z_]\w*)\s*,\s*([^\]]+?)\s*\]\s*$")


@lru_cache(maxsize=1)
def exe_units() -> list[tuple[str, int, int]]:
    """(owner, vram, size) of every configured C unit of the executable."""
    rows = []
    for line in (ROOT / "config/us/rnc1.us.yaml").read_text().splitlines():
        m = ROW_RE.match(line)
        if m:
            rows.append((int(m.group(1), 16), m.group(2), m.group(3)))
    rows.sort()
    out = []
    for i, (offset, kind, owner) in enumerate(rows):
        if kind != "c":
            continue
        end = rows[i + 1][0] if i + 1 < len(rows) else offset
        if end > offset:
            out.append((owner, offset + EXE_DELTA, end - offset))
    return out
