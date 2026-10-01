#!/usr/bin/env python3
"""Strip grouping-section attributes from pending C bodies (measurability fix).

Legacy seeds wrap each recovered function in
``__attribute__((section(".text.<name>")))``.  The retail unit object uses plain
``.text``, so objdiff cannot pair those functions and every such unit reports
0.0% no matter how close the C is.  This tool removes only that attribute from
the ``#else`` C body; the ``#ifndef NON_MATCHING`` oracle stays byte-identical.

It also reports defined function names that cannot pair with the unit's expected
object symbols (read from the baseline workspace oracle when available, else
from the ``INCLUDE_ASM`` declarations).  ``--reconcile-names`` may rewrite a
single unambiguous definition to the expected symbol, one ``--unit`` at a time.

Usage:
  python3 scripts/normalize-pending-bodies.py --check
  python3 scripts/normalize-pending-bodies.py \
      --unit assembly/textbin/fun_0012e368 --apply
  python3 scripts/normalize-pending-bodies.py --all --apply \
      --report build/normalize-report.json

Contaminated units (candidate bank status ``contaminated``) are skipped: their
bodies are de-contamination work, not normalization work.  The tool refuses
``C_EXACT`` and intentional-assembly units and any path outside
``src/assembly/**``.  Writes are atomic (``os.replace``).
"""

from __future__ import annotations

import argparse
import datetime as dt
import json
import os
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rnc_units  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]

SECTION_ATTR_RE = re.compile(
    r'__attribute__\s*\(\(\s*section\s*\(\s*"\.text[^"]*"\s*\)\s*\)\)'
)
STANDALONE_ATTR_LINE_RE = re.compile(
    r'^[ \t]*' + SECTION_ATTR_RE.pattern + r'[ \t]*\n',
    re.M,
)
INCLUDE_ASM_RE = re.compile(
    r'INCLUDE_ASM\s*\(\s*"([^"]+)"\s*,\s*([A-Za-z_]\w*)\s*\)'
)
GLABEL_RE = re.compile(r'(?m)^\s*(?:glabel|\.globl)\s+([A-Za-z_]\w*)\s*$')
DEF_RE = re.compile(
    r'(?m)^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{'
)
ASM_LABEL_RE = re.compile(
    r'(?m)^[A-Za-z_][\w \t\*]*?\b(?P<name>[A-Za-z_]\w*)\s*'
    r'\([^;{}]*\)\s*__asm__\s*\(\s*"(?P<symbol>[A-Za-z_]\w*)"\s*\)\s*;'
)
SCHEMA = "rnc-normalize-report-v1"


def parse_args(argv=None):
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    scope = parser.add_mutually_exclusive_group(required=True)
    scope.add_argument("--all", action="store_true", help="every pending unit")
    scope.add_argument("--unit", action="append", default=[], metavar="UNIT",
                       help="one configured unit (repeatable)")
    parser.add_argument("--check", action="store_true",
                        help="report only; never write (default unless --apply)")
    parser.add_argument("--apply", action="store_true", help="write the changes")
    parser.add_argument("--strip-intentional-c", action="store_true",
                        help="empty the #else C branch of an intentional-asm unit "
                             "(it has no C goal and can never be measured or staged); "
                             "the INCLUDE_ASM oracle and the guards stay untouched")
    parser.add_argument("--reconcile-names", action="store_true",
                        help="rewrite one unambiguous definition to the expected symbol "
                             "(single --unit only)")
    parser.add_argument("--workspace", type=Path, default=None,
                        help="baseline workspace for expected symbols "
                             "(default: $BASELINE_ROOT or build/baseline)")
    parser.add_argument("--candidates", type=Path, default=None,
                        help="candidate bank directory (default: tools-repo "
                             "build/decompiler/candidates when found)")
    parser.add_argument("--contaminated-list", type=Path, default=None,
                        help="extra contaminated-unit list (one per line or JSON list)")
    parser.add_argument("--report", type=Path, default=None, help="write a JSON report")
    parser.add_argument("--limit", type=int, default=0, help="stop after N units (0 = all)")
    parser.add_argument("--json", action="store_true", help="print the report as JSON")
    parser.add_argument("--verbose", action="store_true",
                        help="also report helper definitions that have no oracle counterpart")
    return parser.parse_args(argv)


