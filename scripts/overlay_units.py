"""The level overlay functions as progress units (docs/overlays.md).

Reads ``config/overlays/us/functions.tsv`` (the catalogue: every distinct
function of the 19 level programs, once) and ``src/overlays/``: a shared or
level function is C_EXACT when its C is in ``src/overlays/`` (it is promoted
there only after the byte proof) and pending while its line there is an
``INCLUDE_ASM`` stub. ``exe`` rows are the executable's own functions and are
counted with the executable, not here. A function listed as
``overlays/<name>`` under ``intentional_asm`` in
``config/us/unit_categories.json`` is retail hand asm (VU0/COP2, MMI no
compiler emits, ``addi``): it carries ``asm`` and stays out of C_EXACT.

Needs no retail bytes, so ``gen_progress_report.py`` and the map
work from the repository alone.
"""
from __future__ import annotations

import json
import re
from pathlib import Path

CATALOGUE = Path("config/overlays/us/functions.tsv")
LEVELS = Path("config/overlays/us/levels.json")
CATEGORIES = Path("config/us/unit_categories.json")
SOURCES = Path("src/overlays")
OVERLAY_FUNC_RE = re.compile(r"^FUN_L(\d{2})_([0-9a-f]{8})$")
STUB_RE = re.compile(r'INCLUDE_ASM\s*\(\s*"[^"]*"\s*,\s*(FUN_L\d{2}_[0-9a-f]{8})\s*\)')
# a definition's first line: unindented, not `extern`, and either the
# parameter list closes the line or the body opens on the same line (a
# one-line definition); a prototype (its `;` right after the list) of a
# function whose stub lives in another file is not that function's C
DEF_RE = re.compile(r"^(?!extern\b)[A-Za-z_][\w\s\*]*?\b(FUN_L\d{2}_[0-9a-f]{8})\s*\([^;{}\n]*(?:\{|$)", re.M)


def load_levels(repo: Path) -> dict[int, dict]:
    path = repo / LEVELS
    if not path.is_file():
        return {}
    return {int(lv["index"]): lv for lv in json.loads(path.read_text())["levels"]}


def intentional_asm(repo: Path) -> set[str]:
    """Overlay function names listed as intentional asm."""
    path = repo / CATEGORIES
    if not path.is_file():
        return set()
    entries = json.loads(path.read_text()).get("intentional_asm", [])
    return {e.removeprefix("overlays/") for e in entries if e.startswith("overlays/")}


def source_state(repo: Path) -> tuple[dict[str, str], dict[str, str]]:
    """(stubs, c): function name -> file (relative to the repo) for every
    INCLUDE_ASM stub and every C definition under src/overlays/."""
    stubs: dict[str, str] = {}
    c: dict[str, str] = {}
    root = repo / SOURCES
    if not root.is_dir():
        return stubs, c
    for path in sorted(root.glob("**/*.c")):
        rel = str(path.relative_to(repo))
        text = path.read_text(errors="replace")
        for name in STUB_RE.findall(text):
            stubs[name] = rel
        for name in DEF_RE.findall(text):
            if name not in stubs:
                c[name] = rel
    return stubs, c


def overlay_functions(repo: Path) -> list[dict]:
    """Every shared and level function: name, kind, level, address, size,
    levels (count), file, exact, asm. Empty when the catalogue is absent."""
    path = repo / CATALOGUE
    if not path.is_file():
        return []
    stubs, c = source_state(repo)
    asm = intentional_asm(repo)
    out = []
    for line in path.read_text().splitlines():
        if not line or line.startswith("#"):
            continue
        cols = line.split("\t")
        name, kind, size = cols[0], cols[1], int(cols[2])
        if kind == "exe":
            continue
        m = OVERLAY_FUNC_RE.match(name)
        if not m:
            continue
        level, address = int(m.group(1)), int(m.group(2), 16)
        exact = name in c
        file = c.get(name) or stubs.get(name) or ""
        out.append({
            "name": name, "kind": kind, "level": level, "address": address, "size": size,
            "levels": int(cols[4]) if len(cols) > 4 and cols[4].isdigit() else 1,
            "file": file, "exact": exact, "asm": name in asm and not exact,
        })
    out.sort(key=lambda f: (f["kind"] != "shared", f["level"], f["address"]))
    return out


def category_of(function: dict) -> str:
    """``shared`` or ``level_NN``: the report category of one function."""
    return "shared" if function["kind"] == "shared" else f"level_{function['level']:02d}"


def group_of(function: dict) -> str:
    """The logical group: the src/overlays file path below its category
    directory, without the suffix (e.g. ``gameplay/entities/0025d1b8``), or
    the level."""
    if function["file"]:
        parts = Path(function["file"]).with_suffix("").parts
        return "/".join(parts[3:]) if parts[:2] == ("src", "overlays") else parts[-1]
    return category_of(function)
