#!/usr/bin/env python3
"""Generate decomp_map.svg - a treemap of logical function-group progress.

What the map shows
  Game and SDK show every logical function group as its own tile. The map
  reserves 25% of its area for SDK and 75% for Game; tiles are byte-proportional
  within each half-width pane. The companion JSON retains the same groups.
  Labels show each group's byte-weighted C_EXACT and C_FUZZY progress.
  Assignments come from ``rename_proposals.entries[].logical_group`` where
  available; conservative fallbacks use non-architectural source-module buckets.

    * bolt orange (#dd8b30) - every recoverable function in the group is
      matching C: promoted source or a legacy exact unit listed in
      ``config/us/unit_categories.json``.
    * warm copper shades - partial C_EXACT coverage; C_FUZZY does not affect
      tile colors, and only fully exact groups reach bolt orange.
    * chrome (#c3cbd8) - the group contains intentional low-level asm only:
      hand-written SIMD/VU0/MMI code excluded from the C goal.
    * dark plate (#2e3644) - no C_EXACT progress; C_FUZZY never changes color.

  The Unclassified tile is visually capped at 16 KiB so it does not dominate
  the whole map. Its tooltip and progress numbers retain the true byte total.

  The palette follows the *Ratchet & Clank* (2002) logo: Ratchet's bolt
  orange, Clank's chrome, and the dark riveted plate behind them.

  With ``--workspace`` the map also reports C_FUZZY over the same recoverable
  units as C_EXACT: matching units count 100%, every pending unit contributes
  its measured objdiff ``.text`` similarity, weighted by unit bytes.  The
  scores come from ``scripts/list-functions.py --score``.

  ``--min-bytes`` can combine small groups per category and status;
  by default, every logical group is shown individually.

Layout
  SDK occupies a horizontal band with 25% of the map area; Game uses the
  remaining 75%. Each band is split into two half-width treemaps, so no group
  tile spans more than half the map width. The ``squarify`` package is used
  when installed (``pip install squarify``), otherwise the bundled equivalent.

Usage
  python3 scripts/generate_treemap.py
  python3 scripts/generate_treemap.py --min-bytes 512 --output assets/decomp_map.svg
  python3 scripts/generate_treemap.py --width 800 --height 400   # compact variant
"""

from __future__ import annotations

import argparse
import datetime as dt
import html
import json
import math
import re
import subprocess
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
from progress_groups import (  # noqa: E402
    committed_function_scores,
    group_for_owner,
    load_group_assignments,
    report_category_for_owner,
    report_group_name,
)

# Ratchet & Clank (2002) logo palette: Ratchet's bolt orange, Clank's chrome,
# and the dark riveted plate behind them. The canvas stays GitHub dark.
ORANGE = "#dd8b30"
CHROME = "#c3cbd8"
PLATE = "#2e3644"
BACKGROUND = "#0d1117"
STROKE = "#0d1117"
TEXT = "#e8edf5"
MUTED = "#a6adc8"
FONT = "ui-sans-serif, -apple-system, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif"

# 12x12 hex-nut "bolt" mark (the series' currency) used as the title glyph.
BOLT_PATH = (
    "M6 0.7 L11.3 3.5 L11.3 8.5 L6 11.3 L0.7 8.5 L0.7 3.5 Z "
    "M3.8 6 A2.2 2.2 0 1 1 8.2 6 A2.2 2.2 0 1 1 3.8 6 Z"
)
# Tile label inks: dark ink on the bright orange and chrome plates, light ink
# on the dark pending plate.
LABEL_FILLS = {
    "exact": ("#2b1604", "#5d3512"),
    "asm": ("#10161e", "#4d5768"),
}
# Partial groups use a copper range; a single soft sheen is laid over the map
# instead of repeating a gradient inside every tile.
PARTIAL_ORANGE = "#bf7a2e"
UNCLASSIFIED_LAYOUT_CAP_BYTES = 16 * 1024

ROW_RE = re.compile(
    r"^\s*-\s*\[(0x[0-9A-Fa-f]+)\s*,\s*([A-Za-z_][A-Za-z0-9_]*)\s*,\s*([^\]]+?)\s*\]\s*$"
)

# --------------------------------------------------------------------------
# Display names: presentation-only tile labels.  Canonical identity (the unit
# path and its FUN_xxxx symbol) never changes; every renderer keeps it in the
# tile tooltip.  Sources, in priority order:
#   1. the first function name in the unit's C source,
#   2. ``config/us/recovered_names.json`` entries with ``match == "full"``
#      whose recovered function starts exactly at the unit start,
#   3. no evidence -> the unit basename (address name).
# --------------------------------------------------------------------------
FUNC_DEF_RE = re.compile(
    r"(?m)^[A-Za-z_][A-Za-z0-9_ \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{"
)
INCLUDE_ASM_RE = re.compile(r'INCLUDE_ASM\s*\(\s*"[^"]*"\s*,\s*([A-Za-z_]\w*)\s*\)')
ADDR_SYMBOL_RE = re.compile(r"^(?:FUN_|func_|D_|DAT_)[0-9A-Fa-f]+$")


def demangle_cfront(name: str) -> str:
    """`videoDecAbort__FP8VideoDec` -> `videoDecAbort` (display only)."""
    stem = name.split("__F", 1)[0]
    return stem if stem[:1].isalpha() else name


def load_recovered_full(config_dir: Path) -> dict[str, str]:
    """owner -> recovered name for `full` matches starting at the unit start."""
    path = config_dir / "recovered_names.json"
    if not path.is_file():
        return {}
    try:
        payload = json.loads(path.read_text())
    except (OSError, json.JSONDecodeError):
        return {}
    names: dict[str, str] = {}
    for entry in payload.get("symbols", []):
        if entry.get("match") != "full":
            continue
        unit = entry.get("unit")
        name = str(entry.get("name") or "")
        if not unit or not name:
            continue
        address, unit_address = entry.get("address"), entry.get("unit_address")
        if address and unit_address and address != unit_address:
            continue
        # The yaml owner and the recovered table agree on one of these forms.
        names.setdefault(unit, name)
        names.setdefault(unit.removeprefix("assembly/"), name)
    return names


