#!/usr/bin/env python3
"""Install stable raw-word oracles for textbin and FUN-labeled functions.

Splat's textbin disassembly is useful for analysis, but the frozen EE
compiler's inline assembler does not accept every VU/relocation spelling.
This helper leaves splat output intact and adds a stable ``FUN_*.s`` byte
oracle plus its expected object. Semantic source units that retain an explicit
FUN assembler label also get raw oracles, regardless of their source path.
Build-only: it reads the segment table and raw bytes from the retail ELF.
"""

from __future__ import annotations

import argparse
import csv
import functools
import json
import os
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rnc_units  # noqa: E402

PROJECT_ROOT = Path(__file__).resolve().parents[1]


def parse_main_subsegments(path: Path) -> list[dict[str, object]]:
    """Read the project's simple `main.subsegments` YAML without PyYAML."""
    boundary = re.compile(
        r"^(?P<indent>\s*)-\s*\[(?P<offset>0x[0-9a-fA-F]+|[0-9]+),\s*"
        r"(?P<kind>[A-Za-z0-9_]+),\s*(?P<name>[^\]]+)\]\s*$"
    )
    lines = path.read_text(errors="replace").splitlines()
    in_main = False
    main_start: int | None = None
    main_vram: int | None = None
    main_end: int | None = None
    entries: list[dict[str, object]] = []
    for line in lines:
        stripped = line.strip()
        indent = len(line) - len(line.lstrip())
        if stripped in {"- name: main", "name: main"}:
            in_main = True
            continue
        if in_main and indent <= 4 and stripped.startswith("-"):
            match = boundary.match(line)
            if match:
                main_end = int(match.group("offset"), 0)
            in_main = False
        if not in_main:
            continue
        if stripped.startswith("start:"):
            main_start = int(stripped.split(":", 1)[1].strip(), 0)
        elif stripped.startswith("vram:"):
            main_vram = int(stripped.split(":", 1)[1].strip(), 0)
        match = boundary.match(line)
        if match and len(match.group("indent")) >= 10:
            entries.append(
                {
                    "offset": int(match.group("offset"), 0),
                    "kind": match.group("kind"),
                    "name": match.group("name").strip(),
                }
            )
    if main_start is None or main_vram is None:
        return []
    entries.sort(key=lambda item: int(item["offset"]))
    ranges: list[dict[str, object]] = []
    for index, entry in enumerate(entries):
        next_offset = (
            int(entries[index + 1]["offset"]) if index + 1 < len(entries) else main_end
        )
        if next_offset is None:
            continue
        start = main_vram + int(entry["offset"]) - main_start
        end = main_vram + next_offset - main_start
        ranges.append({**entry, "start": start, "end": end, "size": end - start})
    return ranges


def _elf_file_bytes(path: Path, address: int, size: int) -> bytes:
    """Read a virtual-address range from a little-endian ELF PT_LOAD."""
    data = path.read_bytes()
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
        raise ValueError(f"expected 32-bit little-endian ELF: {path}")
    header = struct.unpack_from("<16sHHIIIIIHHHHHH", data, 0)
    phoff, phentsize, phnum = header[5], header[9], header[10]
    for index in range(phnum):
        values = struct.unpack_from("<IIIIIIII", data, phoff + index * phentsize)
        if values[0] != 1:
            continue
        _, offset, vaddr, _, filesz, _, _, _ = values
        if vaddr <= address and address + size <= vaddr + filesz:
            begin = offset + address - vaddr
            return data[begin : begin + size]
    raise ValueError(f"ELF range is not file-backed: 0x{address:08X}+0x{size:X}")


@functools.lru_cache(maxsize=None)
def _textbin_reference_aliases(source_root: Path) -> dict[int, tuple[str, ...]]:
    """Index address-shaped symbol references used by checked-in sources."""
    pattern = re.compile(r"\b(?:FUN|func|sub|D)_[0-9A-Fa-f]{8}\b")
    indexed: dict[int, set[str]] = {}
    if not source_root.is_dir():
        return {}
    for path in source_root.rglob("*"):
        if path.suffix not in {".c", ".h", ".s", ".inc"} or not path.is_file():
            continue
        try:
            text = path.read_text(errors="replace")
        except OSError:
            continue
        for symbol in pattern.findall(text):
            try:
                address = int(symbol.rsplit("_", 1)[1], 16)
            except ValueError:
                continue
            indexed.setdefault(address, set()).add(symbol)
    return {address: tuple(sorted(symbols)) for address, symbols in indexed.items()}


