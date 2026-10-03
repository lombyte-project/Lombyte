#!/usr/bin/env python3
"""Shared configuration and source helpers for the contribution scripts.

No command-line interface; ``list-functions.py`` and ``check-unit.py`` import
it.  The configured-unit row format (``- [0xADDR, c, owner]``) and the category
file are the same contract as ``scripts/generate_treemap.py``, which keeps its
own parser so it stays standalone.
"""

from __future__ import annotations

import json
import os
import re
from pathlib import Path

ROW_RE = re.compile(
    r"^\s*-\s*\[(0x[0-9A-Fa-f]+)\s*,\s*([A-Za-z_][A-Za-z0-9_]*)\s*,\s*([^\]]+?)\s*\]\s*$"
)
# Assembly-backed units keep the retail oracle under `#ifndef NON_MATCHING`
# and the readable C body under `#else`; see src/assembly/README.md.
GUARD_RE = re.compile(r"^#ifndef\s+NON_MATCHING\s*$")
FUNC_DEF_RE = re.compile(
    r"(?m)^[A-Za-z_][A-Za-z0-9_ \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{"
)
INCLUDE_ASM_RE = re.compile(r'INCLUDE_ASM\s*\(\s*"[^"]*"\s*,\s*([A-Za-z_]\w*)\s*\)')
ADDR_SYMBOL_RE = re.compile(r"^(?:FUN_|func_|D_|DAT_)[0-9A-Fa-f]+$")

CONFIG_PATH = Path("config") / "us" / "rnc1.us.yaml"
CATEGORY_PATH = Path("config") / "us" / "unit_categories.json"
RECOVERED_PATH = Path("config") / "us" / "recovered_names.json"


def parse_config_rows(config: Path) -> list[tuple[int, str, str]]:
    """Configured rows as (address, kind, owner), sorted by address."""
    rows = []
    for line in config.read_text(errors="replace").splitlines():
        match = ROW_RE.match(line)
        if match:
            rows.append((int(match.group(1), 16), match.group(2), match.group(3)))
    rows.sort(key=lambda row: row[0])
    return rows


def configured_units(config: Path) -> list[dict]:
    """Configured C units as dicts with owner, address and byte size."""
    rows = parse_config_rows(config)
    units = []
    for index, (address, kind, owner) in enumerate(rows):
        if kind != "c":
            continue
        end = rows[index + 1][0] if index + 1 < len(rows) else address
        size = end - address
        if size > 0:
            units.append({"owner": owner, "address": address, "size": size})
    return units


def load_categories(path: Path | None) -> tuple[set[str], set[str]]:
    """Return (exact under assembly, intentional asm) owner sets."""
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


def source_symbol(source: Path) -> str | None:
    """First function name (or INCLUDE_ASM symbol) in a unit source, or None.

    A non-static definition wins: units often open with static inline helpers
    (``IsNaN`` in the SDK float code) that are not the unit's own function.
    """
    text = source.read_text(errors="replace")
    matches = list(FUNC_DEF_RE.finditer(text))
    for match in matches:
        if not re.match(r"\s*static\b", match.group(0)):
            return match.group(1)
    if matches:
        return matches[0].group(1)
    match = INCLUDE_ASM_RE.search(text)
    return match.group(1) if match else None


def display_for(owner: str, source: Path, recovered: dict[str, str]) -> str | None:
    """Evidence-backed display name for a unit, or None."""
    if source.is_file():
        name = source_symbol(source)
        if name and not ADDR_SYMBOL_RE.match(name):
            return demangle_cfront(name)
    name = recovered.get(owner) or recovered.get(owner.removeprefix("assembly/"))
    return demangle_cfront(name) if name else None


def unit_source(repo: Path, owner: str) -> Path:
    """Source file path for a configured unit owner."""
    return repo / "src" / f"{owner}.c"


UNIT_NAME_RE = re.compile(r"^[A-Za-z0-9_][A-Za-z0-9_./-]*$")


def unsafe_unit_name(name: str) -> bool:
    """True when a configured unit or symbol name may not be used in paths.

    Build scripts derive file paths and shell command fragments from unit
    names. Only the project's ``[A-Za-z0-9_./-]`` alphabet is accepted, and
    no path segment may be ``..``.
    """
    candidate = str(name).strip()
    if not UNIT_NAME_RE.match(candidate):
        return True
    parts = [part for part in candidate.split("/") if part not in ("", ".")]
    return not parts or any(part == ".." for part in parts)


def default_workspace(repo: Path) -> Path:
    """Baseline workspace used when none is configured.

    ``BASELINE_ROOT`` wins when set; otherwise the project-local
    ``build/baseline`` created by ``./verify-baseline.sh`` is used, so the
    contribution scripts never touch a location outside the checkout unless
    the user points them there explicitly.
    """
    override = os.environ.get("BASELINE_ROOT", "").strip()
    if override:
        return Path(override).expanduser()
    return repo / "build" / "baseline"