def load_overlay_names(repo: Path) -> dict[int, str]:
    """boot address -> recovered name from the overlay name-evidence tables.

    The overlay maps carry exact boot-match evidence (``name_evidence`` with
    ``boot_addr``/``words``); a unit that starts at ``boot_addr`` is the same
    function (or its beginning), so the name is display-safe.
    """
    names: dict[int, str] = {}
    names_dir = repo / "config" / "overlays" / "us" / "names"
    if not names_dir.is_dir():
        return names
    for path in sorted(names_dir.glob("level-*.json")):
        try:
            payload = json.loads(path.read_text())
        except (OSError, json.JSONDecodeError):
            continue
        for entry in payload.get("functions", []):
            evidence = entry.get("name_evidence") or {}
            name = evidence.get("name")
            boot = evidence.get("boot_addr")
            if name and isinstance(boot, int):
                names.setdefault(boot, str(name))
    return names


def source_symbol(source: Path) -> str | None:
    """First function name (or INCLUDE_ASM symbol) in a unit source, or None."""
    text = source.read_text(errors="replace")
    match = FUNC_DEF_RE.search(text)
    if match:
        return match.group(1)
    match = INCLUDE_ASM_RE.search(text)
    return match.group(1) if match else None


def display_for(owner: str, source: Path, recovered: dict[str, str]) -> str | None:
    if source.is_file():
        name = source_symbol(source)
        if name and not ADDR_SYMBOL_RE.match(name):
            return demangle_cfront(name)
    name = recovered.get(owner) or recovered.get(owner.removeprefix("assembly/"))
    return demangle_cfront(name) if name else None


def tile_tooltip(tile) -> str:
    if tile.get("small_bucket"):
        return (
            f"{tile['group_count']} logical groups below {tile['threshold']} B "
            f"({tile['category']}) · {tile['count']} functions · {tile['size']:,} B"
        )
    if tile.get("group"):
        parts = [
            tile.get("display") or tile["logical_group"],
            tile["category"],
            f"{tile['count']} functions",
            f"{tile['size']:,} B",
        ]
        if tile.get("tree_group_count", 1) > 1:
            parts.append(f"{tile['tree_group_count']} logical groups")
        if tile.get("exact_percent") is not None:
            parts.append(f"C_EXACT {tile['exact_percent']:.2f}%")
        else:
            parts.append("intentional asm only")
        if tile.get("fuzzy_percent") is not None:
            parts.append(f"C_FUZZY {tile['fuzzy_percent']:.2f}%")
        parts.append(
            f"{tile['matching_c']} matching C · {tile['pending_c']} pending · "
            f"{tile['intentional_asm']} intentional asm"
        )
        if tile.get("layout_size", tile["size"]) < tile["size"]:
            parts.append(
                f"map area capped at {tile['layout_size']:,} B; statistics use actual size"
            )
        return " · ".join(parts)
    parts = []
    if tile.get("display"):
        parts.append(tile["display"])
    parts.append(tile["owner"])
    parts.append(f"0x{tile['address']:X}")
    parts.append(f"{tile['size']} B")
    label = {"exact": "exact C", "asm": "intentional asm", "pending": "pending C"}[
        tile["category"]
    ]
    if tile["category"] == "pending" and tile.get("score") is not None:
        label = f"{label} · {float(tile['score']):.2f}%"
    parts.append(label)
    return " · ".join(parts)


# --------------------------------------------------------------------------
# squarify: use the package when available, otherwise the same algorithm
# bundled here (MIT, https://github.com/laserson/squarify).
# --------------------------------------------------------------------------
def _layoutrow(sizes, x, y, dx, dy):
    covered = sum(sizes)
    width = covered / dy if dy else 0.0
    rects = []
    for size in sizes:
        height = size / width if width else 0.0
        rects.append({"x": x, "y": y, "dx": width, "dy": height})
        y += height
    return rects


def _layoutcol(sizes, x, y, dx, dy):
    covered = sum(sizes)
    height = covered / dx if dx else 0.0
    rects = []
    for size in sizes:
        width = size / height if height else 0.0
        rects.append({"x": x, "y": y, "dx": width, "dy": height})
        x += width
    return rects


def _layout(sizes, x, y, dx, dy):
    return (
        _layoutrow(sizes, x, y, dx, dy) if dx >= dy else _layoutcol(sizes, x, y, dx, dy)
    )


def _leftover(sizes, x, y, dx, dy):
    if dx >= dy:
        width = sum(sizes) / dy if dy else 0.0
        return (x + width, y, dx - width, dy)
    height = sum(sizes) / dx if dx else 0.0
    return (x, y + height, dx, dy - height)


def _worst_ratio(sizes, x, y, dx, dy):
    ratios = []
    for rect in _layout(sizes, x, y, dx, dy):
        if rect["dx"] <= 0 or rect["dy"] <= 0:
            continue
        ratios.append(max(rect["dx"] / rect["dy"], rect["dy"] / rect["dx"]))
    return max(ratios) if ratios else float("inf")


def _squarify(sizes, x, y, dx, dy):
    if not sizes:
        return []
    if len(sizes) == 1:
        return _layout(sizes, x, y, dx, dy)
    index = 1
    while index < len(sizes) and _worst_ratio(
        sizes[:index], x, y, dx, dy
    ) >= _worst_ratio(sizes[: index + 1], x, y, dx, dy):
        index += 1
    current, remaining = sizes[:index], sizes[index:]
    leftover = _leftover(current, x, y, dx, dy)
    return _layout(current, x, y, dx, dy) + _squarify(remaining, *leftover)


try:  # pragma: no cover - exercised by whichever branch is installed
    import squarify as _squarify_pkg
except ImportError:  # pragma: no cover
    _squarify_pkg = None