def normalize_unit(value: str) -> str | None:
    """A unit path inside ``assembly/`` or None when it can escape the tree."""
    unit = value.strip().replace(os.sep, "/")
    if unit.startswith("/"):
        return None
    if unit.startswith("./"):
        unit = unit[2:]
    if unit.startswith("src/"):
        unit = unit[4:]
    if unit.endswith(".c"):
        unit = unit[:-2]
    parts = [part for part in unit.split("/") if part not in ("", ".")]
    if not parts or any(part == ".." for part in parts):
        return None
    normalized = "/".join(parts)
    if not normalized.startswith("assembly/"):
        return None
    return normalized


def strip_section_attributes(body: str) -> tuple[str, int]:
    """Remove grouping-section attributes from a C body; return (body, count)."""
    count = len(SECTION_ATTR_RE.findall(body))
    if not count:
        return body, 0
    stripped = STANDALONE_ATTR_LINE_RE.sub("", body)
    stripped = SECTION_ATTR_RE.sub("", stripped)
    return stripped, count


def defined_functions(body: str) -> list[str]:
    """Function names defined in a C body (definitions, not declarations)."""
    return sorted(dict.fromkeys(DEF_RE.findall(body)))


def assembler_symbol(body: str, function_name: str) -> str:
    """Return the emitted symbol for a C function, honoring a GNU asm label."""
    labels = {
        match.group("symbol")
        for match in ASM_LABEL_RE.finditer(body)
        if match.group("name") == function_name
    }
    if len(labels) > 1:
        raise ValueError(
            f"conflicting assembler labels for {function_name}: {sorted(labels)}"
        )
    return next(iter(labels)) if labels else function_name


def expected_symbols(workspace: Path | None, unit: str, text: str) -> list[str]:
    """Expected object symbols: workspace oracle labels, else INCLUDE_ASM names."""
    symbols: list[str] = []
    if workspace is not None:
        asm_dir = Path(workspace) / "config" / "us" / "expected" / "asm" / unit
        if asm_dir.is_dir():
            for path in sorted(asm_dir.glob("*.s")):
                symbols.extend(GLABEL_RE.findall(path.read_text(errors="replace")))
    if not symbols:
        symbols = [match.group(2) for match in INCLUDE_ASM_RE.finditer(text)]
    return sorted(dict.fromkeys(symbols))


def rename_definition(body: str, old: str, new: str) -> str:
    """Rename every whole-word occurrence of a function name in the C body."""
    return re.sub(rf"\b{re.escape(old)}\b", new, body)


def candidate_bank() -> Path | None:
    """Locate the refinement candidate bank (tools repo) without a hard path."""
    override = os.environ.get("RNC_CANDIDATES", "").strip()
    if override:
        return Path(override)
    tools_root = os.environ.get("RNC_TOOLS_ROOT", "").strip()
    if tools_root:
        path = Path(tools_root) / "build" / "decompiler" / "candidates"
        if path.is_dir():
            return path
    sibling = ROOT.parent / "Lombyte-Tools" / "build" / "decompiler" / "candidates"
    if sibling.is_dir():
        return sibling
    local = ROOT / "build" / "decompiler" / "candidates"
    if local.is_dir():
        return local
    return None


def load_contaminated(candidates: Path | None, extra: Path | None) -> set[str]:
    units: set[str] = set()
    if candidates is not None and candidates.is_dir():
        for path in candidates.glob("*/candidate.json"):
            try:
                meta = json.loads(path.read_text())
            except (OSError, json.JSONDecodeError):
                continue
            if str(meta.get("status") or "") == "contaminated" and meta.get("unit"):
                units.add(str(meta["unit"]))
    if extra is not None and extra.is_file():
        text = extra.read_text(errors="replace").strip()
        if text.startswith("[") or text.startswith("{"):
            payload = json.loads(text)
            if isinstance(payload, dict):
                units.update(str(unit) for unit in payload.get("contaminated", []))
                units.update(str(unit) for unit in payload.get("units", []))
            else:
                units.update(str(unit) for unit in payload)
        else:
            units.update(line.strip() for line in text.splitlines() if line.strip())
    return units


