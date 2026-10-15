#!/usr/bin/env python3
"""Progress report for recomp.fyi, in objdiff's report format (version 2).

recomp.fyi reads ``report.json`` from the CI artifact ``SCUS_971.99_report``
on the default branch. The report is not committed: every number but one is
derived from the tree (linker config, ``src/``, overlay tables), so the
``progress`` workflow regenerates it on every push and pull request, publishes
it with the progress map on the ``progress`` branch and comments the change
on pull requests. The exception is C_FUZZY, the measured similarity of pending
functions, which needs a build: the maintainers' tooling measures it with
``--workspace`` and keeps ``fuzzy_scores.json``, which the workflow copies to
``build/progress/fuzzy_scores.json`` and publishes next to the report.

What the report counts (the same contract as ``assets/decomp_map.json``):

* every recoverable configured C unit (``config/us/rnc1.us.yaml`` rows
  ``[0xADDR, c, owner]``) with its byte size, nested under a logical group;
* every shared and level function of the 19 level overlays
  (``config/overlays/us/functions.tsv``, ``docs/overlays.md``), once each,
  grouped by its ``src/overlays/`` file; it is matched when its C is there
  (promotion follows the byte proof) and pending while it is an
  ``INCLUDE_ASM`` stub. The top-level measures cover the executable and the
  overlays together; the categories keep them apart: ``boot`` (= ``game`` +
  ``sdk``), ``overlays`` (= ``shared`` + ``levels``), ``level_NN``;
* a function is matched only when it is C_EXACT: a promoted source outside
  ``src/assembly/`` with no inline asm but name labels (``non_label_asm``), or
  a legacy exact unit in ``config/us/unit_categories.json``.
  Assembly-backed units build from their retail oracle, so the ordinary
  objdiff report of the baseline shows them as 100 %; this report does not;
* intentional low-level asm units are left out, as in the C_EXACT metric;
* pending functions carry their measured ``.text`` similarity as the fuzzy score;
* data (``total_data``/``matched_data``): the executable's preserved data
  (``core_data`` .. ``lvl_sndvtbl``) and each level's ``.lit``, ``.data`` and
  vtables; matched are the bytes C defines and the proofs check: the
  ``*_OVERLAYS`` entries of ``configure.py`` (the boot gate) and
  ``config/overlays/us/data.tsv`` (``make overlays``). bss is not counted.
* report units are grouped by the logical subsystem in
  ``recovered_names.json``; a clicked group shows its member functions;
* only entries with status ``proposed`` replace the function's current symbol
  with a semantic name. Grouping hints are independent of name status, and
  conservative fallback groups cover units absent from the proposal catalog.

The file holds group and function names, addresses, sizes and percentages only.
Source paths are omitted because a logical group can span multiple source
files. No retail bytes are included.

Usage::

    python3 scripts/gen_progress_report.py
        write build/progress/report.json from the repository, reusing
        build/progress/fuzzy_scores.json; needs no toolchain or executable
    python3 scripts/gen_progress_report.py --workspace build/baseline
        after ./verify-baseline.sh: measure pending units and also write
        build/progress/fuzzy_scores.json (the tooling; `make progress`)
"""

from __future__ import annotations

import argparse
import ast
import json
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from rnc_units import INCLUDE_ASM_RE, classify_units, parse_config_rows, source_symbol  # noqa: E402
from overlay_units import category_of, group_of, load_levels, overlay_functions  # noqa: E402
from progress_groups import (  # noqa: E402
    canonical_owner,
    write_fuzzy_scores,
    committed_function_scores,
    group_for_owner,
    load_group_assignments,
    report_category_for_owner,
    report_group_name,
)

