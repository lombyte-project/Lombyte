#!/usr/bin/env python3
"""Measure one unit's C against the retail object, rebuilding only that object.

Usage:
  python3 scripts/check-unit.py <unit> [--workspace DIR] [--json]

``<unit>`` is a configured unit path such as ``assembly/textbin/runtime/memory/clear_u64_value``,
or a level overlay function such as ``FUN_L00_002e0988`` (docs/overlays.md); a
leading ``src/`` and a trailing ``.c`` are accepted.  When the file keeps its
assembly oracle under ``#ifndef NON_MATCHING``, the C body under ``#else`` is
staged into the workspace and measured alone, so the source tree keeps its
oracle while you iterate.

This is the fast inner loop, not the acceptance gate: ``make elf`` stays
authoritative.  It needs a baseline workspace built once with
``./verify-baseline.sh`` (``build/baseline``, or ``$BASELINE_ROOT``) and writes
only inside it.

Usage examples:
  python3 scripts/check-unit.py assembly/textbin/runtime/memory/clear_u64_value
  python3 scripts/check-unit.py assembly/runtime/dma/dma_to_spr --workspace build/baseline
  python3 scripts/check-unit.py FUN_L00_002e0988
"""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rnc_units  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]


def parse_args(argv=None):
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("unit", help="configured unit path, e.g. assembly/textbin/fast_sin")
    parser.add_argument(
        "--workspace",
        type=Path,
        default=None,
        help="baseline workspace (default: $BASELINE_ROOT or build/baseline in the checkout)",
    )
    parser.add_argument("--json", action="store_true", help="print the result as JSON")
    return parser.parse_args(argv)


def error(message: str, code: int = 2) -> int:
    print(f"check-unit: error: {message}", file=sys.stderr)
    return code


def normalize_unit(value: str) -> str | None:
    """Normalize a unit path; None when it is empty or can escape the tree."""
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
    return "/".join(parts)


def find_ninja() -> str | None:
    venv_ninja = ROOT / ".venv" / "bin" / "ninja"
    if venv_ninja.is_file():
        return str(venv_ninja)
    return shutil.which("ninja")


def build_environment() -> dict:
    env = dict(os.environ)
    # Match verify-baseline.sh when reconfiguring its disposable workspace.
    env["SN_TOOLCHAIN_ROOT"] = env.get("SN_TOOLCHAIN_ROOT") or str(
        ROOT / "tools/compilers/ee-gcc-2.95.2"
    )
    prefix = []
    venv_bin = ROOT / ".venv" / "bin"
    if venv_bin.is_dir():
        prefix.append(str(venv_bin))
    binutils = os.environ.get("BINUTILS_ROOT")
    candidates = (
        [Path(binutils)]
        if binutils
        else [ROOT / "tools" / "binutils-mips-ps2-decompals", Path("/opt/binutils-mips-ps2-decompals")]
    )
    for candidate in candidates:
        if candidate.is_dir():
            prefix.append(str(candidate))
            break
    if prefix:
        env["PATH"] = os.pathsep.join(prefix + [env.get("PATH", "")])
    return env


def objdiff_report(objdiff: Path, project: Path, unit: str, config: str | None):
    command = [
        str(objdiff),
        "diff",
        "-p",
        str(project),
        "-u",
        unit,
        "--format",
        "json",
        "-o",
        "-",
    ]
    if config:
        command += ["-c", config]
    return subprocess.run(
        command, capture_output=True, text=True, cwd=str(project)
    )


def function_rows(side: dict) -> list[dict]:
    return [
        {
            "name": symbol["name"],
            "size": int(symbol.get("size") or 0),
            "match_percent": symbol.get("match_percent"),
        }
        for symbol in side.get("symbols", [])
        if symbol.get("kind") == "SYMBOL_FUNCTION"
    ]


def first_differences(left: dict, right: dict, limit: int = 3) -> list[str]:
    """Human-readable first mismatching instructions (retail vs candidate)."""
    candidates = {symbol["name"]: symbol for symbol in right.get("symbols", [])}
    rows = []
    for target in left.get("symbols", []):
        if target.get("kind") != "SYMBOL_FUNCTION":
            continue
        candidate = candidates.get(target["name"], {})
        target_instructions = target.get("instructions") or []
        candidate_instructions = candidate.get("instructions") or []
        for index in range(max(len(target_instructions), len(candidate_instructions))):
            target_row = (
                target_instructions[index] if index < len(target_instructions) else {}
            )
            candidate_row = (
                candidate_instructions[index] if index < len(candidate_instructions) else {}
            )
            if not target_row.get("diff_kind") and not candidate_row.get("diff_kind"):
                continue
            address = (target_row.get("instruction") or candidate_row.get("instruction") or {}).get(
                "address"
            )
            retail_text = (target_row.get("instruction") or {}).get("formatted", "").strip()
            candidate_text = (candidate_row.get("instruction") or {}).get("formatted", "").strip()
            rows.append(
                "  0x{address:<4} {retail:<32} {candidate}".format(
                    address=address if address is not None else "0",
                    retail=retail_text or "(missing)",
                    candidate=candidate_text or "(missing)",
                )
            )
            if len(rows) >= limit:
                return rows
    return rows


