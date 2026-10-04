#!/usr/bin/env python3
"""List the units whose C is still pending, smallest first.

Reads ``config/us/rnc1.us.yaml`` and ``config/us/unit_categories.json``; no
build or game files needed.  A *pending* unit is assembly-backed
(``src/assembly/...``) and is neither intentional low-level asm nor already
byte-exact; by default only units that already carry a C body under ``#else``
are listed, so you can refine rather than write from scratch.

With ``--score`` every listed unit's C body is measured against retail and the
highest scores are listed first.  This needs the baseline workspace built once
with ``./verify-baseline.sh`` (``build/baseline`` inside the checkout, or
``$BASELINE_ROOT`` when set); only that workspace is written to.

Usage:
  python3 scripts/list-functions.py                 # 25 smallest with a C body
  python3 scripts/list-functions.py --score         # rank by current match %
  python3 scripts/list-functions.py --score --limit 50 --filter textbin
  python3 scripts/list-functions.py --all           # include units with no C yet
  python3 scripts/list-functions.py --json
  python3 scripts/list-functions.py --overlays      # pending level overlay functions
"""

from __future__ import annotations

import argparse
import datetime as dt
import hashlib
import json
import os
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rnc_units  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
INDEX_SCHEMA = "rnc-pending-similarity-v1"


def parse_args(argv=None):
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--overlays",
        action="store_true",
        help="list pending level overlay functions (FUN_LNN_*) instead",
    )
    parser.add_argument(
        "--all",
        action="store_true",
        help="also list units that have no readable C body yet",
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=25,
        metavar="N",
        help="show at most N units; 0 means no limit (default: 25)",
    )
    parser.add_argument(
        "--filter",
        metavar="TEXT",
        help="only units whose owner path contains TEXT",
    )
    parser.add_argument(
        "--score",
        action="store_true",
        help="measure each listed C body and sort by match percentage",
    )
    parser.add_argument(
        "--workspace",
        type=Path,
        default=None,
        help="baseline workspace for --score (default: $BASELINE_ROOT or build/baseline in the checkout)",
    )
    parser.add_argument(
        "--json",
        action="store_true",
        help="print the matching units as JSON",
    )
    parser.add_argument(
        "--out",
        type=Path,
        default=None,
        help="write the full rnc-pending-similarity-v1 index for every pending "
        "unit (implies --score; atomic write)",
    )
    parser.add_argument(
        "--audit",
        type=Path,
        default=None,
        help="audit JSON to bind the index to (default: latest under the "
        "tools repo when found)",
    )
    return parser.parse_args(argv)


def score_unit(owner: str, workspace: Path) -> tuple[dict | None, str | None]:
    """Measure one unit through check-unit.py; return (payload, error)."""
    process = subprocess.run(
        [
            sys.executable,
            str(ROOT / "scripts" / "check-unit.py"),
            owner,
            "--workspace",
            str(workspace),
            "--json",
        ],
        capture_output=True,
        text=True,
    )
    if process.returncode not in (0, 1):
        lines = [line for line in process.stderr.strip().splitlines() if line.strip()]
        return None, lines[-1] if lines else "check-unit failed"
    try:
        payload = json.loads(process.stdout)
    except json.JSONDecodeError:
        return None, "check-unit returned no usable JSON"
    if not isinstance(payload, dict):
        return None, "check-unit returned no result object"
    return payload, None


def apply_payload(unit: dict, payload: dict | None, problem: str | None) -> None:
    """Copy one check-unit payload onto a listing row."""
    unit["score_error"] = problem
    if payload is None:
        unit["score"] = None
        unit["strict_score"] = None
        unit["measurable"] = False
        unit["unmeasurable_reason"] = "score-error" if problem else None
        unit["non_text_ok"] = None
        return
    unit["score"] = payload.get("text_match_percent")
    unit["strict_score"] = payload.get("strict_match_percent")
    unit["measurable"] = bool(payload.get("measurable"))
    unit["unmeasurable_reason"] = payload.get("unmeasurable_reason")
    unit["non_text_ok"] = payload.get("non_text_ok")


def format_score(percent: float | None) -> str:
    """Keep near-exact scores visibly below 100%."""
    if percent is None:
        return "-"
    if 99.9 <= percent < 100.0:
        return f"{percent:.3f}%"
    return f"{percent:.1f}%"