REPO = Path(__file__).resolve().parents[1]
REPORT = REPO / "build" / "progress" / "report.json"
SCORES = REPO / "build" / "progress" / "fuzzy_scores.json"
# A unit carries every category it belongs to (objdiff sums a category
# over the units that list it): executable units ``boot`` and ``game``/``sdk``,
# overlay functions ``overlays`` and ``shared`` or ``levels`` + ``level_NN``.
CATEGORIES = (("boot", "Boot executable"), ("game", "Game"), ("sdk", "SDK"),
              ("overlays", "Level overlays"), ("shared", "Shared level code"),
              ("levels", "Per-level code"))
LEVEL_CATEGORIES = tuple((f"level_{i:02d}", f"Level {i:02d}") for i in range(19))
# Data the report counts: the executable's preserved data blobs and each
# level's loaded data records (bss holds no bytes and is left out).
BOOT_DATA = ("core_data", "core_rdata", "core_lit", "lit", "data",
             "lvl_vtbl", "lvl_camvtbl", "lvl_sndvtbl")
LEVEL_DATA = (".lit", ".data", "lvl.vtbl", "lvl.camvtbl", "lvl.sndvtbl")
C_DATA_TABLES = ("RODATA_OVERLAYS", "SDATA_OVERLAYS", "DATA_OVERLAYS")


def unit_categories(function: dict) -> list[str]:
    c = function["category"]
    if c in ("game", "sdk"):
        return ["boot", c]
    if c == "shared":
        return ["overlays", "shared"]
    return ["overlays", "levels", c]


def unit_name(owner: str) -> str:
    """Stable name: promotion moves a unit out of ``assembly/``."""
    return owner.removeprefix("assembly/")


def unit_category(name: str) -> str:
    return report_category_for_owner(name)


def unit_symbol(unit: dict) -> str:
    source = unit["source"]
    if source.is_file():
        text = source.read_text(encoding="utf-8", errors="replace")
        if unit["category"] == "pending":
            match = INCLUDE_ASM_RE.search(text)
            if match:
                return match.group(1)
        symbol = source_symbol(source)
        if symbol:
            return symbol
    return unit_name(unit["owner"]).rsplit("/", 1)[-1]


def measure_pending(workspace: Path) -> dict[str, float]:
    """Pending-unit similarity from ``scripts/list-functions.py --score``."""
    process = subprocess.run(
        [sys.executable, str(REPO / "scripts" / "list-functions.py"), "--score",
         "--limit", "0", "--json", "--workspace", str(workspace)],
        stdout=subprocess.PIPE, text=True, cwd=REPO)
    if process.returncode != 0:
        raise SystemExit("scripts/list-functions.py --score failed")
    return {unit_name(str(item["unit"])): float(item["score"])
            for item in json.loads(process.stdout)
            if item.get("unit") and item.get("score") is not None}


def committed_scores(path: Path) -> dict[object, float]:
    return committed_function_scores(path)


def c_data_tables(repo: Path) -> dict[str, tuple]:
    """Every *_OVERLAYS entry of configure.py: unit suffix -> (VMA, offset, size)."""
    path = repo / "configure.py"
    entries: dict[str, tuple] = {}
    if not path.is_file():
        return entries
    for node in ast.parse(path.read_text()).body:
        if (isinstance(node, ast.Assign) and len(node.targets) == 1
                and getattr(node.targets[0], "id", None) in C_DATA_TABLES):
            for suffix, value in ast.literal_eval(node.value).items():
                entries[f"{node.targets[0].id}:{suffix}"] = value
    return entries


def data_totals(repo: Path) -> dict[str, tuple[int, int]]:
    """Category -> (total, matched) data bytes."""
    rows = parse_config_rows(repo / "config" / "us" / "rnc1.us.yaml")
    units = [owner for _, kind, owner in rows if kind == "c"]
    boot = sum(rows[i + 1][0] - offset for i, (offset, kind, name) in enumerate(rows[:-1])
               if kind == "databin" and name in BOOT_DATA)
    owned = sum(size for key, (_, _, size) in c_data_tables(repo).items()
                if any(unit == key.split(":", 1)[1] or unit.endswith("/" + key.split(":", 1)[1])
                       for unit in units))
    totals = {"boot": (boot, owned)}
    level_c = read_level_data(repo)
    for path in sorted((repo / "config" / "overlays" / "us").glob("level-??.json")):
        level = int(path.stem[-2:])
        sections = json.loads(path.read_text()).get("sections", [])
        total = sum(sec["size"] for sec in sections if sec["name"] in LEVEL_DATA)
        totals[f"level_{level:02d}"] = (total, level_c.get(level, 0))
    levels = [v for k, v in totals.items() if k.startswith("level_")]
    totals["levels"] = totals["overlays"] = (sum(t for t, _ in levels), sum(m for _, m in levels))
    totals["all"] = tuple(a + b for a, b in zip(totals["boot"], totals["overlays"]))
    return totals