def path_inside(path: Path, root: Path) -> bool:
    """True when ``path`` resolves inside ``root`` (checked before writing)."""
    try:
        Path(path).resolve().relative_to(Path(root).resolve())
        return True
    except ValueError:
        return False


def oracle_fallback_units(workspace: Path) -> set[str]:
    """Units this workspace rebuilds from the retail oracle instead of their C."""
    path = Path(workspace) / "config" / "us" / "oracle-fallback-units.json"
    if not path.is_file():
        return set()
    try:
        payload = json.loads(path.read_text())
    except (OSError, json.JSONDecodeError):
        return set()
    units = payload.get("units")
    if not isinstance(units, list):
        return set()
    return {str(unit) for unit in units}


def workspace_problem(workspace: Path, repo: Path | None = None) -> str | None:
    """Describe why ``workspace`` cannot be used, or return None.

    Refuses the filesystem root, the user's home, and the checkout itself or
    any of its parents: those can only be damaged, never built.
    """
    resolved = Path(workspace).resolve()
    if resolved == Path(resolved.anchor) or resolved == Path.home():
        return f"refusing to use {resolved} as a baseline workspace"
    if repo is not None:
        checkout = Path(repo).resolve()
        if resolved == checkout or resolved in checkout.parents:
            return (
                f"refusing to use {resolved} as a baseline workspace: it is the "
                "checkout or one of its parents"
            )
    if not (resolved / ".rnc-baseline-root").is_file():
        return (
            f"{resolved} is not a baseline workspace; run ./verify-baseline.sh "
            "once (or pass --workspace)"
        )
    project = resolved / "config" / "us"
    objdiff = resolved / "tools" / "objdiff" / "objdiff-cli"
    if not (project / "build.ninja").is_file() or not objdiff.is_file():
        return f"{resolved} is incomplete; re-run ./verify-baseline.sh"
    return None


# A unit counts as C_EXACT only when its source is C: inline asm is allowed
# solely as a label binding a declaration to its linked name
# (`__asm__("FUN_00202d10")`). Any other asm in the file (instructions, empty
# memory barriers, `.extern` directives, register pins) keeps the unit pending
# however exact its bytes are. include/qcopy.h and include/qzero.h are the approved exceptions and
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


def classify_units(repo: Path) -> list[dict]:
    """Every configured C unit with category, source path and display name.

    Categories follow scripts/generate_treemap.py: ``exact`` (matching C),
    ``asm`` (intentional low-level asm, excluded from the C goal) and
    ``pending`` (C still missing or not byte-exact).
    """
    exact_assembly, intentional = load_categories(repo / CATEGORY_PATH)
    recovered = load_recovered_full(repo / RECOVERED_PATH.parent)
    units = []
    for unit in configured_units(repo / CONFIG_PATH):
        owner = unit["owner"]
        source = unit_source(repo, owner)
        if owner in exact_assembly or (
            not owner.startswith("assembly/") and source.is_file()
            and not non_label_asm(source)
        ):
            category = "exact"
        elif owner in intentional:
            category = "asm"
        else:
            category = "pending"
        units.append(
            {
                **unit,
                "source": source,
                "category": category,
                "display": display_for(owner, source, recovered),
            }
        )
    return units


def split_oracle_guard(text: str) -> tuple[bool, str | None]:
    """Split a source file at the ``#ifndef NON_MATCHING`` oracle guard.

    Returns ``(uses_guard, c_body)``:

    * ``(False, None)`` - plain source without a guard (already pure C, or an
      assembly-only file that has no C body to measure);
    * ``(True, None)`` - guarded oracle without a ``#else`` C body;
    * ``(True, str)`` - guarded oracle with the readable C body.
    """
    lines = text.splitlines(keepends=True)
    start = next(
        (index for index, line in enumerate(lines) if GUARD_RE.match(line.strip())),
        None,
    )
    if start is None:
        return False, None
    depth = 0
    body_start = None
    for index in range(start, len(lines)):
        stripped = lines[index].strip()
        if not stripped.startswith("#"):
            continue
        directive = stripped[1:].split(None, 1)[0] if len(stripped) > 1 else ""
        if directive in ("if", "ifdef", "ifndef"):
            depth += 1
        elif directive == "endif":
            depth -= 1
            if depth == 0:
                if body_start is not None:
                    return True, "".join(lines[body_start:index])
                return True, None
        elif directive in ("else", "elif") and depth == 1:
            body_start = index + 1 if directive == "else" else None
    return True, None
