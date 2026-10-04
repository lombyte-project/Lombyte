#!/usr/bin/env python3
"""Preview/apply the recovered-name proposals to the public game source tree.

Dry-run is the default. The tool reads config/us/recovered_names.json and can
move source files into logical subsystem folders, rename C function identifiers
while retaining their canonical assembler symbols with GNU asm labels, rename
proposed locals/parameters, rewrite C call sites to semantic identifiers while
retaining FUN_ linker symbols, and update configured unit paths. This temporary
migration tool is intended to be removed after the source migration is complete.
"""

from __future__ import annotations

import argparse
import difflib
import json
import os
import re
import stat
import sys
import tempfile
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

SCRIPT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_GAME_ROOT = SCRIPT_ROOT
MAP_REL = Path("config/us/recovered_names.json")
YAML_REL = Path("config/us/rnc1.us.yaml")
CATEGORIES_REL = Path("config/us/unit_categories.json")
EXPECTED_SCHEMA = "rnc-function-name-proposals-v1"
IDENT_RE = re.compile(r"^[a-z][a-z0-9_]*$")
OWNER_RE = re.compile(r"^[A-Za-z0-9_./-]+$")
FUNC_DEF_RE = re.compile(
    r"(?m)^[A-Za-z_][A-Za-z0-9_ \t*]*?\b"
    r"(?P<name>[A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{"
)
ALIAS_LINE_RE = re.compile(r"__attribute__\s*\(\(\s*alias\s*\(\s*\"([^\"]+)\"\s*\)\s*\)\)")
ASM_DECL_RE = re.compile(
    r'(?m)^[A-Za-z_][A-Za-z0-9_ \t*]*?\b(?P<name>[A-Za-z_]\w*)\s*'
    r'\([^;{}]*\)\s*__asm__\s*\(\s*"(?P<label>[^"]+)"\s*\)\s*;'
)
NON_MATCHING_RE = re.compile(r"^\s*#ifndef\s+NON_MATCHING\s*$")


@dataclass
class Candidate:
    entry: dict[str, Any]
    address: int
    current_name: str
    proposed_name: str
    old_owner: str
    new_owner: str
    old_source: Path
    new_source: Path
    already_applied: bool = False
    variable_renames: list[dict[str, Any]] = field(default_factory=list)
    error: str | None = None


@dataclass
class Move:
    candidate: Candidate
    content: str
    mode: int
    aliases_rebound: int = 0


@dataclass
class ReferenceEdit:
    path: Path
    original: str
    content: str
    mode: int
    references: int
    declarations: int


def safe_repo_path(root: Path, relative: str) -> Path:
    rel = Path(relative)
    if rel.is_absolute() or ".." in rel.parts:
        raise ValueError(f"unsafe repository path: {relative}")
    result = root / rel
    if not result.resolve().is_relative_to(root.resolve()):
        raise ValueError(f"path escapes game repository: {relative}")
    return result


def source_state_prefix(owner: str) -> str:
    """Keep assembly-backed units under their source-state directory."""
    if owner.startswith("assembly/textbin/"):
        return "assembly/textbin"
    if owner.startswith("assembly/"):
        return "assembly"
    return ""


def find_function_definition(text: str, name: str) -> re.Match[str] | None:
    pattern = re.compile(
        r"(?m)^[A-Za-z_][A-Za-z0-9_ \t*]*?\b(?P<name>"
        + re.escape(name)
        + r")\s*\([^;{}]*\)\s*\{"
    )
    matches = list(pattern.finditer(text))
    if len(matches) != 1:
        return None
    return matches[0]


def matching_brace(text: str, open_at: int) -> int:
    depth = 0
    i = open_at
    state = "code"
    while i < len(text):
        ch = text[i]
        nxt = text[i + 1] if i + 1 < len(text) else ""
        if state == "line_comment":
            if ch == "\n":
                state = "code"
        elif state == "block_comment":
            if ch == "*" and nxt == "/":
                state = "code"
                i += 1
        elif state in ("string", "char"):
            if ch == "\\":
                i += 1
            elif (state == "string" and ch == '"') or (state == "char" and ch == "'"):
                state = "code"
        else:
            if ch == "/" and nxt == "/":
                state = "line_comment"
                i += 1
            elif ch == "/" and nxt == "*":
                state = "block_comment"
                i += 1
            elif ch == '"':
                state = "string"
            elif ch == "'":
                state = "char"
            elif ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    return i
        i += 1
    raise ValueError("unclosed function body")


def replace_code_identifiers(
    text: str,
    renames: dict[str, str],
    *,
    skip_include_asm: bool = False,
    replacement_counts: dict[str, int] | None = None,
) -> tuple[str, int]:
    """Replace C identifier tokens outside comments and string/char literals."""
    if not renames:
        return text, 0
    lookup = dict(renames)
    out: list[str] = []
    changed = 0
    i = 0
    state = "code"
    while i < len(text):
        ch = text[i]
        nxt = text[i + 1] if i + 1 < len(text) else ""
        if state == "line_comment":
            out.append(ch)
            if ch == "\n":
                state = "code"
            i += 1
            continue
        if state == "block_comment":
            out.append(ch)
            if ch == "*" and nxt == "/":
                out.append(nxt)
                i += 2
                state = "code"
            else:
                i += 1
            continue
        if state in ("string", "char"):
            out.append(ch)
            if ch == "\\" and i + 1 < len(text):
                out.append(text[i + 1])
                i += 2
                continue
            if (state == "string" and ch == '"') or (state == "char" and ch == "'"):
                state = "code"
            i += 1
            continue
        if ch == "/" and nxt == "/":
            out.extend((ch, nxt))
            i += 2
            state = "line_comment"
            continue
        if ch == "/" and nxt == "*":
            out.extend((ch, nxt))
            i += 2
            state = "block_comment"
            continue
        if ch == '"':
            out.append(ch)
            i += 1
            state = "string"
            continue
        if ch == "'":
            out.append(ch)
            i += 1
            state = "char"
            continue
        if ch == "_" or ch.isalpha():
            j = i + 1
            while j < len(text) and (text[j] == "_" or text[j].isalnum()):
                j += 1
            token = text[i:j]
            if skip_include_asm and token == "INCLUDE_ASM":
                try:
                    close = signature_close(text, j)
                except ValueError:
                    close = -1
                if close >= 0:
                    out.append(text[i:close + 1])
                    i = close + 1
                    continue
            replacement = lookup.get(token)
            if replacement is None:
                out.append(token)
            else:
                out.append(replacement)
                changed += 1
                if replacement_counts is not None:
                    replacement_counts[token] = replacement_counts.get(token, 0) + 1
            i = j
            continue
        out.append(ch)
        i += 1
    return "".join(out), changed


