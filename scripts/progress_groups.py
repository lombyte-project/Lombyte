"""Shared logical grouping and committed-score helpers for progress views."""

from __future__ import annotations

import json
import re
from pathlib import Path

NAME_PROPOSALS = Path("config") / "us" / "recovered_names.json"
NAME_PROPOSALS_SCHEMA = "rnc-function-name-proposals-v1"
GROUP_PART_RE = re.compile(r"[a-z][a-z0-9_]*$")
FUNCTION_NAME_RE = re.compile(r"[a-z][a-z0-9_]*$")


def canonical_owner(owner: str) -> str:
    """Return the stable owner key, independent of the assembly source root."""
    return owner.removeprefix("assembly/")


def report_category_for_owner(owner: str) -> str:
    return "sdk" if canonical_owner(owner).split("/", 1)[0] in {"sdk", "kernel"} else "game"


def fallback_group(owner: str) -> str:
    """Return a semantic source bucket, or ``unclassified`` when unknown.

    Address ranges made the progress map look more specific without adding
    useful meaning. Keep owners with no supported subsystem assignment in one
    honest bucket until symbols or call relationships justify a better group.
    """
    parts = canonical_owner(owner).split("/")
    root = parts[0]
    leaf = parts[-1]

    if root in {"ee", "vu", "asm", "core", "gs", "sys"}:
        # Legacy owners should not reintroduce architecture-based categories.
        return "unclassified"
    if root == "textbin":
        if len(parts) == 2:
            known_flat = {
                "fast_draw_quad_real": "rendering/geometry",
                "write_setup_block": "rendering/commands",
                "write_gs_header": "rendering/commands",
                "fast_dec_timer": "runtime/time",
                "fast_vec_dot": "math/vectors",
                "fast_vec_cross": "math/vectors",
                "fast_vector_from_packed_chars": "math/vectors",
                "fast_cos": "math/trigonometry",
                "fast_sin": "math/trigonometry",
                "fast_arc_sin": "math/trigonometry",
                "fast_tween_color": "rendering/color",
                "moby_anim_proc": "gameplay/animation",
                "part_proc": "gameplay/entities",
                "sky_sprite_proc": "rendering/sky",
                "tfrag_proc": "rendering/terrain",
                "build_tfrag_texture_dma": "rendering/terrain",
                "vblank_handler": "runtime/interrupts",
            }
            return known_flat.get(leaf, "unclassified")
        module_parts = parts[1:-1]
        if module_parts and module_parts[0] == "unclassified":
            known = {
                "load_irx_module": "runtime/modules",
                "init_mem_slots": "runtime/memory",
                "init_once": "runtime/startup",
            }
            return known.get(leaf, "unclassified")
        return "/".join(module_parts) if module_parts else "unclassified"
    if root == "sdk":
        if len(parts) == 2:
            return "sdk/library"
        return "/".join(parts[:-1])
    if root in {"runtime", "rendering", "gameplay", "audio", "video", "storage", "ui", "math"}:
        return "/".join(parts[:-1]) if len(parts) > 1 else root
    if root == "kernel":
        return "sdk/interrupts"
    return root