def score_units(units: list[dict], workspace: Path) -> int:
    """Fill in ``score`` for every unit; return the error count."""
    scorable = [unit for unit in units if unit["c_body"]]
    for unit in units:
        unit["score"] = None
        unit["score_error"] = None
        if not unit["c_body"]:
            unit["measurable"] = False
            unit["unmeasurable_reason"] = "no-c-body"
            unit["non_text_ok"] = None
            unit["strict_score"] = None
    errors = 0
    for index, unit in enumerate(scorable, 1):
        payload, problem = score_unit(unit["owner"], workspace)
        apply_payload(unit, payload, problem)
        percent = unit["score"]
        if problem:
            errors += 1
            shown = "error"
        elif percent is None:
            shown = "?"
        else:
            shown = format_score(percent)
        print(
            f"[{index:>3}/{len(scorable)}] {shown:>7}  {unit['owner']}",
            file=sys.stderr,
        )
    return errors


def default_audit() -> Path | None:
    """Latest source-quality audit in the tools repo, when this checkout has one."""
    candidates = []
    tools_root = os.environ.get("RNC_TOOLS_ROOT", "").strip()
    if tools_root:
        candidates.append(Path(tools_root) / "analysis" / "audits")
    candidates.append(ROOT.parent / "Lombyte-Tools" / "analysis" / "audits")
    candidates.append(ROOT / "analysis" / "audits")
    for directory in candidates:
        if directory.is_dir():
            matches = sorted(directory.glob("source-quality-audit-*.json"))
            if matches:
                return matches[-1]
    return None


def sha256_file(path: Path) -> str | None:
    try:
        return hashlib.sha256(Path(path).read_bytes()).hexdigest()
    except OSError:
        return None


def write_index(path: Path, units: list[dict], workspace: Path, audit: Path | None) -> None:
    """Write the rnc-pending-similarity-v1 index atomically."""
    audit = audit or default_audit()
    retail = Path(workspace) / "config" / "us" / "SCUS_971.99"
    payload = {
        "schema": INDEX_SCHEMA,
        "generated_at": dt.datetime.now(dt.timezone.utc).isoformat(),
        "repo": str(ROOT),
        "workspace": str(workspace),
        "retail_sha256": sha256_file(retail),
        "audit_sha256": sha256_file(audit) if audit else None,
        "count": len(units),
        "units": [
            {
                "unit": unit["owner"],
                "address": f"0x{unit['address']:X}",
                "bytes": unit["size"],
                "name": unit["display"],
                "source": str(unit["source"].relative_to(ROOT)),
                "has_c_body": unit["c_body"],
                "score": unit.get("score"),
                "strict_score": unit.get("strict_score"),
                "measurable": unit.get("measurable"),
                "unmeasurable_reason": unit.get("unmeasurable_reason"),
                "non_text_ok": unit.get("non_text_ok"),
                "score_error": unit.get("score_error"),
            }
            for unit in sorted(units, key=lambda row: row["owner"])
        ],
    }
    if audit:
        payload["audit"] = str(audit)
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(path.name + f".tmp{os.getpid()}")
    tmp.write_text(json.dumps(payload, indent=2) + "\n")
    os.replace(tmp, path)
    print(f"wrote {path} ({payload['count']} units, schema {INDEX_SCHEMA})", file=sys.stderr)


def list_overlays(args) -> int:
    import overlay_units

    pending = [f for f in overlay_units.overlay_functions(ROOT) if not f["exact"] and f["file"]]
    if args.filter:
        needle = args.filter.lower()
        pending = [f for f in pending if needle in (f["name"] + f["file"]).lower()]
    pending.sort(key=lambda f: (f["size"], f["name"]))
    shown = pending[: args.limit] if args.limit else pending
    if args.json:
        print(json.dumps(shown, indent=2))
        return 0
    for f in shown:
        print(f"{f['size']:>6} B  {f['name']}  {f['kind']:<6} {f['file']}")
    print(f"\n{len(shown)} of {len(pending)} pending overlay functions; "
          "check one with: python3 scripts/check-unit.py <name>")
    return 0