def treemap(sizes, x, y, dx, dy):
    sizes = [float(size) for size in sizes]
    total = sum(sizes)
    if total <= 0 or dx <= 0 or dy <= 0:
        return []
    # squarify (both the package and the bundled copy) expects areas, not raw
    # byte weights.
    normalized = [size * dx * dy / total for size in sizes]
    if _squarify_pkg is not None:
        return _squarify_pkg.squarify(normalized, x, y, dx, dy)
    return _squarify(normalized, x, y, dx, dy)


def balanced_columns(tiles: list[dict]) -> tuple[list[dict], list[dict]]:
    """Split tile weights as evenly as possible for a strict 50/50 layout."""
    if len(tiles) < 2:
        return list(tiles), []

    sizes = [int(tile["layout_size"]) for tile in tiles]
    target = sum(sizes) // 2
    reachable = [1]
    for size in sizes:
        reachable.append(reachable[-1] | (reachable[-1] << size))

    candidates = reachable[-1] & ((1 << (target + 1)) - 1)
    left_total = candidates.bit_length() - 1
    selected = [False] * len(tiles)
    remaining = left_total
    for index in range(len(tiles) - 1, -1, -1):
        size = sizes[index]
        if remaining >= size and (reachable[index] >> (remaining - size)) & 1:
            selected[index] = True
            remaining -= size

    left = [tile for tile, choose in zip(tiles, selected) if choose]
    right = [tile for tile, choose in zip(tiles, selected) if not choose]
    return left, right


# --------------------------------------------------------------------------
# Data
# --------------------------------------------------------------------------
def parse_units(config: Path):
    """Configured C units as (owner, address, size) sorted by address."""
    rows = []
    for line in config.read_text().splitlines():
        match = ROW_RE.match(line)
        if match:
            rows.append((int(match.group(1), 16), match.group(2), match.group(3)))
    rows.sort(key=lambda row: row[0])
    units = []
    for index, (address, kind, owner) in enumerate(rows):
        if kind != "c":
            continue
        end = rows[index + 1][0] if index + 1 < len(rows) else address
        size = end - address
        if size > 0:
            units.append((owner, address, size))
    return units


def load_categories(path: Path | None):
    """Return (exact under assembly, intentional asm) name sets."""
    if path is None or not path.is_file():
        return set(), set()
    try:
        payload = json.loads(path.read_text())
    except (OSError, json.JSONDecodeError):
        return set(), set()
    return (
        set(payload.get("exact_under_assembly", [])),
        set(payload.get("intentional_asm", [])),
    )


# A unit counts as C_EXACT only when its source is C: inline asm is allowed
# solely as a label binding a declaration to its linked name
# (`__asm__("FUN_00202d10")`). Any other asm in the file (instructions, empty
# memory barriers, `.extern` directives, register pins) keeps the unit pending
# however exact its bytes are. include/qcopy.h is the approved exception and
# lives in a header, not in the unit.
_ASM_START_RE = re.compile(r"\b(?:__asm__|__asm|asm)\b(?:\s*(?:__volatile__|volatile))?\s*\(")
_ASM_LABEL_RE = re.compile(r'\s*"[A-Za-z_.$][\w.$]*"\s*')
_COMMENT_RE = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)
# `register int x asm("v1")` pins a register; it is not a name label.
_ASM_REGISTER_RE = re.compile(
    r"\$\w+|zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra|f[0-9]|f[12][0-9]|f3[01]")


def non_label_asm(source: Path) -> bool:
    """True when the source holds inline asm other than a name label."""
    try:
        text = _COMMENT_RE.sub(" ", source.read_text(errors="replace"))
    except OSError:
        return False
    for match in _ASM_START_RE.finditer(text):
        depth, index = 1, match.end()
        while index < len(text) and depth:
            depth += {"(": 1, ")": -1}.get(text[index], 0)
            index += 1
        body = text[match.end():index - 1]
        if not _ASM_LABEL_RE.fullmatch(body) or _ASM_REGISTER_RE.fullmatch(body.strip()[1:-1]):
            return True
    return False


def build_units(repo: Path, config: Path, categories: Path | None):
    exact_assembly, intentional = load_categories(categories)
    recovered = load_recovered_full(config.parent)
    overlay = load_overlay_names(repo)
    assignments = load_group_assignments(repo)
    result = []
    for owner, address, size in parse_units(config):
        source = repo / "src" / f"{owner}.c"
        if owner in exact_assembly or (
            not owner.startswith("assembly/") and source.is_file()
            and not non_label_asm(source)
        ):
            category = "exact"
        elif owner in intentional:
            category = "asm"
        else:
            category = "pending"
        result.append(
            {
                "owner": owner,
                "address": address,
                "size": size,
                "category": category,
                "logical_group": group_for_owner(owner, assignments),
                "display": assignments.get(owner.removeprefix("assembly/"), {}).get(
                    "proposed_name"
                )
                or display_for(owner, source, recovered)
                or (demangle_cfront(overlay[address]) if address in overlay else None),
            }
        )
    return result


# --------------------------------------------------------------------------
# C_FUZZY: byte-weighted similarity over the C_EXACT denominator
# --------------------------------------------------------------------------
def fuzzy_progress(units, scores) -> float:
    """Byte-weighted mean similarity, in percent, of the recoverable units.

    C_EXACT counts a unit 0 or 100; C_FUZZY replaces the pending units with
    their measured ``.text`` similarity, weighted by unit bytes.  Intentional
    asm stays out of numerator and denominator, exactly like C_EXACT, and
    unmeasured pending units contribute 0.
    """
    recoverable = sum(unit["size"] for unit in units if unit["category"] != "asm")
    if not recoverable:
        return 0.0
    similar = 0.0
    for unit in units:
        if unit["category"] == "exact":
            similarity = 100.0
        elif unit["category"] == "pending":
            score = unit.get("score")
            if score is None:
                score = scores.get(unit["owner"], 0.0)
            similarity = min(100.0, max(0.0, float(score)))
        else:
            continue
        similar += unit["size"] * similarity
    return similar / recoverable