def replace_scoped_identifiers(
    text: str, function_name: str, renames: dict[str, str]
) -> tuple[str, int]:
    """Rename locals/parameters only in the selected function declaration/body."""
    if not renames:
        return text, 0
    match = find_function_definition(text, function_name)
    if match is None:
        raise ValueError(f"cannot find unique definition of {function_name}")
    open_at = text.find("{", match.start(), match.end())
    close_at = matching_brace(text, open_at)
    start = match.start()
    end = close_at + 1
    updated, count = replace_code_identifiers(text[start:end], renames)
    return text[:start] + updated + text[end:], count


def c_body_span(text: str) -> tuple[int, int]:
    """Return the readable C branch span, or the full span for ordinary C."""
    lines = text.splitlines(keepends=True)
    offsets: list[int] = []
    offset = 0
    for line in lines:
        offsets.append(offset)
        offset += len(line)
    start = next(
        (index for index, line in enumerate(lines) if NON_MATCHING_RE.match(line)),
        None,
    )
    if start is None:
        return 0, len(text)

    depth = 0
    body_start: int | None = None
    for index in range(start, len(lines)):
        stripped = lines[index].strip()
        if not stripped.startswith("#"):
            continue
        directive = stripped[1:].split(None, 1)[0] if len(stripped) > 1 else ""
        if directive in {"if", "ifdef", "ifndef"}:
            depth += 1
        elif directive == "else" and depth == 1:
            body_start = offsets[index] + len(lines[index])
        elif directive == "endif":
            depth -= 1
            if depth == 0:
                if body_start is None:
                    raise ValueError("assembly-backed source has no NON_MATCHING C branch")
                return body_start, offsets[index]
    raise ValueError("unterminated NON_MATCHING guard")


def signature_close(text: str, after_name: int) -> int:
    """Find the closing parenthesis for a function declarator."""
    open_at = text.find("(", after_name)
    if open_at < 0:
        raise ValueError("function declarator has no parameter list")
    depth = 0
    state = "code"
    index = open_at
    while index < len(text):
        char = text[index]
        following = text[index + 1] if index + 1 < len(text) else ""
        if state == "line_comment":
            if char == "\n":
                state = "code"
        elif state == "block_comment":
            if char == "*" and following == "/":
                state = "code"
                index += 1
        elif state in {"string", "char"}:
            if char == "\\":
                index += 1
            elif (state == "string" and char == '"') or (state == "char" and char == "'"):
                state = "code"
        elif char == "/" and following == "/":
            state = "line_comment"
            index += 1
        elif char == "/" and following == "*":
            state = "block_comment"
            index += 1
        elif char == '"':
            state = "string"
        elif char == "'":
            state = "char"
        elif char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
            if depth == 0:
                return index
        index += 1
    raise ValueError("unclosed function parameter list")


def preserve_semantic_alias(
    text: str, semantic_name: str, canonical_name: str
) -> tuple[str, int]:
    """Keep a pre-existing linker alias under its existing assembler name."""
    lines = text.splitlines(keepends=True)
    kept: list[str] = []
    rebound = 0
    for line in lines:
        alias = ALIAS_LINE_RE.search(line)
        if alias and alias.group(1) == canonical_name:
            attr_start = line.find("__attribute__")
            names = list(re.finditer(r"\b([A-Za-z_]\w*)\b", line[:attr_start]))
            if names and names[-1].group(1) == semantic_name:
                suffix = canonical_name.removeprefix("FUN_").lower()
                link_name = f"rnc_link_alias_{suffix}"
                line = line[:names[-1].start()] + link_name + line[names[-1].end():]
                attr_start = line.find("__attribute__")
                line = (
                    line[:attr_start].rstrip()
                    + f' __asm__("{semantic_name}") '
                    + line[attr_start:]
                )
                rebound += 1
        kept.append(line)
    return "".join(kept), rebound


def function_asm_label(text: str, function_name: str) -> str | None:
    labels = {
        match.group("label")
        for match in ASM_DECL_RE.finditer(text)
        if match.group("name") == function_name
    }
    if len(labels) > 1:
        raise ValueError(f"conflicting asm labels for {function_name}: {sorted(labels)}")
    return next(iter(labels)) if labels else None