def _textbin_oracle_labels(source_root: Path, symbol: str, address: int) -> list[str]:
    # Pending wrappers include listings from config/us/expected/asm, and a
    # listing can call a function by a spelling no C source uses.
    aliases = set(_textbin_reference_aliases(source_root).get(address, ()))
    listings = source_root.parent / "config/us/expected/asm"
    aliases |= set(_textbin_reference_aliases(listings).get(address, ()))
    aliases = sorted(aliases)
    return [symbol] + [alias for alias in aliases if alias != symbol]


def _symbol_addrs(path: Path) -> dict[int, tuple[str, ...]]:
    """Retail names the splat configuration assigns to addresses."""
    names: dict[int, list[str]] = {}
    if not path.is_file():
        return {}
    for line in path.read_text(errors="replace").splitlines():
        match = re.match(r"\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", line)
        if match:
            names.setdefault(int(match.group(2), 16), []).append(match.group(1))
    return {address: tuple(found) for address, found in names.items()}


def _has_address_asm_label(source: Path, address: int) -> bool:
    """Whether a C unit binds to its FUN symbol or includes its FUN oracle."""
    try:
        text = source.read_text(errors="replace")
    except OSError:
        return False
    label = re.escape(f"FUN_{address:08x}")
    if re.search(r'__asm__\s*\(\s*"' + label + r'"\s*\)', text, re.IGNORECASE):
        return True
    # An intentional-assembly wrapper includes the raw oracle by its FUN name.
    return re.search(
        r'INCLUDE_ASM\s*\(\s*"[^"]*/' + label + r'\.s"', text, re.IGNORECASE
    ) is not None


def map_rows(path: Path) -> dict[int, dict[str, object]]:
    rows: dict[int, dict[str, object]] = {}
    with path.open(newline="") as stream:
        for row in csv.DictReader(stream):
            try:
                start = int(row["Start"], 0)
                end = int(row["End"], 0)
            except (KeyError, TypeError, ValueError):
                continue
            if end > start:
                rows[start] = {
                    "symbol": row.get("Name") or f"FUN_{start:08x}",
                    "size": end - start,
                }
    return rows


def _is_address_unit(name: str) -> bool:
    """A promoted unit still named after its address (``<group>/fun_001f4fb8``):
    its source defines the FUN symbol itself, as the ``textbin/`` units did."""
    return not name.startswith("assembly/") and re.fullmatch(r"fun_[0-9a-f]{8}", name.rsplit("/", 1)[-1]) is not None


def configured_textbin_functions(
    config: Path,
    function_map: Path,
    source_root: Path | None = None,
) -> list[dict[str, object]]:
    mapped = map_rows(function_map)
    rows: list[dict[str, object]] = []
    for item in parse_main_subsegments(config):
        if item["kind"] != "c":
            continue
        name = str(item["name"])
        # Validate before using the configured name to inspect a source path.
        if rnc_units.unsafe_unit_name(name):
            raise ValueError(f"unsafe unit name in configuration: {name!r}")
        is_textbin_unit = name.startswith(("assembly/textbin/", "textbin/")) or _is_address_unit(name)
        address = int(item["start"])
        # A unit may hold several functions of one translation unit; they must
        # tile its range exactly in the function map.
        functions: list[dict[str, object]] = []
        cursor, end = address, address + int(item["size"])
        while cursor < end:
            row = mapped.get(cursor)
            if row is None:
                break
            symbol = str(row["symbol"])
            # Unit and symbol names become file paths and assembler arguments:
            # fail closed instead of writing outside the workspace.
            if rnc_units.unsafe_unit_name(symbol):
                raise ValueError(f"unsafe symbol name in function map: {symbol!r}")
            functions.append({"symbol": symbol, "address": cursor, "size": row["size"]})
            cursor += int(row["size"])
        if not functions or cursor != end:
            continue
        has_address_asm_label = source_root is not None and any(
            _has_address_asm_label(source_root / f"{name}.c", int(f["address"]))
            for f in functions
        )
        # Retained NON_MATCHING units keep the ``assembly/textbin/`` prefix;
        # exact promotion used to normalize them to ``textbin/``. Semantic
        # source moves no longer carry that prefix, so also select C units
        # whose source explicitly binds its function to a FUN assembler label.
        if not (is_textbin_unit or has_address_asm_label):
            continue
        rows.append(
            {
                "unit": name,
                "address": address,
                "size": item["size"],
                "symbol": functions[0]["symbol"],
                "functions": functions,
                "has_address_asm_label": bool(has_address_asm_label),
            }
        )
    return rows