def measure_scores(workspace: Path, scores_out: Path) -> tuple[dict[str, float], Path] | None:
    """Per-unit similarity from the work-list scorer plus its persisted index.

    Delegates to ``scripts/list-functions.py --score`` so C_FUZZY reuses the
    same objdiff measurement as the contribution tooling; ``--out`` makes that
    same pass write the ``rnc-pending-similarity-v1`` index next to the
    measured objects.
    """
    script = Path(__file__).resolve().parent / "list-functions.py"
    process = subprocess.run(
        [
            sys.executable,
            str(script),
            "--score",
            "--limit",
            "0",
            "--json",
            "--out",
            str(scores_out),
            "--workspace",
            str(workspace),
        ],
        stdout=subprocess.PIPE,
        text=True,
    )
    if process.returncode != 0:
        print("scripts/list-functions.py --score failed", file=sys.stderr)
        return None
    try:
        payload = json.loads(process.stdout)
    except json.JSONDecodeError:
        print("list-functions.py --score returned no usable JSON", file=sys.stderr)
        return None
    scores = {
        str(item["unit"]): float(item["score"])
        for item in payload
        if item.get("unit") and item.get("score") is not None
    }
    return scores, Path(scores_out)


# --------------------------------------------------------------------------
# SVG
# --------------------------------------------------------------------------
def esc(value) -> str:
    return html.escape(str(value), quote=True)


def base_name(tile) -> str:
    if tile.get("small_bucket"):
        return "small logical groups"
    if tile.get("group"):
        return tile["logical_group"]
    return tile["owner"].rsplit("/", 1)[-1]


def tile_exact_percent(tile) -> float:
    """Return C_EXACT coverage; fuzzy similarity must not affect tile color."""
    if tile["category"] == "exact":
        return 100.0
    return min(100.0, max(0.0, float(tile.get("exact_percent") or 0.0)))


def tile_fill(tile) -> str:
    """Use Ratchet orange only for exact groups and copper for partial C."""
    category = tile["category"]
    if category == "asm":
        return CHROME
    if category == "exact":
        return ORANGE
    progress = tile_exact_percent(tile) if tile.get("group") else 0.0
    if progress <= 0.0:
        return PLATE

    # Keep low exact coverage close to the gray plate, while making mid-range
    # exact coverage visibly copper. Bolt orange remains exclusive to 100%.
    weight = math.pow(progress / 100.0, 1.5)
    start = tuple(int(PLATE[index : index + 2], 16) for index in (1, 3, 5))
    end = tuple(int(PARTIAL_ORANGE[index : index + 2], 16) for index in (1, 3, 5))
    return "#" + "".join(
        f"{round(left + (right - left) * weight):02x}"
        for left, right in zip(start, end)
    )


def contrast_ratio(first: str, second: str) -> float:
    def luminance(color: str) -> float:
        channels = [int(color[index : index + 2], 16) / 255 for index in (1, 3, 5)]
        linear = [
            value / 12.92 if value <= 0.04045 else ((value + 0.055) / 1.055) ** 2.4
            for value in channels
        ]
        return 0.2126 * linear[0] + 0.7152 * linear[1] + 0.0722 * linear[2]

    light, dark = sorted((luminance(first), luminance(second)), reverse=True)
    return (light + 0.05) / (dark + 0.05)


def tile_label_fills(tile, fill: str) -> tuple[str, str]:
    if tile["category"] in ("exact", "asm"):
        return LABEL_FILLS[tile["category"]]
    # Copper tiles need dark ink; near-zero progress tiles need pale ink.
    name_inks = (TEXT, "#2b1604")
    detail_inks = (MUTED, TEXT, "#2b1604")
    return (
        max(name_inks, key=lambda color: contrast_ratio(color, fill)),
        max(detail_inks, key=lambda color: contrast_ratio(color, fill)),
    )


def tile_detail(tile) -> str:
    count = tile.get("count")
    count_text = f"{int(count)} fn" if count is not None else "? fn"
    exact = tile.get("exact_percent")
    if exact is None and tile.get("category") == "exact":
        exact = 100.0
    fuzzy = tile.get("fuzzy_percent")
    exact_text = f"{float(exact):.1f}%" if exact is not None else "?%"
    fuzzy_text = f"{float(fuzzy):.1f}%" if fuzzy is not None else "?%"
    return f"{count_text} · {exact_text} exact · {fuzzy_text} fuzzy"


def draw_tile_label(lines, tile, x, y, dx, dy) -> None:
    """Draw the group name and the same exact/fuzzy summary on every tile."""
    name = tile.get("display") or base_name(tile)
    short = tile.get("short_display", name)
    names = list(dict.fromkeys(value for value in (name, short) if value))
    detail = tile_detail(tile)
    name_fill, detail_fill = tile_label_fills(tile, tile_fill(tile))

    def fits(text: str, size: float) -> bool:
        return len(text) * size * 0.56 <= dx - 8

    # Keep the full three-part format whenever both lines fit. No status or
    # percentage fragment is substituted when a tile is too narrow.
    for size in (11, 10, 9):
        if dy < size * 3.3:
            continue
        for label in names:
            if not fits(label, size) or not fits(detail, size - 1.5):
                continue
            lines.append(
                f'<text x="{x + 4:.2f}" y="{y + size + 3:.2f}" '
                f'font-family="{esc(FONT)}" font-size="{size}" fill="{name_fill}">'
                f"{esc(label)}</text>"
            )
            lines.append(
                f'<text x="{x + 4:.2f}" y="{y + size + 15:.2f}" '
                f'font-family="{esc(FONT)}" font-size="{size - 1.5}" fill="{detail_fill}">'
                f"{esc(detail)}</text>"
            )
            return

    # Tiny tiles may retain a readable name, but the detail line is hidden as
    # a whole instead of being shortened or losing one of its values. Center
    # labels near the top only in narrow tiles; wider groups keep a left edge.
    centered = dx < 64
    sizes = (10, 9, 8, 7) if centered else (11, 10, 9, 8, 7)
    for size in sizes:
        if dy < size + 6:
            continue
        for label in names:
            if centered:
                if len(label) * size * 0.6 > dx - 4:
                    continue
                text_x = x + dx / 2
                anchor = ' text-anchor="middle"'
            else:
                if not fits(label, size):
                    continue
                text_x = x + 4
                anchor = ""
            lines.append(
                f'<text x="{text_x:.2f}" y="{y + size + 3:.2f}"{anchor} '
                f'font-family="{esc(FONT)}" font-size="{size}" fill="{name_fill}">'
                f"{esc(label)}</text>"
            )
            return