def rewrite_function_alias_references(
    text: str, aliases: dict[str, str]
) -> tuple[str, int, int, dict[str, int], dict[str, int], list[str]]:
    """Use semantic C identifiers while binding declarations to FUN_ symbols."""
    lines = text.splitlines(keepends=True)
    declarations: dict[str, int] = {}
    declaration_updates = 0
    issues: list[str] = []
    aliases_by_identifier: dict[str, list[tuple[str, str]]] = {}
    for canonical, semantic in aliases.items():
        for identifier in {canonical, semantic}:
            aliases_by_identifier.setdefault(identifier, []).append(
                (canonical, semantic)
            )
    function_identifier = re.compile(r"\b([A-Za-z_]\w*)\s*\(")

    for index, line in enumerate(lines):
        if not re.match(r"^[ \t]*extern\b", line):
            continue
        found_by_canonical: dict[str, tuple[str, str, str]] = {}
        for match in function_identifier.finditer(line):
            identifier = match.group(1)
            for canonical, semantic in aliases_by_identifier.get(identifier, ()):
                found_by_canonical.setdefault(
                    canonical, (canonical, semantic, identifier)
                )
        found = list(found_by_canonical.values())
        if not found:
            continue
        if len(found) > 1:
            issues.append("one extern line declares multiple mapped functions")
            continue

        canonical, semantic, identifier = found[0]
        semicolon = line.find(";")
        if semicolon < 0 or not re.search(
            r"\b" + re.escape(identifier) + r"\s*\([^;\n]*\)", line[:semicolon]
        ):
            issues.append(f"cannot parse extern declaration for {canonical}")
            continue

        rewritten, _ = replace_code_identifiers(
            line, {canonical: semantic}
        )
        rewritten_semicolon = rewritten.find(";")
        if rewritten_semicolon < 0:
            issues.append(f"cannot find extern terminator for {canonical}")
            continue
        existing_labels = re.findall(
            r'__asm__\s*\(\s*"([^"]+)"\s*\)',
            rewritten[:rewritten_semicolon],
        )
        if existing_labels and existing_labels != [canonical]:
            issues.append(
                f"extern {semantic} binds to {existing_labels}, expected {canonical}"
            )
            continue
        if not existing_labels:
            rewritten = (
                rewritten[:rewritten_semicolon].rstrip()
                + f' __asm__("{canonical}")'
                + rewritten[rewritten_semicolon:]
            )
        if rewritten != line:
            declaration_updates += 1
        lines[index] = rewritten
        declarations[canonical] = declarations.get(canonical, 0) + 1

    declared_text = "".join(lines)
    reference_counts: dict[str, int] = {}
    updated, reference_count = replace_code_identifiers(
        declared_text,
        aliases,
        skip_include_asm=True,
        replacement_counts=reference_counts,
    )
    return (
        updated,
        reference_count,
        declaration_updates,
        declarations,
        reference_counts,
        issues,
    )


def plan_function_reference_aliases(
    root: Path, candidates: list[Candidate], moves: list[Move]
) -> tuple[list[ReferenceEdit], int, int, int, list[str]]:
    """Plan semantic call-site identifiers across source and public headers."""
    aliases: dict[str, str] = {}
    issues: list[str] = []
    for candidate in candidates:
        if candidate.error:
            continue
        previous = aliases.get(candidate.current_name)
        if previous is not None and previous != candidate.proposed_name:
            issues.append(
                f"{candidate.current_name} maps to multiple semantic names"
            )
        aliases[candidate.current_name] = candidate.proposed_name
    if not aliases:
        return [], 0, 0, 0, issues

    move_by_source = {move.candidate.old_source: move for move in moves}
    source_roots = [root / "src", root / "include"]
    source_files = sorted(
        path
        for source_root in source_roots
        if source_root.is_dir()
        for path in source_root.rglob("*")
        if path.is_file() and path.suffix in {".c", ".h"}
    )
    edits: list[ReferenceEdit] = []
    file_results: list[tuple[Path, str, dict[str, int], dict[str, int]]] = []
    total_references = 0
    total_declarations = 0
    updated_files = 0

    for path in source_files:
        move = move_by_source.get(path)
        if move is not None:
            original = path.read_text(encoding="utf-8")
            source_text = move.content
        else:
            original = path.read_text(encoding="utf-8")
            source_text = original

        (
            updated,
            references,
            declaration_updates,
            declarations,
            reference_counts,
            file_issues,
        ) = rewrite_function_alias_references(source_text, aliases)
        issues.extend(f"{path}: {issue}" for issue in file_issues)
        total_references += references
        total_declarations += declaration_updates
        if updated != source_text:
            updated_files += 1
            if move is not None:
                move.content = updated
            else:
                edits.append(ReferenceEdit(
                    path=path,
                    original=original,
                    content=updated,
                    mode=stat.S_IMODE(path.stat().st_mode),
                    references=references,
                    declarations=declaration_updates,
                ))
        file_results.append((path, updated, declarations, reference_counts))

    header_declarations = {
        canonical
        for path, _text, declarations, _references in file_results
        if path.suffix == ".h"
        for canonical, count in declarations.items()
        if count
    }
    for path, text, declarations, reference_counts in file_results:
        if path.suffix != ".c":
            continue
        for canonical, count in reference_counts.items():
            if not count or declarations.get(canonical) or canonical in header_declarations:
                continue
            semantic = aliases[canonical]
            try:
                label = function_asm_label(text, semantic)
            except ValueError as exc:
                issues.append(f"{path}: {exc}")
                continue
            if label != canonical:
                issues.append(
                    f"{path}: {semantic} references {canonical} without an asm-labeled declaration"
                )

    return edits, total_references, total_declarations, updated_files, issues


def rename_function_with_asm_label(
    text: str, canonical_name: str, semantic_name: str
) -> tuple[str, int]:
    """Rename the C identifier but keep the canonical external symbol."""
    start, end = c_body_span(text)
    body = text[start:end]
    body, _ = replace_code_identifiers(body, {canonical_name: semantic_name})
    definition = find_function_definition(body, semantic_name)
    if definition is None:
        raise ValueError(f"cannot find unique {canonical_name} C definition")

    existing_label = function_asm_label(body, semantic_name)
    if existing_label is not None and existing_label != canonical_name:
        raise ValueError(
            f"{semantic_name} already has asm label {existing_label}, expected {canonical_name}"
        )
    if existing_label is None:
        close = signature_close(body, definition.end("name"))
        declaration = body[definition.start():close + 1].strip()
        asm_declaration = f'{declaration} __asm__("{canonical_name}");\n\n'
        body = body[:definition.start()] + asm_declaration + body[definition.start():]

    body, aliases_rebound = preserve_semantic_alias(body, semantic_name, canonical_name)
    return text[:start] + body + text[end:], aliases_rebound