def install(
    workspace: Path,
    *,
    config: Path,
    function_map: Path,
    elf: Path,
) -> dict[str, object]:
    binutils_root = os.environ.get("BINUTILS_ROOT")
    assembler = Path(
        (str(Path(binutils_root) / "mips-ps2-decompals-as") if binutils_root else None)
        or shutil.which("mips-ps2-decompals-as")
        or "/opt/binutils-mips-ps2-decompals/mips-ps2-decompals-as"
    )
    installed: list[str] = []
    errors: list[dict[str, str]] = []
    retail_names = _symbol_addrs(config.parent / "symbol_addrs.txt")
    for row in configured_textbin_functions(
        config, function_map, workspace / "src"
    ):
        unit = str(row["unit"])
        symbol = str(row["symbol"])
        asm = workspace / "config/us/expected/asm" / unit / f"{symbol}.s"
        obj = workspace / "config/us/expected/obj" / f"{unit}.c.o"
        wrapper = workspace / "src" / f"{unit}.c"
        for path in (asm, obj, wrapper):
            if not rnc_units.path_inside(path, workspace):
                raise ValueError(f"refusing to write outside the workspace: {path}")
        asm.parent.mkdir(parents=True, exist_ok=True)
        obj.parent.mkdir(parents=True, exist_ok=True)
        try:
            body = ""
            for function in row["functions"]:
                name = str(function["symbol"])
                start = int(function["address"])
                raw = _elf_file_bytes(elf, start, int(function["size"]))
                words = [
                    struct.unpack_from("<I", raw, offset)[0]
                    for offset in range(0, len(raw), 4)
                ]
                if (unit.startswith("textbin/") or _is_address_unit(unit)) and not row["has_address_asm_label"]:
                    labels = [name]
                else:
                    labels = _textbin_oracle_labels(workspace / "src", name, start)
                    retail = [n for n in retail_names.get(start, ()) if n not in labels]
                    # A function the source defines under its retail name, not
                    # its FUN label, is sized under that name as well.
                    if retail and not _has_address_asm_label(
                        workspace / "src" / f"{unit}.c", start
                    ):
                        name = retail[0]
                        labels = [name] + labels + retail[1:]
                    else:
                        labels += retail
                body += "".join(f"glabel {label}\n" for label in labels)
                body += "".join(f"    .word 0x{word:08X}\n" for word in words)
                body += f".size {name}, . - {name}\n"
            rendered = (
                '.include "macro.inc"\n.set noreorder\n.text\n'
                + body
                + ".set reorder\n"
            )
            if obj.is_file() and asm.is_file() and asm.read_text() == rendered:
                installed.append(unit)
                continue
            asm.write_text(rendered)
            if wrapper.is_file():
                os.utime(wrapper, None)
            if not assembler.is_file():
                raise RuntimeError(f"assembler not found: {assembler}")
            result = subprocess.run(
                [
                    str(assembler),
                    "-EL",
                    "-march=r5900",
                    "-mabi=eabi",
                    "-no-pad-sections",
                    f"-I{workspace / 'include'}",
                    f"-I{workspace / 'config/us'}",
                    "-o",
                    str(obj),
                    str(asm),
                ],
                cwd=workspace,
                capture_output=True,
                text=True,
            )
            if result.returncode != 0:
                raise RuntimeError((result.stdout + result.stderr)[-1200:])
            installed.append(unit)
        except (OSError, RuntimeError, ValueError, struct.error) as error:
            obj.unlink(missing_ok=True)
            errors.append({"unit": unit, "error": str(error)})
    return {"workspace": str(workspace), "installed": installed, "errors": errors}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--workspace", type=Path, required=True)
    parser.add_argument("--config", type=Path)
    parser.add_argument(
        "--function-map",
        type=Path,
        default=PROJECT_ROOT / "config/ghidra/ghidra_functions.csv",
    )
    parser.add_argument("--elf", type=Path)
    parser.add_argument("--json", type=Path)
    args = parser.parse_args()
    workspace = args.workspace.resolve()
    config = (args.config or workspace / "config/us/rnc1.us.yaml").resolve()
    elf = (args.elf or workspace / "config/us/SCUS_971.99").resolve()
    try:
        result = install(
            workspace,
            config=config,
            function_map=args.function_map.resolve(),
            elf=elf,
        )
    except ValueError as error:
        print(f"materialize-textbin-oracles: error: {error}", file=sys.stderr)
        return 2
    if args.json:
        args.json.resolve().write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))
    return 0 if not result["errors"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