def summarize_group(category: str, logical_group: str, members: list[dict]) -> dict:
    total_bytes = sum(unit["size"] for unit in members)
    exact = [unit for unit in members if unit["category"] == "exact"]
    pending = [unit for unit in members if unit["category"] == "pending"]
    asm = [unit for unit in members if unit["category"] == "asm"]
    exact_bytes = sum(unit["size"] for unit in exact)
    asm_bytes = sum(unit["size"] for unit in asm)
    recoverable = total_bytes - asm_bytes
    fuzzy_bytes = sum(unit["size"] * 100.0 for unit in exact)
    fuzzy_bytes += sum(
        unit["size"] * min(100.0, max(0.0, float(unit.get("score") or 0.0)))
        for unit in pending
    )
    has_pending_scores = any(unit.get("score") is not None for unit in pending)
    fuzzy_percent = None
    if recoverable and not pending:
        fuzzy_percent = 100.0
    elif recoverable and has_pending_scores:
        fuzzy_percent = fuzzy_bytes / recoverable
    if not recoverable:
        tile_category = "asm"
    elif exact_bytes == recoverable:
        tile_category = "exact"
    else:
        tile_category = "pending"
    return {
        "owner": report_group_name(category, logical_group),
        "logical_group": logical_group,
        "category": tile_category,
        "report_category": category,
        "address": min(unit["address"] for unit in members),
        "size": total_bytes,
        "group": True,
        "count": len(members),
        "matching_c": len(exact),
        "pending_c": len(pending),
        "intentional_asm": len(asm),
        "bytes_matching_c": exact_bytes,
        "bytes_intentional_asm": asm_bytes,
        "bytes_pending_c": sum(unit["size"] for unit in pending),
        "recoverable_bytes": recoverable,
        "exact_percent": 100.0 * exact_bytes / recoverable if recoverable else None,
        "fuzzy_percent": fuzzy_percent,
        "members": members,
    }


def group_display_name(category: str, logical_group: str) -> str:
    parts = logical_group.split("/")
    if category == "sdk" and parts[:1] == ["sdk"]:
        parts = parts[1:]
    if not parts:
        parts = ["general"]
    if parts == ["unclassified"]:
        return "Unclassified functions"
    label = " / ".join(format_group_part(part) for part in parts)
    return f"SDK / {label}" if category == "sdk" else label


def format_group_part(part: str) -> str:
    label = part.replace("_", " ").replace("-", " ")
    if label.casefold() == "ui":
        return "UI"
    return label.title()


def group_short_name(category: str, logical_group: str) -> str:
    part = logical_group.rsplit("/", 1)[-1]
    if part == "unclassified":
        return "Unclassified"
    return format_group_part(part)


def assign_unique_short_names(tiles: list[dict]) -> None:
    """Use the shortest unique path suffix when a full tile label does not fit."""
    candidates = []
    owners: dict[str, set[int]] = {}
    for index, tile in enumerate(tiles):
        if tile.get("small_bucket"):
            names = [tile.get("short_display", ""), tile.get("display", "")]
        else:
            parts = (tile.get("display") or tile["logical_group"]).split(" / ")
            names = [" / ".join(parts[-length:]) for length in range(1, len(parts) + 1)]
        candidates.append(names)
        for name in names:
            owners.setdefault(name, set()).add(index)

    for index, tile in enumerate(tiles):
        tile["short_display"] = next(
            (name for name in candidates[index] if owners[name] == {index}),
            tile.get("display") or tile["logical_group"],
        )


def build_group_tiles(units: list[dict]) -> list[dict]:
    grouped: dict[tuple[str, str], list[dict]] = {}
    for unit in units:
        key = (report_category_for_owner(unit["owner"]), unit["logical_group"])
        grouped.setdefault(key, []).append(unit)
    tiles = [summarize_group(category, name, members)
             for (category, name), members in grouped.items()]
    for tile in tiles:
        tile["display"] = group_display_name(
            tile["report_category"], tile["logical_group"]
        )
        tile["short_display"] = group_short_name(
            tile["report_category"], tile["logical_group"]
        )
    return sorted(tiles, key=lambda tile: (-tile["size"], tile["address"], tile["owner"]))