def load_entries(payload: dict[str, Any], include_review: bool) -> list[dict[str, Any]]:
    proposals = payload.get("rename_proposals")
    if not isinstance(proposals, dict) or proposals.get("schema") != EXPECTED_SCHEMA:
        raise ValueError(f"rename_proposals schema must be {EXPECTED_SCHEMA}")
    entries = proposals.get("entries")
    if not isinstance(entries, list):
        raise ValueError("rename_proposals.entries must be an array")
    accepted = {"proposed"}
    if include_review:
        accepted.add("needs_review")
    selected = [
        e for e in entries
        if isinstance(e, dict)
        and e.get("status") in accepted
        and isinstance(e.get("proposed_name"), str)
        and e.get("proposed_name")
    ]
    return sorted(selected, key=lambda e: int(str(e.get("address", "0")), 16))


def build_candidates(
    root: Path, entries: list[dict[str, Any]], limit: int | None
) -> list[Candidate]:
    if limit is not None:
        entries = entries[:limit]
    result: list[Candidate] = []
    for entry in entries:
        try:
            current = str(entry["current_name"])
            proposed = str(entry["proposed_name"])
            address = int(str(entry["address"]), 16)
            source_path = str(entry["source_path"])
            if not IDENT_RE.fullmatch(proposed):
                raise ValueError(f"invalid lower_snake_case identifier: {proposed}")
            if not re.fullmatch(r"FUN_[0-9A-Fa-f]{8}", current):
                raise ValueError(f"unsupported current function name: {current}")
            if not source_path.startswith("src/") or not source_path.endswith(".c"):
                raise ValueError(f"unsupported source_path: {source_path}")
            old_owner = source_path[len("src/"):-2]
            if not OWNER_RE.fullmatch(old_owner) or ".." in Path(old_owner).parts:
                raise ValueError(f"unsafe source owner path: {old_owner}")
            reported_unit = str(entry.get("unit", ""))
            if reported_unit not in {old_owner, old_owner.removeprefix("assembly/")}:
                raise ValueError(
                    f"unit/source_path disagreement: {reported_unit} vs {old_owner}"
                )
            state_prefix = source_state_prefix(old_owner)
            group = str(entry.get("logical_group", ""))
            group_parts = Path(group).parts
            if (
                not group
                or Path(group).is_absolute()
                or ".." in group_parts
                or any(not re.fullmatch(r"[a-z][a-z0-9_]*", part) for part in group_parts)
                or group_parts[0] in {"ee", "vu"}
            ):
                raise ValueError(f"invalid logical_group: {group}")
            if old_owner.startswith("textbin/") and group == "unclassified":
                new_owner = f"textbin/{proposed}"
            else:
                new_owner = "/".join(
                    part for part in (state_prefix, group, proposed) if part
                )
            identifier_renames = entry.get("identifier_renames", [])
            if not isinstance(identifier_renames, list):
                raise ValueError("identifier_renames must be an array")
            old_source = safe_repo_path(root, source_path)
            new_source = safe_repo_path(root, f"src/{new_owner}.c")
            result.append(
                Candidate(
                    entry=entry,
                    address=address,
                    current_name=current,
                    proposed_name=proposed,
                    old_owner=old_owner,
                    new_owner=new_owner,
                    old_source=old_source,
                    new_source=new_source,
                    variable_renames=identifier_renames,
                )
            )
        except Exception as exc:
            dummy = Candidate(
                entry=entry,
                address=0,
                current_name=str(entry.get("current_name", "?")),
                proposed_name=str(entry.get("proposed_name", "?")),
                old_owner=str(entry.get("unit", "?")),
                new_owner="?",
                old_source=root,
                new_source=root,
                error=str(exc),
            )
            result.append(dummy)
    return result


def find_stale_route_tables(
    root: Path, candidates: list[Candidate]
) -> dict[str, list[tuple[str, str]]]:
    """Path-keyed unit sets in configure.py that a planned move invalidates.

    configure.py holds hand-curated sets of linker-config owners - the compile
    rule, the flag sets, the origin sets - and the owner's *path* is the key. A
    rename that moves a unit out of textbin/ therefore drops it out of whichever
    set it belonged to, and the unit gets rebuilt by a different compiler. The
    bytes change and the gate fails, with nothing in this tool's output naming
    the cause. These sets carry per-unit comments explaining why each unit is
    there, so they are reported rather than rewritten: the caller knows the new
    name and the reasoning does not survive an automatic edit.
    """
    path = root / "configure.py"
    if not path.is_file():
        return {}
    text = path.read_text(encoding="utf-8", errors="replace")
    renames = {
        c.old_owner: c.new_owner
        for c in candidates
        if not c.error and not c.already_applied and c.old_owner != c.new_owner
    }
    if not renames:
        return {}
    stale: dict[str, list[tuple[str, str]]] = {}
    for match in re.finditer(r"(?m)^([A-Z][A-Z0-9_]*) = \{(.*?)^\}", text, re.S):
        set_name, body = match.group(1), match.group(2)
        for owner in re.findall(r'"([^"]+)"', body):
            if owner in renames:
                stale.setdefault(set_name, []).append((owner, renames[owner]))
    return stale