def main(argv=None) -> int:
    args = parse_args(argv)
    if args.overlays:
        return list_overlays(args)
    all_units = rnc_units.classify_units(ROOT)
    listed = [unit for unit in all_units if unit["category"] == "pending"]
    if args.filter:
        needle = args.filter.lower()
        listed = [unit for unit in listed if needle in unit["owner"].lower()]
    for unit in listed:
        text = (
            unit["source"].read_text(errors="replace")
            if unit["source"].is_file()
            else ""
        )
        has_guard, body = rnc_units.split_oracle_guard(text)
        unit["c_body"] = bool(body) if has_guard else bool(text)

    with_body = [unit for unit in listed if unit["c_body"]]
    selected = listed if args.all else with_body
    to_score = len(with_body)

    score_mode = args.score or args.out is not None
    errors = 0
    elapsed = 0.0
    workspace = None
    if score_mode:
        workspace = args.workspace or rnc_units.default_workspace(ROOT)
        workspace = workspace.expanduser().resolve()
        problem = rnc_units.workspace_problem(workspace, ROOT)
        if problem:
            print(f"list-functions: error: {problem}", file=sys.stderr)
            return 2
        if not rnc_units.path_inside(workspace, ROOT):
            print(
                f"note: workspace is outside the checkout: {workspace}",
                file=sys.stderr,
            )
        started = time.monotonic()
        if args.out is not None:
            # The index covers every pending unit, not just the printed page.
            errors = score_units(list(listed), workspace)
            for unit in listed:
                unit["_scored"] = True
        else:
            errors = score_units(list(selected), workspace)
        elapsed = time.monotonic() - started
        selected.sort(
            key=lambda unit: (
                0 if unit["score"] is None else -unit["score"],
                unit["size"],
                unit["owner"],
            )
        )
    else:
        selected.sort(key=lambda unit: (unit["size"], unit["owner"]))
    if args.limit > 0:
        selected = selected[: args.limit]

    if args.out is not None:
        write_index(args.out, listed, workspace, args.audit)

    if args.json:
        print(
            json.dumps(
                [
                    {
                        "unit": unit["owner"],
                        "address": f"0x{unit['address']:X}",
                        "bytes": unit["size"],
                        "name": unit["display"],
                        "source": str(unit["source"].relative_to(ROOT)),
                        "has_c_body": unit["c_body"],
                        "score": unit.get("score"),
                        "score_error": unit.get("score_error"),
                    }
                    for unit in selected
                ],
                indent=2,
            )
        )
        return 0

    total = sum(unit["size"] for unit in listed)
    print(
        f"{len(listed)} pending units / {total:,} bytes "
        f"(of {len(all_units)} configured C units)\n"
    )
    if score_mode:
        print(
            f"Scored {to_score} units in {elapsed:.1f}s; highest match "
            "first (closest to promotion):\n"
        )
    elif not args.all:
        missing = [unit for unit in listed if not unit["c_body"]]
        line = f"{len(with_body)} of them have a readable C body; smallest first"
        if missing:
            line += f" ({len(missing)} more have no C yet; pass --all to include them)"
        print(line + ":\n")
    else:
        print("Smallest first:\n")

    if not selected:
        print("  (no matching unit)")
        return 0

    width = max(len(unit["owner"]) for unit in selected)
    if score_mode:
        print(f"  {'SCORE':>6}  {'BYTES':>6}  {'UNIT':<{width}}  NAME")
    else:
        print(f"  {'BYTES':>6}  {'UNIT':<{width}}  NAME")
    for unit in selected:
        name = unit["display"] or "-"
        flag = "" if unit["c_body"] else "  [asm only]"
        if score_mode:
            score = unit.get("score")
            shown = "error" if unit.get("score_error") else (
                format_score(score)
            )
            print(f"  {shown:>6}  {unit['size']:>6}  {unit['owner']:<{width}}  {name}{flag}")
        else:
            print(f"  {unit['size']:>6}  {unit['owner']:<{width}}  {name}{flag}")

    shown = len(selected)
    pool = len(listed if args.all else with_body)
    if shown < pool:
        print(f"\nShowing {shown} of {pool}.")
    if errors:
        print(f"{errors} unit(s) could not be measured.")
    if not score_mode:
        print("\nTip: pass --score to rank the list by current match percentage.")
    print("\nRefine one with:" if score_mode else "\nPick one and measure it with:")
    print("  python3 scripts/check-unit.py <unit>")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