def section_percentages(left: dict) -> tuple[float | None, list[dict]]:
    """Split a side payload into (.text percent, non-.text section rows)."""
    text_match = None
    others = []
    for section in left.get("sections", []):
        percent = section.get("match_percent")
        if section["name"] == ".text":
            if percent is not None:
                text_match = percent
        elif percent is not None:
            others.append(
                {
                    "name": section["name"],
                    "size": section.get("size"),
                    "match_percent": percent,
                }
            )
    return text_match, others


def score_verdict(
    *,
    text_match: float | None,
    functions: list[dict],
    section_rows: list[dict],
    custom_sections: list[str] | None = None,
    non_text_ok: bool | None = None,
) -> dict:
    """Pure measurement verdict for one unit.

    ``ok`` keeps its original meaning (``.text`` and every function at 100%),
    so existing consumers keep working; ``promotable`` additionally requires
    every non-``.text`` section at 100% on the strict pass.
    """
    custom_sections = custom_sections or []
    # A zero-size alias label (func_xxxx beside FUN_xxxx) has no body to
    # score, so objdiff reports None for it; it cannot block the verdict.
    functions = [
        row for row in functions
        if not (row["match_percent"] is None and row.get("size", 0) == 0)
    ]
    if custom_sections:
        return {
            "measurable": False,
            "unmeasurable_reason": "custom-sections",
            "non_text_ok": None,
            "promotable": False,
            "ok": False,
        }
    if text_match is None or not functions:
        return {
            "measurable": False,
            "unmeasurable_reason": "no-pairing",
            "non_text_ok": None,
            "promotable": False,
            "ok": False,
        }
    if all(row["match_percent"] is None for row in functions):
        return {
            "measurable": False,
            "unmeasurable_reason": "no-pairing",
            "non_text_ok": None,
            "promotable": False,
            "ok": False,
        }
    ok = text_match == 100.0 and all(
        function["match_percent"] == 100.0 for function in functions
    )
    if non_text_ok is None:
        non_text_ok = all(row["match_percent"] == 100.0 for row in section_rows)
    return {
        "measurable": True,
        "unmeasurable_reason": None,
        "non_text_ok": bool(non_text_ok),
        "promotable": bool(ok and non_text_ok),
        "ok": ok,
    }


def fail_result(unit: str, reason: str, message: str, *, as_json: bool, code: int) -> int:
    """Report a unit that cannot be measured (keeps exit codes unchanged)."""
    if as_json:
        print(
            json.dumps(
                {
                    "unit": unit,
                    "measurable": False,
                    "unmeasurable_reason": reason,
                    "non_text_ok": None,
                    "promotable": False,
                    "ok": False,
                    "error": message,
                },
                indent=2,
            )
        )
    print(f"check-unit: error: {message}", file=sys.stderr)
    return code


def print_result(result: dict) -> None:
    print(f"unit       {result['unit']}")
    print(f"source     {result['source']} ({result['staged_from']})")
    print(f"retail     {result['retail_bytes']} bytes")
    for index, path in enumerate(result["retail_asm"]):
        label = "retail asm" if index == 0 else "          "
        print(f"{label} {result['workspace']}/{path}")
    print(f"workspace  {result['workspace']}")
    print()
    print(f"rebuilt build/src/{result['unit']}.c.o")
    print()
    print("object score (objdiff; relocation names ignored for linked bytes)")
    print(f"  .text   {result['text_match_percent']:.1f}%")
    for function in result["functions"]:
        percent = function["match_percent"]
        shown = "n/a" if percent is None else f"{percent:.1f}%"
        print(f"  {function['name']}   {shown}")
    for section in result["other_sections"]:
        print(
            f"  {section['name']}   {section['match_percent']:.1f}% "
            f"({section['size']} bytes)"
        )
    print()
    if result["promotable"]:
        print("Object matches (promotable).")
        if result["strict_match_percent"] not in (None, 100.0):
            print(
                "Strict symbol-name comparison still differs "
                f"({result['strict_match_percent']:.1f}%); the linked bytes are "
                "equal, so the full-image gate decides."
            )
        print("Run ./verify-baseline.sh for the authoritative full-image SHA gate.")
        return
    if result["ok"] and result["non_text_ok"] is False:
        print("Text matches; data/rodata differ — the full gate will fail.")
        for section in result["other_sections"]:
            print(
                f"  {section['name']}   {section['match_percent']:.1f}% "
                f"({section['size']} bytes)"
            )
        return
    if result["ok"]:
        print(
            "Text matches; promotability unverified (the strict pass did not "
            "produce non-.text percentages)."
        )
        return
    if result["measurable"]:
        print(f"Not matching yet ({result['text_match_percent']:.1f}%).")
    else:
        print(f"Not measurable: {result['unmeasurable_reason']} "
              f"({result['text_match_percent']:.1f}%).")
    for note in result["notes"]:
        print()
        print(f"note: {note}")
    if result["differences"]:
        print()
        print("first differences (retail vs your C)")
        for row in result["differences"]:
            print(row)
    print()
    print("Edit the C body, then run this command again.")