def load_group_assignments(repo: Path) -> dict[str, dict[str, str | None]]:
    """Load catalog groups and accepted display names, keyed by canonical owner.

    Group hints are usable regardless of name status: a function may have a
    useful subsystem assignment even when its semantic name is unresolved.
    The function name itself is only taken from entries marked ``proposed``.
    """
    path = repo / NAME_PROPOSALS
    if not path.is_file():
        return {}
    payload = json.loads(path.read_text())
    proposals = payload.get("rename_proposals") or {}
    if not proposals:
        return {}
    if proposals.get("schema") != NAME_PROPOSALS_SCHEMA:
        raise ValueError(f"unsupported recovered-name proposal schema in {path}")

    result: dict[str, dict[str, str | None]] = {}
    first_address: dict[str, int | None] = {}
    for entry in proposals.get("entries", []):
        if not isinstance(entry, dict):
            continue
        unit = entry.get("unit")
        source_path = entry.get("source_path")
        group = entry.get("logical_group")
        if not isinstance(unit, str) or not isinstance(source_path, str):
            raise ValueError(f"proposal at {entry.get('address')} has no source-unit owner")
        if not source_path.startswith("src/") or not source_path.endswith(".c"):
            raise ValueError(f"invalid source_path in proposal at {entry.get('address')}")
        unit_key = canonical_owner(unit)
        source_key = canonical_owner(source_path[len("src/"):-2])
        if unit_key != source_key:
            raise ValueError(
                f"unit/source_path disagreement in proposal at {entry.get('address')}: "
                f"{unit} vs {source_path}"
            )
        if not isinstance(group, str) or not group:
            raise ValueError(f"proposal at {entry.get('address')} has no logical_group")
        parts = group.split("/")
        if any(not GROUP_PART_RE.fullmatch(part) for part in parts):
            raise ValueError(f"invalid logical_group in proposal at {entry.get('address')}")
        if parts[0] in {"ee", "vu"}:
            raise ValueError(f"architecture root is not a logical group: {group}")

        # ``unclassified`` is an explicit lack of semantic evidence. Preserve
        # that label unless the source module supplies a useful broad fallback.
        if group == "unclassified":
            group = fallback_group(unit_key)

        name = entry.get("proposed_name")
        if entry.get("status") != "proposed" or not isinstance(name, str) or not FUNCTION_NAME_RE.fullmatch(name):
            name = None
        assignment = {"logical_group": group, "proposed_name": name}
        # A unit holding several functions of one translation unit has one
        # entry per function: they must agree on the group, and the unit takes
        # the name of its first function.
        address = parse_address(entry.get("address"))
        previous = result.get(unit_key)
        if previous is not None:
            if previous["logical_group"] != group:
                raise ValueError(f"multiple grouping entries for source unit {unit_key}")
            previous_address = first_address.get(unit_key)
            if address is None or previous_address is None:
                if previous != assignment:
                    raise ValueError(f"multiple grouping/name entries for source unit {unit_key}")
                continue
            if address == previous_address:
                raise ValueError(f"multiple entries for function {entry.get('address')} in {unit_key}")
            if address > previous_address:
                continue
        result[unit_key] = assignment
        first_address[unit_key] = address
    return result


def group_for_owner(owner: str, assignments: dict[str, dict[str, str | None]]) -> str:
    assignment = assignments.get(canonical_owner(owner))
    if assignment and assignment.get("logical_group"):
        return str(assignment["logical_group"])
    return fallback_group(owner)


def report_group_name(category: str, logical_group: str) -> str:
    """Namespace group names by report category to keep IDs globally unique."""
    if logical_group.startswith(f"{category}/"):
        return logical_group
    return f"{category}/{logical_group}"


def parse_address(value) -> int | None:
    if value is None:
        return None
    try:
        text = str(value)
        return int(text, 16) if text.lower().startswith("0x") else int(text, 10)
    except (TypeError, ValueError):
        return None


def committed_function_scores(path: Path) -> dict[object, float]:
    """Read per-function scores from ``fuzzy_scores.json``
    (``rnc-fuzzy-scores-v1``) or from a flat or grouped report v2 JSON."""
    if not path.is_file():
        return {}
    report = json.loads(path.read_text())
    scores: dict[object, float] = {}
    if "scores" in report:
        for key, value in report["scores"].items():
            address = parse_address(key)
            if address is not None:
                scores[address] = float(value)
        return scores
    for unit in report.get("units", []):
        functions = unit.get("functions") or []
        for function in functions:
            score = float(function.get("fuzzy_match_percent", 0.0))
            metadata = function.get("metadata") or {}
            address = parse_address(metadata.get("virtual_address"))
            if address is not None:
                scores[address] = score
        # Preserve compatibility with old one-function-per-unit snapshots.
        if len(functions) == 1 and unit.get("name"):
            scores[unit["name"]] = float(functions[0].get("fuzzy_match_percent", 0.0))
    return scores


def write_fuzzy_scores(path: Path, report: dict) -> int:
    """Keep the measured similarity of every partly matched function.

    The scores need a build to measure (the maintainers' tooling does it); everything else
    in the report is derived from the tree.
    """
    scores = {}
    for unit in report.get("units", []):
        for function in unit.get("functions") or []:
            score = float(function.get("fuzzy_match_percent", 0.0))
            address = parse_address((function.get("metadata") or {}).get("virtual_address"))
            if address is not None and 0.0 < score < 100.0:
                scores[f"0x{address:08x}"] = score
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps({"schema": "rnc-fuzzy-scores-v1",
                                "scores": dict(sorted(scores.items()))}, indent=1) + "\n",
                    encoding="utf-8")
    return len(scores)
