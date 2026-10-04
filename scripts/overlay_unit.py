"""check-unit and `make overlays` for level overlay functions (FUN_LNN_xxxxxxxx).

A pending overlay function keeps its stub as the oracle and its C under
``#else``, exactly like an executable unit:

    #ifndef NON_MATCHING
    INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002e0988.s", FUN_L00_002e0988);
    #else
    void FUN_L00_002e0988(...) { ... }
    #endif /* NON_MATCHING */

The C is compiled on the game route (cc1, Ps2EeAs; build/overlays/build.ninja)
and placed at the function's address in its level; it is exact when the
bytes equal the level's text (scripts/overlay_proof.py).
"""
from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

import rnc_overlays as ov

ROOT = ov.ROOT
SOURCES = ROOT / "src/overlays"
BUILD = ROOT / "build/overlays"
NAME_RE = re.compile(r"^FUN_L\d{2}_[0-9a-f]{8}$")
GUARD_RE = re.compile(
    r"^#ifndef NON_MATCHING\n(?P<stub>INCLUDE_ASM\([^\n]*\b{name}\)\s*;?)\n"
    r"#else\n(?P<body>.*?)^#endif[^\n]*\n", re.M | re.S)


def is_overlay_name(value: str) -> bool:
    return bool(NAME_RE.match(value))


def find_source(name: str) -> Path | None:
    """The file holding NAME's stub, else its C definition."""
    import overlay_units

    stubs, c = overlay_units.source_state(ROOT)
    rel = stubs.get(name) or c.get(name)
    return ROOT / rel if rel else None


def stage(name: str, text: str) -> tuple[str | None, str]:
    """(C to compile, how it was staged); None when there is no C yet."""
    m = re.compile(GUARD_RE.pattern.replace("{name}", name), GUARD_RE.flags).search(text)
    if m:
        text = text[:m.start()] + m.group("body") + text[m.end():]
        how = "C body staged from the NON_MATCHING guard"
    elif re.search(rf'INCLUDE_ASM\([^\n]*\b{name}\)', text):
        return None, ""
    else:
        how = "promoted C, staged as-is"
    return "".join(line for line in text.splitlines(keepends=True)
                   if "INCLUDE_ASM" not in line), how


def ninja(*targets: str) -> subprocess.CompletedProcess:
    exe = ROOT / ".venv/bin/ninja"
    return subprocess.run([str(exe) if exe.is_file() else "ninja", "-C", str(BUILD), *targets],
                          capture_output=True, text=True)


def ensure_configured(force: bool = False) -> str | None:
    """Write build/overlays/build.ninja when missing (always with FORCE, so a
    new or renamed source file is picked up)."""
    if not ov.ASM_DIR.is_dir():
        return ("config/us/overlays/asm is missing: run "
                "`python3 scripts/overlay-extract.py --iso <your disc image>` first")
    if force or not (BUILD / "build.ninja").is_file():
        proc = subprocess.run([sys.executable, "configure.py", "--overlays"], cwd=ROOT,
                              capture_output=True, text=True)
        if proc.returncode:
            return proc.stdout + proc.stderr
    return None


def check(name: str) -> dict:
    """check-unit for one overlay function."""
    result = {"unit": name, "ok": False, "promotable": False}
    if name not in ov.read_catalogue():
        return {**result, "error": f"{name} is not in config/overlays/us/functions.tsv"}
    source = find_source(name)
    if source is None:
        return {**result, "error": f"no file under src/overlays holds {name}"}
    result["source"] = str(source.relative_to(ROOT))
    result["retail_asm"] = str((ov.ASM_DIR / f"{name}.s").relative_to(ROOT))
    staged, how = stage(name, source.read_text(errors="replace"))
    if staged is None:
        return {**result, "error": (
            f"{name} has no C yet. Wrap its INCLUDE_ASM line in {result['source']} as\n"
            "  #ifndef NON_MATCHING\n  INCLUDE_ASM(...);\n  #else\n  <your C>\n"
            "  #endif /* NON_MATCHING */\nand run this command again")}
    result["staged_from"] = how
    problem = ensure_configured()
    if problem:
        return {**result, "error": problem}
    target = BUILD / "stage" / f"{name}.c"
    target.parent.mkdir(parents=True, exist_ok=True)
    if not target.is_file() or target.read_text() != staged:
        target.write_text(staged)
    built = ninja(f"stage/{name}.c.o")
    if built.returncode:
        return {**result, "error": "compile failed:\n" + (built.stdout + built.stderr)[-3000:]}
    import overlay_proof as proof

    verdict = proof.check(BUILD / "stage" / f"{name}.c.o", name, show=True)
    result.update(verdict=verdict["verdict"], retail_bytes=verdict.get("size"),
                  differences=verdict.get("diff", [])[:8],
                  ok=verdict["exact"], promotable=verdict["exact"])
    return result


def print_check(result: dict) -> None:
    print(f"unit       {result['unit']}")
    if "source" in result:
        print(f"source     {result['source']} ({result.get('staged_from', 'no C')})")
        print(f"retail asm {result['retail_asm']}")
    if "error" in result:
        print(f"\ncheck-unit: error: {result['error']}")
        return
    print(f"retail     {result['retail_bytes']} bytes")
    print(f"\nplaced bytes: {result['verdict']}")
    if result["ok"] and result["staged_from"].startswith("promoted"):
        print("\nAlready promoted and exact.")
        return
    if result["ok"]:
        print("\nObject matches (promotable). Delete the #ifndef NON_MATCHING / INCLUDE_ASM /"
              "\n#else lines and the #endif, keeping the C, then run `make overlays`.")
        return
    if result["differences"]:
        print("\nfirst differences")
        for line in result["differences"]:
            print(line)
    print("\nEdit the C body, then run this command again.")


def verify_all() -> int:
    """Every function in C under src/overlays must be byte-exact."""
    problem = ensure_configured(force=True)
    if problem:
        print(problem)
        return 2
    built = ninja()
    if built.returncode:
        print(built.stdout[-4000:] + built.stderr[-2000:])
        return 1
    from elftools.elf.elffile import ELFFile

    import overlay_proof as proof
    total, bad = 0, []
    for obj in sorted((BUILD / "c").glob("**/*.c.o")):
        with open(obj, "rb") as handle:
            symtab = ELFFile(handle).get_section_by_name(".symtab")
            names = [s.name for s in symtab.iter_symbols()
                     if is_overlay_name(s.name) and s["st_size"]
                     and s["st_info"]["type"] == "STT_FUNC"] if symtab else []
        for name in names:
            total += 1
            verdict = proof.check(obj, name)
            if not verdict["exact"]:
                bad.append(f"{name:24s} {verdict['verdict']}  ({obj.relative_to(BUILD)})")
    for line in bad:
        print(line)
    if bad:
        print(f"FAIL: {len(bad)} of {total} overlay functions in C do not match retail")
        return 1
    print(f"PASS: all {total} overlay functions in C match retail")
    return 0


if __name__ == "__main__":
    raise SystemExit(verify_all())