def render_svg(
    units,
    *,
    width,
    height,
    margin,
    header,
    footer,
    min_bytes,
    title,
    fuzzy_percent=None,
) -> str:
    # Chrome (margins, header, footer, legend) scales with the canvas, while
    # tile label fonts stay at a fixed readable size so a larger map fits
    # names on many more tiles.
    scale = max(1.0, min(width / 800.0, height / 400.0))
    # Keep the chrome compact on large canvases so tiles stay as big as
    # possible and more unit names fit.
    chrome_scale = 1.0 + (scale - 1.0) * 0.1
    text_scale = chrome_scale
    margin_px = round(margin * chrome_scale)
    header_px = round(header * chrome_scale)
    footer_px = round(footer * chrome_scale)
    map_x, map_y = margin_px, header_px
    map_dx = width - 2 * margin_px
    map_dy = height - header_px - footer_px - margin_px

    group_tiles = build_group_tiles(units)
    tiles = [tile for tile in group_tiles if tile["size"] >= min_bytes]
    small_groups = [tile for tile in group_tiles if tile["size"] < min_bytes]
    for report_category in ("game", "sdk"):
        for status in ("exact", "asm", "pending"):
            selected = [
                tile for tile in small_groups
                if tile["report_category"] == report_category
                and tile["category"] == status
            ]
            if not selected:
                continue
            members = [unit for tile in selected for unit in tile["members"]]
            bucket_name = f"small_{status}_groups"
            tile = summarize_group(report_category, bucket_name, members)
            tile["owner"] = report_group_name(report_category, bucket_name)
            tile["logical_group"] = bucket_name
            tile["display"] = group_display_name(report_category, bucket_name)
            tile["short_display"] = f"{len(selected)} small groups"
            tile["small_bucket"] = True
            tile["group_count"] = len(selected)
            tile["tree_group_count"] = len(selected)
            tile["threshold"] = min_bytes
            tiles.append(tile)

    assign_unique_short_names(tiles)

    total_units = len(units)
    total_bytes = sum(unit["size"] for unit in units)
    exact = [unit for unit in units if unit["category"] == "exact"]
    asm = [unit for unit in units if unit["category"] == "asm"]
    pending = [unit for unit in units if unit["category"] == "pending"]
    exact_bytes = sum(unit["size"] for unit in exact)
    asm_bytes = sum(unit["size"] for unit in asm)
    pending_bytes = total_bytes - exact_bytes - asm_bytes
    recoverable = total_bytes - asm_bytes
    exact_percent = (100.0 * exact_bytes / total_bytes) if total_bytes else 0.0
    recoverable_percent = (100.0 * exact_bytes / recoverable) if recoverable else 0.0

    # Cap only the visual weight of Unclassified. Its byte counts, tooltip
    # statistics, and global progress remain based on the full configured size.
    for tile in tiles:
        layout_size = tile["size"]
        if (
            tile.get("report_category") == "game"
            and tile.get("logical_group") == "unclassified"
        ):
            layout_size = min(layout_size, UNCLASSIFIED_LAYOUT_CAP_BYTES)
        tile["layout_size"] = layout_size

    # Keep SDK compact in a short top band and let Game use the remaining
    # area. Each band is split into equal-width panes to cap every tile at half
    # the map width while preserving proportional layout within each pane.
    sdk_tiles = [tile for tile in tiles if tile["report_category"] == "sdk"]
    game_tiles = [tile for tile in tiles if tile["report_category"] == "game"]
    sdk_height = map_dy * 0.25 if sdk_tiles and game_tiles else (map_dy if sdk_tiles else 0.0)
    game_y = map_y + sdk_height
    game_height = map_dy - sdk_height
    half_width = map_dx / 2.0
    placements = []

    def place_category(category_tiles, y, band_height):
        ordered = sorted(
            category_tiles,
            key=lambda tile: (-tile["layout_size"], tile["address"], tile["owner"]),
        )
        left_tiles, right_tiles = balanced_columns(ordered)
        for column_tiles, column_x in (
            (left_tiles, map_x),
            (right_tiles, map_x + half_width),
        ):
            if not column_tiles:
                continue
            rects = treemap(
                [tile["layout_size"] for tile in column_tiles],
                column_x,
                y,
                half_width,
                band_height,
            )
            placements.extend(zip(column_tiles, rects))

    place_category(sdk_tiles, map_y, sdk_height)
    place_category(game_tiles, game_y, game_height)

    lines = [
        '<?xml version="1.0" encoding="UTF-8"?>',
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" '
        f'viewBox="0 0 {width} {height}" role="img" '
        f'aria-label="Decompilation progress treemap of the boot ELF (SCUS_971.99)">',
        f"<title>{esc(title)}</title>",
        f'<rect width="{width}" height="{height}" fill="{BACKGROUND}"/>',
        '<defs><linearGradient id="progress-range" x1="0" y1="0" x2="1" y2="0">'
        f'<stop offset="0" stop-color="{PLATE}"/>'
        f'<stop offset="1" stop-color="{PARTIAL_ORANGE}"/>'
        '</linearGradient><linearGradient id="sheen" x1="0" y1="0" x2="0" y2="1">'
        '<stop offset="0" stop-color="#ffffff" stop-opacity="0.05"/>'
        '<stop offset="0.45" stop-color="#ffffff" stop-opacity="0"/>'
        '<stop offset="1" stop-color="#000000" stop-opacity="0.16"/>'
        "</linearGradient></defs>",
    ]

    # Header: bolt mark and two-tone title on the left, legend on the right.
    head, separator, tail = title.partition(" - ")
    ratchet, ampersand, clank = head.partition(" & ")
    title_spans = f'<tspan fill="{ORANGE}">{esc(ratchet)}</tspan>'
    if ampersand:
        title_spans += f'<tspan fill="{CHROME}"> &amp; {esc(clank)}</tspan>'
    if separator:
        title_spans += f'<tspan fill="{MUTED}" font-weight="500"> - {esc(tail)}</tspan>'
    mark = round(10 * text_scale)
    lines.append(
        f'<path d="{BOLT_PATH}" fill="{ORANGE}" fill-rule="evenodd" '
        f'transform="translate({margin_px} {round(24 * scale) - mark}) scale({mark / 12:.3f})"/>'
    )
    lines.append(
        f'<text x="{margin_px + round(16 * text_scale)}" y="{round(24 * scale)}" '
        f'font-family="{esc(FONT)}" font-size="{14 * text_scale:.1f}" '
        f'font-weight="700" letter-spacing="0.2">{title_spans}</text>'
    )
    lines.append(
        f'<text x="{margin_px}" y="{round(41 * scale)}" font-family="{esc(FONT)}" '
        f'font-size="{10 * text_scale:.1f}" '
        f'fill="{MUTED}">{len(exact)} matching C &#183; {len(asm)} intentional asm '
        f"&#183; {len(pending)} pending</text>"
    )
    legend_font = 10 * text_scale
    swatch = round(10 * scale)
    gap = round(15 * scale)
    legend_rows = (
        (ORANGE, f"matching C &#183; {exact_bytes:,} B"),
        (CHROME, f"intentional asm &#183; {asm_bytes:,} B"),
        ("url(#progress-range)", f"pending C &#183; {pending_bytes:,} B"),
    )
    # All rows share a left edge and hug the right edge as one block.
    # 0.56em/char is the same width estimate the tile labels use.
    widest = max(
        len(html.unescape(text)) * legend_font * 0.56 for _, text in legend_rows
    )
    row_x = width - margin_px - round(widest) - gap
    for offset, (color, text) in enumerate(legend_rows):
        y = round(14 * scale) + offset * round(17 * scale)
        title = (
            '<title>Pending C: dark plate to copper shows C_EXACT coverage; '
            'bolt orange marks 100% exact.</title>'
            if color == "url(#progress-range)"
            else ""
        )
        lines.append(
            f'<rect x="{row_x}" y="{y}" width="{swatch}" height="{swatch}" rx="2" '
            f'fill="{color}">{title}</rect>'
        )
        lines.append(
            f'<text x="{row_x + gap}" y="{y + round(9 * scale)}" '
            f'font-family="{esc(FONT)}" font-size="{legend_font:.1f}" fill="{MUTED}">{text}</text>'
        )

    # Hero stat: the goal-tracking percentage, HUD-style.
    status_x = row_x - round(10 * chrome_scale)
    lines.append(
        f'<text x="{status_x}" y="{round(38 * scale)}" text-anchor="end" '
        f'font-family="{esc(FONT)}" font-size="{28 * text_scale:.1f}" font-weight="800" '
        f'fill="{ORANGE}">{recoverable_percent:.1f}%</text>'
    )
    lines.append(
        f'<text x="{status_x}" y="{round(52 * scale)}" text-anchor="end" '
        f'font-family="{esc(FONT)}" font-size="{9 * text_scale:.1f}" fill="{MUTED}">'
        f"of recoverable C decompiled</text>"
    )

    placed = []
    for tile, rect in placements:
        x, y = rect["x"], rect["y"]
        dx, dy = max(rect["dx"], 0.0), max(rect["dy"], 0.0)
        if dx <= 0 or dy <= 0:
            continue
        fill = tile_fill(tile)
        dash = ' stroke-dasharray="3 2"' if tile.get("group") else ""
        lines.append(
            f'<rect x="{x:.2f}" y="{y:.2f}" width="{dx:.2f}" height="{dy:.2f}" rx="1" '
            f'fill="{fill}" stroke="{STROKE}" stroke-width="0.6"{dash} '
            f'shape-rendering="geometricPrecision">'
            f"<title>{esc(tile_tooltip(tile))}</title></rect>"
        )
        placed.append((tile, x, y, dx, dy))

    # One soft light across the whole plate: a single gradient reads as depth,
    # where a gradient inside every tile reads as noise.
    lines.append(
        f'<rect x="{map_x}" y="{map_y}" width="{map_dx}" height="{map_dy}" fill="url(#sheen)"/>'
    )
    for tile, x, y, dx, dy in placed:
        draw_tile_label(lines, tile, x, y, dx, dy)

    generated = dt.datetime.now(dt.timezone.utc).strftime("%Y-%m-%d")
    separator_y = height - footer_px
    lines.append(
        f'<line x1="{margin_px}" y1="{separator_y:.1f}" x2="{width - margin_px}" '
        f'y2="{separator_y:.1f}" stroke="{PLATE}" stroke-width="1" opacity="0.8"/>'
    )
    if fuzzy_percent is None:
        progress_text = f"C_EXACT {recoverable_percent:.1f}% of recoverable C"
    else:
        progress_text = (
            f"C_EXACT {recoverable_percent:.1f}% / C_FUZZY {fuzzy_percent:.1f}% "
            "of recoverable C"
        )
    lines.append(
        f'<text x="{margin_px}" y="{height - round(20 * scale)}" '
        f'font-family="{esc(FONT)}" font-size="{9 * text_scale:.1f}" fill="{MUTED}" opacity="0.85">'
        f"{exact_bytes:,} of {total_bytes:,} configured bytes are matching C &#183; "
        f"{exact_percent:.1f}% of all configured code</text>"
    )
    lines.append(
        f'<text x="{margin_px}" y="{height - round(8 * scale)}" '
        f'font-family="{esc(FONT)}" font-size="{9 * text_scale:.1f}" fill="{MUTED}" opacity="0.85">'
        f"{progress_text}</text>"
    )
    lines.append(
        f'<text x="{width - margin_px}" y="{height - round(8 * scale)}" text-anchor="end" '
        f'font-family="{esc(FONT)}" font-size="{9 * text_scale:.1f}" fill="{MUTED}" opacity="0.85">'
        f"generated {generated} &#183; scripts/generate_treemap.py</text>"
    )
    lines.append("</svg>")
    return "\n".join(lines) + "\n"


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--repo", type=Path, default=Path(__file__).resolve().parents[1]
    )
    parser.add_argument(
        "--config",
        type=Path,
        help="linker config (default: <repo>/config/us/rnc1.us.yaml)",
    )
    parser.add_argument(
        "--categories",
        type=Path,
        help="derived categories JSON (default: <repo>/config/us/unit_categories.json)",
    )
    parser.add_argument(
        "--output", type=Path, help="SVG path (default: <repo>/assets/decomp_map.svg)"
    )
    parser.add_argument("--width", type=int, default=800)
    parser.add_argument("--height", type=int, default=1600)
    parser.add_argument("--margin", type=int, default=10)
    parser.add_argument(
        "--header",
        type=int,
        default=62,
        help="header band above the map; must fit the three legend rows",
    )
    parser.add_argument(
        "--footer",
        type=int,
        default=34,
        help="footer band below the map; fits the two footer lines",
    )
    parser.add_argument(
        "--min-bytes",
        type=int,
        default=0,
        help="groups below this size are combined per status class; 0 (default) "
        "draws every logical group",
    )
    parser.add_argument("--title", default="Ratchet & Clank - boot ELF (SCUS_971.99) decompilation progress")
    parser.add_argument(
        "--no-scores",
        action="store_true",
        help="skip workspace and committed similarity scores; omit C_FUZZY values",
    )
    parser.add_argument(
        "--workspace",
        type=Path,
        help="baseline workspace; measure the pending C bodies with "
        "scripts/list-functions.py --score and also report C_FUZZY",
    )
    parser.add_argument(
        "--scores-out",
        type=Path,
        default=None,
        help="rnc-pending-similarity-v1 index path for --workspace "
        "(default: WORKSPACE/c_fuzzy_scores.json)",
    )
    args = parser.parse_args(argv)

    repo = args.repo.resolve()
    config = (args.config or repo / "config/us/rnc1.us.yaml").resolve()
    if not config.is_file():
        print(f"error: linker config not found: {config}", file=sys.stderr)
        return 2
    categories = (args.categories or repo / "config/us/unit_categories.json").resolve()
    output = (args.output or repo / "assets" / "decomp_map.svg").resolve()

    units = build_units(repo, config, categories)
    if not units:
        print("error: no configured C units found", file=sys.stderr)
        return 1

    fuzzy_percent = None
    scores: dict[object, float] = {}
    index_path = None
    workspace = args.workspace
    if workspace is not None:
        workspace = workspace.expanduser().resolve()
        scores_out = (
            args.scores_out.expanduser().resolve()
            if args.scores_out
            else workspace / "c_fuzzy_scores.json"
        )
        measured = measure_scores(workspace, scores_out)
        if measured is None:
            return 2
        scores, index_path = measured
    elif not args.no_scores:
        scores = committed_function_scores(repo / "progress" / "report.json")

    for unit in units:
        score = scores.get(unit["owner"])
        if score is None:
            score = scores.get(unit["owner"].removeprefix("assembly/"))
        if score is None:
            score = scores.get(unit["address"])
        if score is not None:
            unit["score"] = float(score)
    if scores:
        fuzzy_percent = fuzzy_progress(units, scores)

    svg = render_svg(
        units,
        width=args.width,
        height=args.height,
        margin=args.margin,
        header=args.header,
        footer=args.footer,
        min_bytes=args.min_bytes,
        title=args.title,
        fuzzy_percent=fuzzy_percent,
    )
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(svg)

    group_tiles = build_group_tiles(units)
    total = sum(unit["size"] for unit in units)
    exact = [unit for unit in units if unit["category"] == "exact"]
    asm = [unit for unit in units if unit["category"] == "asm"]
    pending = [unit for unit in units if unit["category"] == "pending"]
    exact_bytes = sum(unit["size"] for unit in exact)
    asm_bytes = sum(unit["size"] for unit in asm)
    # Single source of truth for public progress: the workbench status bar and
    # any other surface reads this file, so the SVG footer and the status values
    # can never drift apart.
    stats_path = output.with_suffix(".json")
    recoverable = total - asm_bytes
    stats = {
        "schema": "rnc-public-progress-v2",
        "source": "scripts/generate_treemap.py",
        "units_total": len(units),
        "groups_total": len(group_tiles),
        "matching_c": len(exact),
        "intentional_asm": len(asm),
        "pending_c": len(pending),
        "bytes_total": total,
        "bytes_matching_c": exact_bytes,
        "bytes_intentional_asm": asm_bytes,
        "bytes_pending_c": recoverable - exact_bytes,
        "c_exact_percent_of_recoverable": round(100.0 * exact_bytes / recoverable, 4) if recoverable else 0.0,
        "c_fuzzy_percent_of_recoverable": round(fuzzy_percent, 4) if fuzzy_percent is not None else None,
        "groups": [
            {
                "name": report_group_name(group["report_category"], group["logical_group"]),
                "logical_group": group["logical_group"],
                "category": group["report_category"],
                "functions": group["count"],
                "matching_c": group["matching_c"],
                "pending_c": group["pending_c"],
                "intentional_asm": group["intentional_asm"],
                "bytes_total": group["size"],
                "bytes_matching_c": group["bytes_matching_c"],
                "bytes_pending_c": group["bytes_pending_c"],
                "bytes_intentional_asm": group["bytes_intentional_asm"],
                "c_exact_percent_of_recoverable": round(group["exact_percent"], 4)
                if group["exact_percent"] is not None else None,
                "c_fuzzy_percent_of_recoverable": round(group["fuzzy_percent"], 4)
                if group["fuzzy_percent"] is not None else None,
            }
            for group in group_tiles
        ],
    }
    stats_path.write_text(json.dumps(stats, indent=2) + "\n")
    print(f"  stats: {stats_path}")
    shown = sum(1 for group in group_tiles if group["size"] >= args.min_bytes)
    small_buckets = sum(
        1
        for category in ("game", "sdk")
        for status in ("exact", "asm", "pending")
        if any(
            group["size"] < args.min_bytes
            and group["report_category"] == category
            and group["category"] == status
            for group in group_tiles
        )
    )
    print(f"wrote {output}")
    print(
        f"  matching C: {len(exact)}/{len(units)} units, {exact_bytes:,}/{total:,} bytes "
        f"({100.0 * exact_bytes / total:.2f}%; "
        f"{100.0 * exact_bytes / (total - asm_bytes):.2f}% of recoverable C)"
    )
    if fuzzy_percent is not None:
        measured = sum(1 for unit in pending if unit.get("score") is not None)
        print(
            f"  fuzzy C (C_FUZZY): {fuzzy_percent:.2f}% of recoverable C "
            f"({measured}/{len(pending)} pending units measured)"
        )
        if index_path is not None:
            print(f"  similarity index: {index_path}")
    print(f"  intentional asm: {len(asm)} units, {asm_bytes:,} B")
    print(f"  pending C: {len(pending)} units, {total - exact_bytes - asm_bytes:,} B")
    print(
        f"  tiles: {shown} logical groups + {small_buckets} small-status groups"
        if args.min_bytes > 0
        else f"  tiles: {shown} logical groups"
    )
    print(
        f"  layout: {'squarify' if _squarify_pkg is not None else 'bundled fallback'}"
    )
    if categories.is_file():
        print(
            f"  categories: {categories.relative_to(repo) if categories.is_relative_to(repo) else categories}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