def read_level_data(repo: Path) -> dict[int, int]:
    """Level -> bytes of its data defined in C (config/overlays/us/data.tsv)."""
    path = repo / "config" / "overlays" / "us" / "data.tsv"
    out: dict[int, int] = {}
    if path.is_file():
        for line in path.read_text().splitlines():
            m = re.match(r"^D_L(\d{2})_[0-9A-Fa-f]{8}\t(\d+)", line)
            if m:
                out[int(m.group(1))] = out.get(int(m.group(1)), 0) + int(m.group(2))
            m = re.match(r"^D_[0-9A-Fa-f]{8}\t(\d+)", line)
            if m:  # data every level holds at that address, proven in all of them
                for level in (int(p.stem[-2:]) for p in path.parent.glob("level-??.json")):
                    out[level] = out.get(level, 0) + int(m.group(1))
    return out


def zero_level_data(repo: Path) -> int:
    """Bytes of the level data in C that are zero (defined `= 0` or `= {0}`),
    counted per level like the matched bytes: a D_XXXXXXXX row counts once for
    every level."""
    zero = set()
    for path in (repo / "src" / "overlays").rglob("*.c"):
        for label, bare in re.findall(r'^(?!extern\b)[^;=\n]*?(?:__asm__\("(D_\w+)"\)|\b(D_(?:L\d\d_)?[0-9A-Fa-f]{8})\b)'
                                      r'[^;=\n]*=\s*(?:\{\s*0\s*\}|0)\s*;', path.read_text(errors="replace"), re.M):
            zero.add(label or bare)
    levels = len(list((repo / "config" / "overlays" / "us").glob("level-??.json")))
    rows = (repo / "config" / "overlays" / "us" / "data.tsv")
    total = 0
    for line in rows.read_text().splitlines() if rows.is_file() else []:
        name, _, size = line.partition("\t")
        if name in zero:
            total += int(size) * (1 if name.startswith("D_L") else levels)
    return total


def data_measures(total: int, matched: int) -> dict:
    pct = round(100.0 * matched / total, 6) if total else 0.0
    return {"total_data": str(total), "matched_data": str(matched), "matched_data_percent": pct,
            "complete_data": str(matched), "complete_data_percent": pct}


def measures(functions: list[dict], groups: list[dict]) -> dict:
    total = sum(function["size"] for function in functions)
    matched = sum(function["size"] for function in functions if function["exact"])
    fuzzy = sum(function["size"] * function["fuzzy"] for function in functions)
    pct = (lambda part: round(100.0 * part / total, 6)) if total else (lambda part: 0.0)
    matched_functions = sum(1 for function in functions if function["exact"])
    complete_groups = sum(
        1 for group in groups if all(function["exact"] for function in group["functions"])
    )
    return {
        "fuzzy_match_percent": round(fuzzy / total, 6) if total else 0.0,
        "total_code": str(total),
        "matched_code": str(matched),
        "matched_code_percent": pct(matched),
        "total_functions": len(functions),
        "matched_functions": matched_functions,
        "matched_functions_percent":
            round(100.0 * matched_functions / len(functions), 6) if functions else 0.0,
        "complete_code": str(matched),
        "complete_code_percent": pct(matched),
        "total_units": len(groups),
        "complete_units": complete_groups,
    }