def collect_symbol_definitions(
    src_root: Path,
) -> tuple[
    dict[str, list[Path]],
    dict[str, list[tuple[Path, str]]],
    dict[str, list[Path]],
]:
    definitions: dict[str, list[Path]] = {}
    aliases: dict[str, list[tuple[Path, str]]] = {}
    asm_labels: dict[str, list[Path]] = {}
    for path in src_root.rglob("*"):
        if path.suffix not in {".c", ".h"} or not path.is_file():
            continue
        text = path.read_text(encoding="utf-8", errors="replace")
        for match in FUNC_DEF_RE.finditer(text):
            definitions.setdefault(match.group("name"), []).append(path)
        for match in ASM_DECL_RE.finditer(text):
            # An extern declaration with an asm label is a *reference* to the
            # symbol, not a second emitter of it. A promoted sibling that calls
            # a still-pending unit declares the unit's retail symbol this way,
            # so counting it as a duplicate would refuse every rename the tree
            # layout makes legitimate.
            if re.match(r"(?m)extern\b", match.group(0)):
                continue
            asm_labels.setdefault(match.group("label"), []).append(path)
        for line in text.splitlines():
            if "alias" not in line:
                continue
            match = ALIAS_LINE_RE.search(line)
            if not match:
                continue
            declarator = line[:match.start()]
            asm_at = declarator.find("__asm__")
            if asm_at >= 0:
                declarator = declarator[:asm_at]
            declarator = re.sub(r'"(?:\\.|[^"\\])*"', '""', declarator)
            ident_matches = list(re.finditer(r"\b([A-Za-z_]\w*)\b", declarator))
            if not ident_matches:
                continue
            name = ident_matches[-1].group(1)
            aliases.setdefault(name, []).append((path, match.group(1)))
    return definitions, aliases, asm_labels


def inspect_candidates(
    root: Path, candidates: list[Candidate]
) -> tuple[list[Candidate], list[Move], list[str]]:
    src_root = root / "src"
    definitions, aliases, asm_labels = collect_symbol_definitions(src_root)
    issues: list[str] = []
    active: list[Candidate] = []
    moves: list[Move] = []
    destinations: dict[Path, Candidate] = {}
    proposed_owner = {c.old_owner: c for c in candidates if not c.error}

    for candidate in candidates:
        if candidate.error:
            issues.append(f"{candidate.current_name}: {candidate.error}")
            continue
        old_exists = candidate.old_source.is_file()
        new_exists = candidate.new_source.is_file()
        if old_exists and new_exists and candidate.old_source != candidate.new_source:
            candidate.error = f"destination already exists: {candidate.new_source}"
        elif old_exists and new_exists:
            text = candidate.old_source.read_text(encoding="utf-8", errors="replace")
            if (
                find_function_definition(text, candidate.proposed_name) is not None
                and function_asm_label(text, candidate.proposed_name) == candidate.current_name
            ):
                candidate.already_applied = True
            elif find_function_definition(text, candidate.current_name) is None:
                candidate.error = f"no unique {candidate.current_name} definition in source"
        elif not old_exists and new_exists:
            text = candidate.new_source.read_text(encoding="utf-8", errors="replace")
            if find_function_definition(text, candidate.proposed_name) is None:
                candidate.error = "destination exists but semantic function is absent"
            elif function_asm_label(text, candidate.proposed_name) != candidate.current_name:
                candidate.error = "destination does not bind the semantic function to its canonical asm symbol"
            else:
                candidate.already_applied = True
        elif not old_exists:
            candidate.error = f"source file not found: {candidate.old_source}"
        else:
            text = candidate.old_source.read_text(encoding="utf-8", errors="replace")
            if find_function_definition(text, candidate.current_name) is None:
                if (
                    find_function_definition(text, candidate.proposed_name) is not None
                    and function_asm_label(text, candidate.proposed_name) == candidate.current_name
                ):
                    candidate.error = "function rename is present, but its source path/config row is still old"
                else:
                    candidate.error = f"no unique {candidate.current_name} definition in source"
            if candidate.new_source in destinations:
                candidate.error = f"duplicate target path: {candidate.new_source}"
            else:
                destinations[candidate.new_source] = candidate

        if candidate.error:
            issues.append(f"{candidate.current_name}: {candidate.error}")
            continue
        if candidate.already_applied:
            active.append(candidate)
            continue

        duplicate_definitions = [
            path for path in definitions.get(candidate.current_name, [])
            if path not in {candidate.old_source, candidate.new_source}
        ]
        if duplicate_definitions:
            candidate.error = "canonical function is also defined in: " + ", ".join(
                str(path) for path in duplicate_definitions
            )
        duplicate_asm_labels = [
            path for path in asm_labels.get(candidate.current_name, [])
            if path not in {candidate.old_source, candidate.new_source}
        ]
        if duplicate_asm_labels:
            candidate.error = "canonical asm label is already declared in: " + ", ".join(
                str(path) for path in duplicate_asm_labels
            )

        name_defs = definitions.get(candidate.proposed_name, [])
        other_defs = [
            p for p in name_defs
            if p != candidate.old_source and p != candidate.new_source
        ]
        if other_defs:
            candidate.error = "semantic name already has a function definition: " + ", ".join(
                str(p) for p in other_defs
            )
        name_aliases = aliases.get(candidate.proposed_name, [])
        conflicting_aliases = [
            (p, target) for p, target in name_aliases
            if not (p == candidate.old_source and target == candidate.current_name)
        ]
        if conflicting_aliases:
            candidate.error = "semantic name already aliases another symbol: " + ", ".join(
                f"{p} -> {target}" for p, target in conflicting_aliases
            )
        conflicting_asm_labels = [
            path for path in asm_labels.get(candidate.proposed_name, [])
            if path not in {candidate.old_source, candidate.new_source}
        ]
        if conflicting_asm_labels:
            candidate.error = "semantic linker name is already assigned in: " + ", ".join(
                str(path) for path in conflicting_asm_labels
            )
        if candidate.error:
            issues.append(f"{candidate.current_name}: {candidate.error}")
            continue

        source_text = candidate.old_source.read_text(encoding="utf-8")
        variable_renames: dict[str, str] = {}
        for rename in candidate.variable_renames:
            if not isinstance(rename, dict) or rename.get("status") != "proposed":
                continue
            if rename.get("kind") not in {"local_variable", "parameter"}:
                candidate.error = (
                    f"unsupported identifier_renames kind: {rename.get('kind')}"
                )
                break
            old = rename.get("current_name")
            new = rename.get("proposed_name")
            if not isinstance(old, str) or not isinstance(new, str) or not re.fullmatch(
                r"[A-Za-z_]\w*", old
            ) or not re.fullmatch(r"[A-Za-z_]\w*", new):
                candidate.error = "invalid local/parameter rename record"
                break
            variable_renames[old] = new
        if candidate.error:
            issues.append(f"{candidate.current_name}: {candidate.error}")
            continue
        if variable_renames:
            try:
                source_text, _ = replace_scoped_identifiers(
                    source_text, candidate.current_name, variable_renames
                )
            except ValueError as exc:
                candidate.error = str(exc)
                issues.append(f"{candidate.current_name}: {candidate.error}")
                continue

        expected_old_asm = f"config/us/expected/asm/{candidate.old_owner}/"
        expected_new_asm = f"config/us/expected/asm/{candidate.new_owner}/"
        source_text = source_text.replace(expected_old_asm, expected_new_asm)
        try:
            source_text, aliases_rebound = rename_function_with_asm_label(
                source_text, candidate.current_name, candidate.proposed_name
            )
        except ValueError as exc:
            candidate.error = str(exc)
            issues.append(f"{candidate.current_name}: {candidate.error}")
            continue
        try:
            mode = stat.S_IMODE(candidate.old_source.stat().st_mode)
        except OSError:
            mode = 0o644
        moves.append(Move(
            candidate=candidate,
            content=source_text,
            mode=mode,
            aliases_rebound=aliases_rebound,
        ))
        active.append(candidate)

    return active, moves, issues