def unit_plan(unit: str, source: Path, text: str, workspace: Path | None,
              contaminated: set[str], reconcile: bool) -> dict:
    """Plan one unit's normalization without writing anything."""
    plan = {
        "unit": unit,
        "source": str(source),
        "skipped": None,
        "changed": False,
        "attributes_removed": 0,
        "defined": [],
        "expected": [],
        "unpaired": [],
        "renamed": None,
    }
    if unit in contaminated:
        plan["skipped"] = "contaminated"
        return plan
    uses_guard, body = rnc_units.split_oracle_guard(text)
    if not uses_guard or body is None:
        plan["skipped"] = "no-c-body"
        return plan
    if "INCLUDE_ASM" not in text.split("#else", 1)[0]:
        plan["skipped"] = "no-oracle"
        return plan
    if not unit.startswith("assembly/"):
        plan["skipped"] = "promoted"
        return plan

    stripped, count = strip_section_attributes(body)
    plan["attributes_removed"] = count
    plan["defined"] = defined_functions(stripped)
    plan["expected"] = expected_symbols(workspace, unit, text)
    # The oracle and the registry each pick their own spelling of a FUN_ hex
    # tail, so pair case-insensitively; the emitted symbol is still the exact
    # name, this only decides whether the unit's own function is accounted for.
    expected_set = {name.lower() for name in plan["expected"]}
    paired, unpaired = [], []
    for name in plan["defined"]:
        symbol = assembler_symbol(stripped, name)
        (paired if symbol.lower() in expected_set else unpaired).append(name)
    plan["unpaired"] = unpaired
    # A helper with no oracle counterpart is not a defect: inline_fn, IsInRegion
    # and friends are part of the body. The defect is a body that emits NONE of
    # the unit's expected symbols, which is what a wrong-function body does -
    # reporting the helpers as "unpaired" is what made an audit of 298 units come
    # back with eleven false alarms.
    plan["missing_symbol"] = not paired

    if reconcile and plan["missing_symbol"] and len(plan["expected"]) == 1:
        # Only an unambiguous candidate: a definition that carries no asm label
        # of its own, when the oracle names exactly one symbol.
        candidates = [name for name in plan["defined"]
                      if assembler_symbol(stripped, name) == name]
        new = plan["expected"][0]
        if len(candidates) == 1 and candidates[0] != new:
            old = candidates[0]
            stripped = rename_definition(stripped, old, new)
            plan["renamed"] = {"from": old, "to": new}
            plan["defined"] = [name for name in plan["defined"] if name != old] + [new]
            plan["unpaired"] = [name for name in plan["unpaired"] if name != old]
            plan["missing_symbol"] = False
    if stripped != body:
        new_text = text.replace(body, stripped, 1)
        plan["new_text"] = new_text
        plan["changed"] = True
    return plan


def write_unit(source: Path, text: str) -> None:
    """Atomic source write (same directory, os.replace)."""
    tmp = source.with_name(source.name + f".tmp{os.getpid()}")
    tmp.write_text(text)
    os.replace(tmp, source)


def categories(repo: Path) -> tuple[set[str], set[str]]:
    return rnc_units.load_categories(repo / rnc_units.CATEGORY_PATH)


INTENTIONAL_NOTE = (
    "/* No C body on purpose: this unit is intentional low-level assembly\n"
    "   (config/us/unit_categories.json), so it has no C goal and no public\n"
    "   fuzzy score. The assembly oracle above is the whole unit. */"
)


def strip_c_body(text: str) -> dict:
    """Empty the ``#else`` branch of a pending wrapper, keeping the oracle.

    An intentional-asm unit is excluded from the C goal, and ``check-unit.py``
    refuses it by design, so a C body there can never be measured, never be
    staged and never be promoted. It is also the ideal hiding place for a wrong
    body: nothing ever looks at it. Four of the dozen wrong public bodies found
    this way were intentional-asm units.

    The guard pair and the ``INCLUDE_ASM`` line are left exactly as they are, so
    the file still preprocesses to the same oracle and the ELF is unchanged; only
    the C branch is replaced by a note saying why it is empty.
    """
    plan = {"changed": False, "removed_lines": 0, "new_text": None,
            "attributes_removed": 0}
    lines = text.splitlines(keepends=True)
    try:
        start = next(i for i, line in enumerate(lines) if line.strip() == "#else")
        end = next(i for i in range(len(lines) - 1, start, -1)
                   if lines[i].strip().startswith("#endif"))
    except StopIteration:
        return plan
    body = "".join(lines[start + 1:end])
    if not body.strip():
        return plan
    plan["changed"] = True
    plan["removed_lines"] = end - start - 1
    plan["new_text"] = "".join(lines[: start + 1]) + INTENTIONAL_NOTE + "\n" + "".join(lines[end:])
    return plan


