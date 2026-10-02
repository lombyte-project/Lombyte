#!/usr/bin/env python3
"""Pull request progress comment: compare two reports from gen_progress_report.py.

The ``progress`` workflow runs it with the report published for main (the
``progress`` branch) and the report built from the pull request, and posts the
Markdown as one comment that later runs update in place.

Usage::

    python3 scripts/progress_comment.py BASE_REPORT HEAD_REPORT > comment.md

A missing base report (first run) is treated as empty.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

MARKER = "<!-- lombyte-progress-report -->"
ROWS = (("total", "All code"), ("boot", "Boot executable"), ("game", "Game"),
        ("sdk", "SDK"), ("overlays", "Level overlays"),
        ("shared", "Shared level code"), ("levels", "Per-level code"))
LIST_LIMIT = 40


def load(path: Path) -> dict:
    if not path.is_file():
        return {}
    return json.loads(path.read_text(encoding="utf-8"))


def measures(report: dict) -> dict[str, dict]:
    result = {"total": report.get("measures", {})}
    for category in report.get("categories", []):
        result[category["id"]] = category.get("measures", {})
    return result


def functions(report: dict) -> dict[tuple[str, int], dict]:
    """Every function by report category and retail address (level overlays
    share address ranges), with its unit and match state."""
    result = {}
    for unit in report.get("units", []):
        for function in unit.get("functions") or []:
            address = int((function.get("metadata") or {}).get("virtual_address", -1))
            result[(unit["name"].split("/", 1)[0], address)] = {
                "name": function["name"],
                "unit": unit["name"],
                "size": int(function["size"]),
                "exact": float(function.get("fuzzy_match_percent", 0.0)) >= 100.0,
            }
    return result


def signed(value: float, unit: str, digits: int = 2) -> str:
    if round(value, digits) == 0:
        return "—"
    return f"{value:+,.{digits}f}{unit}" if digits else f"{value:+,d}{unit}"


def function_rows(items: list[dict]) -> list[str]:
    rows = ["| Function | Address | Size | Unit |", "| --- | --- | ---: | --- |"]
    for item in items[:LIST_LIMIT]:
        rows.append(f"| `{item['name']}` | `0x{item['address']:08x}` | {item['size']:,} B "
                    f"| `{item['unit']}` |")
    if len(items) > LIST_LIMIT:
        rows.append(f"| … and {len(items) - LIST_LIMIT} more | | | |")
    return rows


def render(base: dict, head: dict, base_label: str) -> str:
    old, new = measures(base), measures(head)
    lines = [MARKER, "### Decompilation progress", ""]
    lines += ["| | C_EXACT | Change | Matched bytes | Functions | C_FUZZY |",
              "| --- | ---: | ---: | ---: | ---: | ---: |"]
    for key, label in ROWS:
        if key not in new:
            continue
        n, o = new[key], old.get(key, {})
        exact = float(n.get("matched_code_percent", 0.0))
        delta = exact - float(o.get("matched_code_percent", 0.0)) if o else 0.0
        bytes_delta = int(n.get("matched_code", 0)) - int(o.get("matched_code", 0)) if o else 0
        funcs_delta = int(n.get("matched_functions", 0)) - int(o.get("matched_functions", 0)) if o else 0
        lines.append(
            f"| {label} | {exact:.2f} % | {signed(delta, ' pp')} "
            f"| {int(n.get('matched_code', 0)):,} / {int(n.get('total_code', 0)):,} B "
            f"({signed(bytes_delta, ' B', 0)}) "
            f"| {n.get('matched_functions', 0):,} / {n.get('total_functions', 0):,} "
            f"({signed(funcs_delta, '', 0)}) "
            f"| {float(n.get('fuzzy_match_percent', 0.0)):.2f} % |")
    lines.append("")

    before, after = (functions(base), functions(head)) if base else ({}, {})
    gained = sorted(({"address": a[1], **f} for a, f in after.items()
                     if f["exact"] and not before.get(a, {}).get("exact")),
                    key=lambda item: -item["size"])
    lost = sorted(({"address": a[1], **f} for a, f in before.items()
                   if f["exact"] and not after.get(a, {}).get("exact")),
                  key=lambda item: -item["size"])
    if not base:
        lines.append(f"No report for {base_label} yet, so there is nothing to compare against.")
    elif not gained and not lost:
        lines.append(f"No function changed its match state compared with {base_label}.")
    if gained:
        lines += ["", f"<details><summary>{len(gained)} newly matched function"
                  f"{'s' if len(gained) != 1 else ''} "
                  f"({sum(item['size'] for item in gained):,} B)</summary>", ""]
        lines += function_rows(gained) + ["", "</details>"]
    if lost:
        lines += ["", f"**{len(lost)} function{'s' if len(lost) != 1 else ''} no longer "
                  f"matched** ({sum(item['size'] for item in lost):,} B):", ""]
        lines += function_rows(lost)
    lines += ["", f"<sub>Compared with {base_label}. C_FUZZY uses "
              "main's last measurement.</sub>", ""]
    return "\n".join(lines)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("base", type=Path)
    parser.add_argument("head", type=Path)
    parser.add_argument("--base-label", default="main")
    args = parser.parse_args(argv)
    sys.stdout.write(render(load(args.base), load(args.head), args.base_label))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