def check_overlay(name: str, as_json: bool) -> int:
    try:
        import elftools  # noqa: F401
    except ImportError:
        venv = ROOT / ".venv" / "bin" / "python"
        if venv.is_file() and Path(sys.executable) != venv:
            os.execv(str(venv), [str(venv), *sys.argv])
        return error("pyelftools is missing; install requirements.txt into .venv")
    import overlay_unit

    result = overlay_unit.check(name)
    if as_json:
        print(json.dumps(result, indent=2))
    else:
        overlay_unit.print_check(result)
    if "error" in result:
        return 2
    return 0 if result["ok"] else 1


def main(argv=None) -> int:
    args = parse_args(argv)
    if rnc_units.OVERLAY_NAME.match(args.unit.strip()):
        return check_overlay(args.unit.strip(), args.json)
    unit = normalize_unit(args.unit)
    if unit is None:
        return error(f"invalid unit path: {args.unit!r}")
    source = rnc_units.unit_source(ROOT, unit)
    if not rnc_units.path_inside(source, ROOT / "src"):
        return error(f"refusing to read outside the source tree: {source}")
    if not source.is_file():
        return error(f"no such unit source: {source}")
    _, intentional = rnc_units.load_categories(ROOT / rnc_units.CATEGORY_PATH)
    if unit in intentional:
        return fail_result(
            unit,
            "intentional-asm",
            f"{unit} is intentional low-level asm and is excluded from the C "
            "goal; pick another unit (see scripts/list-functions.py)",
            as_json=args.json,
            code=2,
        )

    text = source.read_text(errors="replace")
    uses_guard, body = rnc_units.split_oracle_guard(text)
    if uses_guard and body is None:
        return error(
            f"{unit} has no readable C body yet; write your C under `#else` "
            "and run this command again"
        )
    if not uses_guard and "INCLUDE_ASM" in text:
        return error(
            f"{unit} is assembly-only (`INCLUDE_ASM` without a C body); "
            "pick a unit that has a C body (see scripts/list-functions.py)"
        )
    staged = body if uses_guard else text
    staged_from = (
        "C body staged from the NON_MATCHING guard" if uses_guard else "staged as-is"
    )

    workspace = args.workspace or rnc_units.default_workspace(ROOT)
    workspace = workspace.expanduser().resolve()
    problem = rnc_units.workspace_problem(workspace, ROOT)
    if problem:
        return error(problem)
    if not rnc_units.path_inside(workspace, ROOT):
        print(f"note: workspace is outside the checkout: {workspace}", file=sys.stderr)
    project = workspace / "config" / "us"
    objdiff = workspace / "tools" / "objdiff" / "objdiff-cli"
    workspace_source = workspace / "src" / f"{unit}.c"
    if not rnc_units.path_inside(workspace_source, workspace):
        return error(f"refusing to write outside the workspace: {workspace_source}")
    if not workspace_source.is_file():
        return error(
            f"{unit} is not part of {workspace}; re-run ./verify-baseline.sh "
            "after pulling the latest source"
        )
    if unit in rnc_units.oracle_fallback_units(workspace):
        return fail_result(
            unit,
            "oracle-fallback",
            f"{unit} is rebuilt from the retail oracle in this workspace "
            "because EE_GCC_PATCHED_ROOT is not set; run "
            "scripts/build-patched-toolchain.py, re-run make elf, then measure "
            "its C",
            as_json=args.json,
            code=2,
        )

    ninja = find_ninja()
    if ninja is None:
        return error("ninja was not found; install the requirements into .venv")

    if workspace_source.read_text() != staged:
        workspace_source.write_text(staged)
    object_target = f"build/src/{unit}.c.o"
    # The workspace was configured with the oracle present. Staging its C
    # does not change Ninja's rule: refresh it so configure selects the normal
    # game-compiler route (including Ps2EeAs), rather than include-asm.
    oracle_rule = f"build {object_target}: include-asm "
    if any(
        line.startswith(oracle_rule)
        for line in (project / "build.ninja").read_text().splitlines()
    ):
        configured = subprocess.run(
            [sys.executable, str(workspace / "configure.py"), "config/us/rnc1.us.yaml"],
            cwd=str(workspace), capture_output=True, text=True,
            env=build_environment(),
        )
        if configured.returncode != 0:
            print(configured.stdout, end="", file=sys.stderr)
            print(configured.stderr, end="", file=sys.stderr)
            return fail_result(
                unit, "configure-failed", "could not configure the staged C build",
                as_json=args.json, code=2,
            )
    build = subprocess.run(
        [ninja, "-C", str(project), object_target],
        capture_output=True,
        text=True,
        env=build_environment(),
    )
    if build.returncode != 0:
        print(build.stdout, end="", file=sys.stderr)
        print(build.stderr, end="", file=sys.stderr)
        return fail_result(
            unit,
            "compile-failed",
            f"compiling {unit} failed; fix the compile errors above",
            as_json=args.json,
            code=1,
        )

    scored = objdiff_report(objdiff, project, unit, "functionRelocDiffs=none")
    if scored.returncode != 0:
        print(scored.stderr, end="", file=sys.stderr)
        return error(f"objdiff failed for {unit}", 1)
    try:
        payload = json.loads(scored.stdout)
    except json.JSONDecodeError:
        return error(f"objdiff returned no usable JSON for {unit}", 1)

    left = payload.get("left") or {}
    text_match, section_rows = section_percentages(left)
    functions = function_rows(left)
    other_sections = [
        row for row in section_rows if row["match_percent"] != 100.0
    ]
    right = payload.get("right") or {}
    target_sections = {section["name"] for section in left.get("sections", [])}
    custom_sections = [
        section["name"]
        for section in right.get("sections", [])
        if section["name"].startswith(".text.")
        and section["name"] not in target_sections
    ]
    verdict = score_verdict(
        text_match=text_match,
        functions=functions,
        section_rows=section_rows,
        custom_sections=custom_sections,
    )
    ok = verdict["ok"]
    notes = []
    if not ok:
        if custom_sections:
            notes.append(
                "your C body places code in custom sections ("
                + ", ".join(custom_sections)
                + "); the retail unit object uses plain `.text`. Remove the "
                '`__attribute__((section(".text.*")))` seed attributes so the '
                "functions can be paired and linked."
            )
        elif any(
            function["match_percent"] is None and function.get("size", 0) > 0
            for function in functions
        ):
            notes.append(
                "the retail function symbol(s) could not be paired with your "
                "C; check the canonical symbol names in the body "
                "(see src/README.md)."
            )

    strict_match = None
    non_text_ok = verdict["non_text_ok"]
    if ok:
        strict = objdiff_report(objdiff, project, unit, None)
        if strict.returncode == 0:
            try:
                strict_left = json.loads(strict.stdout).get("left") or {}
                strict_text, strict_rows = section_percentages(strict_left)
                if strict_text is not None:
                    strict_match = strict_text
                non_text_ok = all(
                    row["match_percent"] == 100.0 for row in strict_rows
                )
            except json.JSONDecodeError:
                strict_match = None
                non_text_ok = None
        else:
            non_text_ok = None
    promotable = bool(ok and non_text_ok)

    asm_dir = project / "expected" / "asm" / unit
    retail_asm = (
        sorted(
            str(path.relative_to(workspace))
            for path in asm_dir.glob("*.s")
            if path.is_file()
        )
        if asm_dir.is_dir()
        else []
    )
    result = {
        "unit": unit,
        "source": str(source.relative_to(ROOT)),
        "workspace": str(workspace),
        "staged_from": staged_from,
        "retail_bytes": sum(function["size"] for function in functions),
        "retail_asm": retail_asm,
        "functions": functions,
        "text_match_percent": text_match or 0.0,
        "strict_match_percent": strict_match,
        "other_sections": other_sections,
        "notes": notes,
        "differences": [] if ok else first_differences(left, payload.get("right") or {}),
        "ok": ok,
        "measurable": verdict["measurable"],
        "unmeasurable_reason": verdict["unmeasurable_reason"],
        "non_text_ok": non_text_ok,
        "promotable": promotable,
    }
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        print_result(result)
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
