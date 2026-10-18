"""Shared logical grouping and committed-score helpers for progress views."""

from __future__ import annotations

import json
from pathlib import Path


def canonical_owner(owner: str) -> str:
    """Return the stable owner key, independent of the assembly source root."""
    return owner.removeprefix("assembly/")


def report_category_for_owner(owner: str) -> str:
    return "sdk" if canonical_owner(owner).split("/", 1)[0] in {"sdk", "kernel"} else "game"


def group_for_owner(owner: str) -> str:
    """Logical group of a unit: its source folder, or ``unclassified`` when unknown.

    A function moves to another group by moving its file to that folder.
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
    if root in {
        "runtime", "rendering", "gameplay", "audio", "video",
        "storage", "ui", "math", "input", "world",
    }:
        return "/".join(parts[:-1]) if len(parts) > 1 else root
    if root == "kernel":
        return "sdk/interrupts"
    return root


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