def replace_config_owners(text: str, candidates: list[Candidate]) -> tuple[str, list[str]]:
    lines = text.splitlines(keepends=True)
    errors: list[str] = []
    row_re = re.compile(
        r"^(\s*-\s*\[\s*0x[0-9A-Fa-f]+\s*,\s*c\s*,\s*)"
        r"([^\]]+?)(\s*\]\s*)$"
    )
    for candidate in candidates:
        if candidate.already_applied or candidate.old_owner == candidate.new_owner:
            continue
        old_rows: list[tuple[int, re.Match[str]]] = []
        new_rows: list[tuple[int, re.Match[str]]] = []
        for index, line in enumerate(lines):
            match = row_re.match(line)
            if not match:
                continue
            owner = match.group(2).strip()
            if owner == candidate.old_owner:
                old_rows.append((index, match))
            elif owner == candidate.new_owner:
                new_rows.append((index, match))
        if len(old_rows) == 1 and not new_rows:
            index, match = old_rows[0]
            lines[index] = match.group(1) + candidate.new_owner + match.group(3)
        elif not old_rows and len(new_rows) == 1:
            errors.append(
                f"{candidate.current_name}: config already names {candidate.new_owner} "
                "while the source is still at its old path"
            )
        else:
            errors.append(
                f"{candidate.current_name}: expected one config row for "
                f"{candidate.old_owner}, found {len(old_rows)} old and "
                f"{len(new_rows)} target rows"
            )
    return "".join(lines), errors


def replace_category_owners(payload: Any, renames: dict[str, str]) -> int:
    changed = 0
    if isinstance(payload, dict):
        for key, value in payload.items():
            if isinstance(value, str) and value in renames:
                payload[key] = renames[value]
                changed += 1
            else:
                changed += replace_category_owners(value, renames)
    elif isinstance(payload, list):
        for index, value in enumerate(payload):
            if isinstance(value, str) and value in renames:
                payload[index] = renames[value]
                changed += 1
            else:
                changed += replace_category_owners(value, renames)
    return changed


def find_duplicate_catalog_units(payload: dict[str, Any]) -> list[str]:
    """Unit paths in which more than one rename proposal claims one function.

    ``update_catalog_owners`` rewrites the unit of *every* entry that shares an
    address with a moved unit, so a catalog that already held two rows for one
    function - one proposed, one left unresolved - ends up with both rows naming
    the same unit. ``progress_groups.py`` then refuses to load the catalog, and
    the progress workflow fails three files away from the cause. The invariant is
    one row per function of a unit (a unit that holds a whole translation unit
    has one row per function), and it is checked here so the failure names the
    rows instead of the symptom.
    """
    seen: dict[tuple[str, str], list[dict[str, Any]]] = {}
    entries = (payload.get("rename_proposals") or {}).get("entries") or []
    for entry in entries:
        if not isinstance(entry, dict):
            continue
        source = str(entry.get("source_path") or "")
        if not source:
            continue
        unit = (source[4:-2] if source.startswith("src/") else source[:-2]).removeprefix(
            "assembly/"
        )
        seen.setdefault((unit, str(entry.get("address") or "")), []).append(entry)
    return sorted(
        {unit for (unit, _), rows in seen.items() if len(rows) > 1}
    )


def update_catalog_owners(payload: dict[str, Any], candidates: list[Candidate]) -> int:
    """Keep proposal and recovered-symbol owner references aligned after moves."""
    selected = {candidate.address: candidate for candidate in candidates}
    owner_updates: dict[str, str] = {}
    for candidate in candidates:
        owner_updates[candidate.old_owner] = candidate.new_owner
        owner_updates[candidate.old_owner.removeprefix("assembly/")] = (
            candidate.new_owner.removeprefix("assembly/")
        )
    proposals = payload.get("rename_proposals", {})
    changed = 0
    for entry in proposals.get("entries", []):
        if not isinstance(entry, dict):
            continue
        try:
            address = int(str(entry.get("address", "0")), 16)
        except ValueError:
            continue
        candidate = selected.get(address)
        if candidate is None:
            continue
        new_unit = candidate.new_owner.removeprefix("assembly/")
        new_source = f"src/{candidate.new_owner}.c"
        if entry.get("unit") != new_unit or entry.get("source_path") != new_source:
            entry["unit"] = new_unit
            entry["source_path"] = new_source
            changed += 1
    for entry in payload.get("symbols", []):
        if not isinstance(entry, dict):
            continue
        old_unit = entry.get("unit")
        new_unit = owner_updates.get(old_unit) if isinstance(old_unit, str) else None
        if new_unit is not None and new_unit != old_unit:
            entry["unit"] = new_unit
            changed += 1
    if changed:
        payload["notes"] = (
            "Recovered original symbols and address-bound semantic function names. "
            "The symbols array preserves recovered names and match extents; "
            "rename_proposals records semantic names and logical groups as "
            "reference metadata outside the build configuration."
        )
    return changed