# A function the unit's source defines, bound to its retail label.
LABEL_DEF_RE = re.compile(
    r'^(?!extern\b)[^\n;{}]*?\b(\w+)\s*\([^;{}]*\)\s*__asm__\("(?:FUN|func)_([0-9A-Fa-f]{8})"\)\s*;',
    re.M)
INCLUDE_ASM_LABEL_RE = re.compile(r'INCLUDE_ASM\([^\n]*\b(?:FUN|func)_([0-9A-Fa-f]{8})\)')


def main_vram_offset() -> int | None:
    """VMA minus file offset of the executable's main segment."""
    path = REPO / "config" / "us" / "rnc1.us.yaml"
    text = path.read_text() if path.is_file() else ""
    match = re.search(r"name: main\n\s+type: code\n\s+start: (0x[0-9a-fA-F]+)\n\s+vram: (0x[0-9a-fA-F]+)", text)
    return int(match.group(2), 16) - int(match.group(1), 16) if match else None


def unit_functions(unit: dict, vram_offset: int | None) -> list[tuple[int, int, str | None]]:
    """(offset, size, C name) of each function a unit holds.

    A unit holding several functions lists them by the retail labels its
    source binds them to; any other unit is one function."""
    whole = [(int(unit["address"]), int(unit["size"]), None)]
    source = unit["source"]
    if vram_offset is None or not source.is_file():
        return whole
    start, end = int(unit["address"]), int(unit["address"]) + int(unit["size"])
    text = source.read_text(encoding="utf-8", errors="replace")
    found: dict[int, str | None] = {}
    for match in LABEL_DEF_RE.finditer(text):
        found.setdefault(int(match.group(2), 16) - vram_offset, match.group(1))
    for match in INCLUDE_ASM_LABEL_RE.finditer(text):
        found.setdefault(int(match.group(1), 16) - vram_offset, None)
    offsets = sorted(offset for offset in found if start <= offset < end)
    if len(offsets) < 2 or offsets[0] != start:
        return whole
    bounds = offsets + [end]
    return [(offset, bounds[i + 1] - offset, found[offset]) for i, offset in enumerate(offsets)]


def recovered_by_address() -> dict[int, str]:
    """Proposed recovered name of each function address."""
    path = REPO / "config" / "us" / "recovered_names.json"
    payload = json.loads(path.read_text()) if path.is_file() else {}
    names: dict[int, str] = {}

    def walk(node: object) -> None:
        if isinstance(node, dict):
            address, name = node.get("address"), node.get("proposed_name")
            if isinstance(address, str) and isinstance(name, str) and "source_path" in node:
                names.setdefault(int(address, 16), name)
            for value in node.values():
                walk(value)
        elif isinstance(node, list):
            for value in node:
                walk(value)

    walk(payload)
    return names