def main(argv=None) -> int:
    args = parse_args(argv)
    apply = args.apply
    if args.reconcile_names and (args.all or len(args.unit) != 1):
        print("normalize-pending-bodies: error: --reconcile-names needs exactly one --unit",
              file=sys.stderr)
        return 2

    exact, intentional = categories(ROOT)
    contaminated = load_contaminated(
        args.candidates if args.candidates is not None else candidate_bank(),
        args.contaminated_list,
    )
    if contaminated:
        print(f"normalize-pending-bodies: {len(contaminated)} contaminated units excluded",
              file=sys.stderr)

    if args.all:
        if args.strip_intentional_c:
            # The intentional set is not part of the "pending" category, so
            # classify_units() would never reach these units. When the caller
            # asks for the strip, walk the category itself.
            units = sorted(unit for unit in intentional
                           if rnc_units.unit_source(ROOT, unit).is_file())
        else:
            all_units = rnc_units.classify_units(ROOT)
            units = [unit["owner"] for unit in all_units if unit["category"] == "pending"]
    else:
        units = []
        for raw in args.unit:
            unit = normalize_unit(raw)
            if unit is None:
                print(f"normalize-pending-bodies: error: refusing unit outside "
                      f"src/assembly/**: {raw!r}", file=sys.stderr)
                return 2
            units.append(unit)
    if args.limit > 0:
        units = units[: args.limit]

    workspace = args.workspace or rnc_units.default_workspace(ROOT)
    if not (workspace / ".rnc-baseline-root").is_file():
        print(f"note: no baseline workspace at {workspace}; expected symbols fall back "
              "to INCLUDE_ASM names", file=sys.stderr)
        workspace = None

    results = []
    for unit in units:
        if unit in intentional and not args.strip_intentional_c:
            results.append({"unit": unit, "skipped": "intentional-asm"})
            continue
        if unit in intentional:
            source = rnc_units.unit_source(ROOT, unit)
            if not source.is_file():
                results.append({"unit": unit, "skipped": "missing-source"})
                continue
            plan = strip_c_body(source.read_text(errors="replace"))
            plan["unit"] = unit
            try:
                plan["source"] = str(source.relative_to(ROOT))
            except ValueError:
                pass
            if apply and plan.get("changed"):
                write_unit(source, plan["new_text"])
                plan["applied"] = True
            plan.pop("new_text", None)
            results.append(plan)
            continue
        if unit in exact:
            results.append({"unit": unit, "skipped": "c-exact-under-assembly"})
            continue
        source = rnc_units.unit_source(ROOT, unit)
        if not rnc_units.path_inside(source, ROOT / "src"):
            results.append({"unit": unit, "skipped": "outside-source-tree"})
            continue
        if not source.is_file():
            results.append({"unit": unit, "skipped": "missing-source"})
            continue
        text = source.read_text(errors="replace")
        plan = unit_plan(unit, source, text, workspace, contaminated, args.reconcile_names)
        try:
            plan["source"] = str(source.relative_to(ROOT))
        except ValueError:
            pass
        if apply and plan.get("changed") and plan.get("new_text") is not None:
            write_unit(source, plan["new_text"])
            plan["applied"] = True
        plan.pop("new_text", None)
        results.append(plan)

    report = {
        "schema": SCHEMA,
        "generated_at": dt.datetime.now(dt.timezone.utc).isoformat(),
        "mode": "apply" if apply else "check",
        "workspace": str(workspace) if workspace else None,
        "candidates": str(args.candidates) if args.candidates else None,
        "counts": {
            "total": len(results),
            "changed": sum(1 for row in results if row.get("changed")),
            "skipped": sum(1 for row in results if row.get("skipped")),
            "renamed": sum(1 for row in results if row.get("renamed")),
            "unpaired": sum(1 for row in results if row.get("unpaired")),
            "missing_symbol": sum(1 for row in results if row.get("missing_symbol")),
        },
        "units": results,
    }
    if not args.json:
        for row in results:
            if row.get("changed"):
                action = "applied" if row.get("applied") else "would normalize"
                print(f"{action}: {row['unit']} "
                      f"({row['attributes_removed']} attribute(s) removed"
                      + (f", renamed {row['renamed']['from']} -> {row['renamed']['to']}"
                         if row.get("renamed") else "")
                      + ")")
            if row.get("missing_symbol"):
                print(f"name mismatch: {row['unit']} emits none of {row['expected']} "
                      f"(defined {row['defined']})")
            elif row.get("unpaired") and args.verbose:
                print(f"note: {row['unit']} has {len(row['unpaired'])} helper definition(s) "
                      f"with no oracle counterpart: {', '.join(row['unpaired'])}")
            if row.get("skipped"):
                print(f"skip ({row['skipped']}): {row['unit']}")
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2) + "\n")
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(json.dumps(report["counts"], indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