def print_diff(old_name: str, new_name: str, old: str, new: str) -> None:
    diff = difflib.unified_diff(
        old.splitlines(keepends=True),
        new.splitlines(keepends=True),
        fromfile=f"a/{old_name}",
        tofile=f"b/{new_name}",
    )
    sys.stdout.writelines(diff)


def apply_transaction(
    writes: dict[Path, tuple[str, int]], deletes: set[Path]
) -> None:
    affected = set(writes) | set(deletes)
    backups: dict[Path, tuple[bytes, int] | None] = {}
    for path in affected:
        if path.exists():
            backups[path] = (path.read_bytes(), stat.S_IMODE(path.stat().st_mode))
        else:
            backups[path] = None

    created_dirs: list[Path] = []
    staged: dict[Path, Path] = {}
    try:
        for target, (content, mode) in writes.items():
            missing: list[Path] = []
            parent = target.parent
            while not parent.exists():
                missing.append(parent)
                parent = parent.parent
            for directory in reversed(missing):
                directory.mkdir()
                created_dirs.append(directory)
            with tempfile.NamedTemporaryFile(
                mode="wb", prefix=".rnc-rename-", dir=target.parent, delete=False
            ) as handle:
                handle.write(content.encode("utf-8"))
                handle.flush()
                os.fsync(handle.fileno())
                os.chmod(handle.name, mode)
                staged[target] = Path(handle.name)
        for target, temp_path in staged.items():
            os.replace(temp_path, target)
        for path in deletes:
            if path.exists():
                path.unlink()
    except Exception:
        for temp_path in staged.values():
            try:
                temp_path.unlink(missing_ok=True)
            except OSError:
                pass
        for path, backup in backups.items():
            try:
                if backup is None:
                    path.unlink(missing_ok=True)
                else:
                    data, mode = backup
                    path.parent.mkdir(parents=True, exist_ok=True)
                    with tempfile.NamedTemporaryFile(
                        mode="wb", prefix=".rnc-rename-rollback-",
                        dir=path.parent, delete=False
                    ) as handle:
                        handle.write(data)
                        temp_restore = Path(handle.name)
                    os.chmod(temp_restore, mode)
                    os.replace(temp_restore, path)
            except OSError:
                pass
        for directory in reversed(created_dirs):
            try:
                directory.rmdir()
            except OSError:
                pass
        raise


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Preview or apply function-name/path proposals from recovered_names.json. "
            "Dry-run is the default."
        )
    )
    parser.add_argument(
        "--game-root",
        type=Path,
        default=Path(os.environ.get("RNC_GAME_ROOT", DEFAULT_GAME_ROOT)),
        help="public Lombyte checkout (default: this script's repository root)",
    )
    parser.add_argument("--include-needs-review", action="store_true")
    parser.add_argument("--limit", type=int, help="limit to the first N proposals by address")
    parser.add_argument(
        "--address",
        help="select one proposal by address, e.g. 0x0012d8f8",
    )
    parser.add_argument("--diff", action="store_true", help="show unified content diffs")
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--dry-run", action="store_true", help="preview only (the default)")
    mode.add_argument("--apply", action="store_true", help="write the planned changes")
    args = parser.parse_args()

    root = args.game_root.resolve()
    map_path = root / MAP_REL
    yaml_path = root / YAML_REL
    categories_path = root / CATEGORIES_REL
    if not (root / "src").is_dir() or not map_path.is_file():
        print(f"error: not a Lombyte game checkout: {root}", file=sys.stderr)
        return 2
    try:
        map_original = map_path.read_text(encoding="utf-8")
        payload = json.loads(map_original)
        proposals = payload.get("rename_proposals", {})
        if proposals.get("schema") != EXPECTED_SCHEMA:
            raise ValueError(f"expected rename schema {EXPECTED_SCHEMA}")
        entries = load_entries(payload, args.include_needs_review)
        if args.address:
            wanted = int(args.address, 16)
            entries = [
                entry for entry in entries
                if int(str(entry.get("address", "0")), 16) == wanted
            ]
            if not entries:
                raise ValueError(f"no selected proposal at {args.address}")
        candidates = build_candidates(root, entries, args.limit)
    except (OSError, json.JSONDecodeError, ValueError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    active, moves, issues = inspect_candidates(root, candidates)
    (
        reference_edits,
        reference_count,
        reference_declaration_count,
        reference_file_count,
        reference_issues,
    ) = plan_function_reference_aliases(root, active, moves)
    issues.extend(reference_issues)

    yaml_text = yaml_path.read_text(encoding="utf-8")
    updated_yaml, yaml_errors = replace_config_owners(yaml_text, active)
    issues.extend(yaml_errors)

    category_text: str | None = None
    category_count = 0
    if categories_path.is_file():
        category_payload = json.loads(categories_path.read_text(encoding="utf-8"))
        category_renames = {
            c.old_owner: c.new_owner
            for c in active
            if not c.already_applied and c.old_owner != c.new_owner
        }
        category_count = replace_category_owners(category_payload, category_renames)
        if category_count:
            category_text = json.dumps(category_payload, indent=2, ensure_ascii=False) + "\n"

    catalog_owner_count = update_catalog_owners(payload, active)
    duplicate_units = find_duplicate_catalog_units(payload)
    if duplicate_units:
        issues.append(
            "the catalog would name one unit from "
            f"{len(duplicate_units)} row(s): " + ", ".join(duplicate_units[:6])
        )
    if args.apply and not issues and active:
        proposals = payload["rename_proposals"]
        proposals["status"] = "applied"
        proposals.setdefault("naming", {})["source_changes"] = True
    map_text = json.dumps(payload, indent=2, ensure_ascii=False) + "\n"

    moved_candidates = [m.candidate for m in moves]
    moving_count = len(moved_candidates)
    variable_count = sum(len(c.variable_renames) for c in moved_candidates)
    aliases_rebound = sum(move.aliases_rebound for move in moves)

    print(f"Game repo: {root}")
    print(f"Mapping:   {map_path}")
    print(f"Mode:      {'APPLY' if args.apply else 'DRY RUN'}")
    print(
        f"Selected {len(candidates)} entries; planned {moving_count} source moves, "
        f"{moving_count} semantic C definitions with preserved FUN_ asm labels, "
        f"{aliases_rebound} pre-existing semantic aliases preserved, "
        f"{variable_count} variable/parameter proposals."
    )
    already = sum(c.already_applied for c in active)
    if already:
        print(f"Already applied: {already}")
    print("Linker policy: keep FUN_<address> as the emitted symbol via __asm__; use the proposed name in C.")
    print(
        f"C references planned for semantic aliases: {reference_count} references, "
        f"{reference_declaration_count} asm-labeled declarations, "
        f"{reference_file_count} files."
    )
    stale_route_tables = find_stale_route_tables(root, active)
    if stale_route_tables:
        print()
        print("UNITS MOVED OUT OF A PATH-KEYED SET IN configure.py - update it by hand:")
        for set_name, owners in stale_route_tables.items():
            print(f"  {set_name}: {len(owners)} owner(s) still named by their old path")
            for old_owner, new_owner in owners:
                print(f"    {old_owner} -> {new_owner}")
        print(
            "  These sets decide the compile rule by owner. An owner that leaves one "
            "silently changes compiler, and the baseline gate fails on the bytes."
        )
    print()

    for candidate in active:
        if candidate.already_applied:
            print(f"ALREADY 0x{candidate.address:08x} {candidate.proposed_name}")
            continue
        print(
            f"0x{candidate.address:08x} {candidate.current_name} -> "
            f"{candidate.proposed_name} [__asm__(\"{candidate.current_name}\")]"
        )
        print(f"  {candidate.old_source.relative_to(root)}")
        print(f"  -> {candidate.new_source.relative_to(root)}")
        print(f"  unit: {candidate.old_owner} -> {candidate.new_owner}")

    if issues:
        print()
        print(f"Conflicts: {len(issues)}")
        for issue in issues:
            print(f"  - {issue}")
        if args.apply:
            print("No files written. Resolve the conflicts and rerun.")
            return 2

    content_changes: list[tuple[str, str, str, str]] = []
    for move in moves:
        old = move.candidate.old_source.read_text(encoding="utf-8")
        content_changes.append(
            (
                str(move.candidate.old_source.relative_to(root)),
                str(move.candidate.new_source.relative_to(root)),
                old,
                move.content,
            )
        )
    for edit in reference_edits:
        content_changes.append(
            (
                str(edit.path.relative_to(root)),
                str(edit.path.relative_to(root)),
                edit.original,
                edit.content,
            )
        )
    if updated_yaml != yaml_text:
        content_changes.append(
            (str(YAML_REL), str(YAML_REL), yaml_text, updated_yaml)
        )
    if map_text != map_original:
        content_changes.append(
            (str(MAP_REL), str(MAP_REL), map_original, map_text)
        )
    if category_text is not None:
        content_changes.append(
            (
                str(CATEGORIES_REL),
                str(CATEGORIES_REL),
                categories_path.read_text(encoding="utf-8"),
                category_text,
            )
        )

    print()
    print(
        f"Content/config changes: {sum(old != new for _, _, old, new in content_changes)} "
        "files."
    )
    print(f"Unit-category strings updated: {category_count}")
    print(f"Catalog source-unit records updated: {catalog_owner_count}")
    if args.diff:
        for old_name, new_name, old, new in content_changes:
            print_diff(old_name, new_name, old, new)

    if not args.apply:
        print("\nDry run only; no files were written.")
        print("Use --apply to write this plan.")
        return 0
    if issues:
        return 2

    writes: dict[Path, tuple[str, int]] = {}
    deletes: set[Path] = set()
    for move in moves:
        writes[move.candidate.new_source] = (move.content, move.mode)
        # A rename in place (the file keeps its path) must not delete the
        # file it has just written.
        if move.candidate.old_source != move.candidate.new_source:
            deletes.add(move.candidate.old_source)
    for edit in reference_edits:
        writes[edit.path] = (edit.content, edit.mode)
    if updated_yaml != yaml_text:
        writes[yaml_path] = (updated_yaml, stat.S_IMODE(yaml_path.stat().st_mode))
    if map_text != map_original:
        writes[map_path] = (map_text, stat.S_IMODE(map_path.stat().st_mode))
    if category_text is not None:
        writes[categories_path] = (
            category_text,
            stat.S_IMODE(categories_path.stat().st_mode),
        )
    try:
        apply_transaction(writes, deletes)
    except Exception as exc:
        print(f"error applying changes; rollback attempted: {exc}", file=sys.stderr)
        return 1
    config_paths = {yaml_path, map_path}
    if categories_path.is_file():
        config_paths.add(categories_path)
    config_updates = sum(path in config_paths for path in writes)
    print(
        f"Applied {len(moves)} function source changes, updated "
        f"{len(reference_edits)} caller files, and wrote {config_updates} config files."
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