def build_report(scores: dict[object, float]) -> dict:
    assignments = load_group_assignments(REPO)
    vram_offset = main_vram_offset()
    recovered = recovered_by_address()
    functions = []
    for unit in classify_units(REPO):
        if unit["category"] == "asm":
            continue
        canonical_name = unit_name(unit["owner"])
        address = int(unit["address"])
        owner = str(unit["owner"])
        assignment = assignments.get(canonical_owner(owner), {})
        proposed_name = assignment.get("proposed_name")
        exact = unit["category"] == "exact"
        score = scores.get(address, scores.get(canonical_name, scores.get(owner, 0.0)))
        fuzzy = 100.0 if exact else round(min(100.0, max(0.0, score)), 4)
        if not exact and fuzzy >= 100.0:
            fuzzy = 99.99  # only promoted C counts as matched
        parts = unit_functions(unit, vram_offset)
        for offset, size, c_name in parts:
            if len(parts) == 1:
                symbol = proposed_name or unit_symbol(unit)
            else:
                vma = offset + (vram_offset or 0)
                symbol = recovered.get(vma) or c_name or f"FUN_{vma:08x}"
            functions.append({
                "owner": canonical_name,
                "size": size,
                "address": offset,
                "exact": exact,
                "fuzzy": fuzzy,
                "category": unit_category(owner),
                "logical_group": group_for_owner(owner, assignments),
                "symbol": symbol,
            })
    functions.sort(key=lambda function: function["address"])

    levels = load_levels(REPO)
    for function in overlay_functions(REPO):
        if function["asm"]:
            continue
        category = category_of(function)
        functions.append({
            "owner": f"overlays/{function['name']}",
            "size": function["size"],
            "address": function["address"],
            "exact": function["exact"],
            "fuzzy": 100.0 if function["exact"] else 0.0,
            "category": category,
            "logical_group": group_of(function),
            "symbol": function["name"],
            "level_name": levels.get(function["level"], {}).get("planet", ""),
        })

    grouped: dict[tuple[str, str], list[dict]] = {}
    for function in functions:
        key = (function["category"], function["logical_group"])
        grouped.setdefault(key, []).append(function)

    report_groups = []
    for (category, logical_group), members in grouped.items():
        if category in ("game", "sdk"):
            name = report_group_name(category, logical_group)
        else:
            name = f"{category}/{logical_group}"
        report_groups.append({
            "name": name,
            "category": category,
            "logical_group": logical_group,
            "functions": members,
        })
    report_groups.sort(key=lambda group: (group["functions"][0]["address"], group["name"]))

    report_units = []
    for group in report_groups:
        members = group["functions"]
        report_units.append({
            "name": group["name"],
            "measures": measures(members, [group]),
            "functions": [{
                "name": function["symbol"],
                "size": str(function["size"]),
                "fuzzy_match_percent": function["fuzzy"],
                "address": str(function["address"] - members[0]["address"]),
                "metadata": {"virtual_address": str(function["address"])},
            } for function in members],
            "metadata": {
                "complete": all(function["exact"] for function in members),
                "progress_categories": unit_categories(members[0]),
            },
        })
    categories = list(CATEGORIES) + [
        (cid, cname) for cid, cname in LEVEL_CATEGORIES
        if any(function["category"] == cid for function in functions)
    ]
    data = data_totals(REPO)
    return {
        "measures": {**measures(functions, report_groups), **data_measures(*data["all"])},
        "units": report_units,
        "version": 2,
        "categories": [{"id": cid, "name": cname,
                        "measures": {**measures(
                            [function for function in functions if cid in unit_categories(function)],
                            [group for group in report_groups if cid in unit_categories(group["functions"][0])],
                        ), **(data_measures(*data[cid]) if cid in data else {})}}
                       for cid, cname in categories],
    }


def render(report: dict) -> str:
    return json.dumps(report, indent=2) + "\n"


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--workspace", type=Path,
                        help="baseline workspace to measure pending units in (local only); "
                             "also refreshes --scores")
    parser.add_argument("--report", type=Path, default=REPORT)
    parser.add_argument("--scores", type=Path, default=SCORES,
                        help="C_FUZZY scores of pending functions (default: %(default)s)")
    args = parser.parse_args(argv)

    scores = (measure_pending(args.workspace) if args.workspace
              else committed_scores(args.scores))
    report = build_report(scores)
    if args.workspace:
        count = write_fuzzy_scores(args.scores, report)
        print(f"wrote {display(args.scores)}: {count} measured functions")
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(render(report), encoding="utf-8")
    m = report["measures"]
    print(f"wrote {display(args.report)}: {m['matched_code']} / {m['total_code']} B "
          f"({m['matched_code_percent']:.2f} %), fuzzy {m['fuzzy_match_percent']:.2f} %, "
          f"{m['complete_units']} / {m['total_units']} groups, data {m['matched_data']} / "
          f"{m['total_data']} B ({m['matched_data_percent']:.2f} %)")
    return 0


def display(path: Path) -> str:
    try:
        return str(path.resolve().relative_to(REPO))
    except ValueError:
        return str(path)


if __name__ == "__main__":
    raise SystemExit(main())
