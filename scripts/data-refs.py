#!/usr/bin/env python3
"""List the data the boot executable reads and writes, found from its code.

Every load and store in the retail boot code touches an address. It is either
gp-relative, or its base register holds a constant that a ``lui`` and an
``addiu``/``ori`` built earlier in the same block. This script works those out
for the whole executable and reports, per touched address:

* the data section it lies in (``.data``, ``core.lit``, ``.bss`` ...),
* the access widths seen (1, 2, 4, 8, 16 bytes) and how often it is loaded,
  stored, or only has its address formed,
* the configured unit whose code makes the access (``[C]`` marks a unit with
  a C body in ``src/``).

It reads the retail ELF, which is your own disc's executable and is not in
the repository, so the output stays local: it goes to ``build/data-refs/``
(ignored by Git). Two things it cannot see: addresses reached only through a
pointer loaded from other data, and accesses from the level overlays (their
code is not in the boot ELF).

Usage:
  python3 scripts/data-refs.py                      # all touched data
  python3 scripts/data-refs.py --owner runtime/      # only units under a path
  python3 scripts/data-refs.py --labels             # also check src/ D_ labels
  python3 scripts/data-refs.py --elf PATH           # another executable
  python3 scripts/data-refs.py --catalog            # write build/data-refs/boot/data.yaml
  python3 scripts/data-refs.py --level 0 --overlays DIR
                                                    # one level image, DIR is
                                                    # extracted/overlays/ from Tools
"""

from __future__ import annotations

import argparse
import bisect
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile

from dataclasses import dataclass
from pathlib import Path
from typing import Callable, Iterable, Iterator, Union

import capstone
from capstone.mips import MIPS_OP_IMM, MIPS_OP_MEM, MIPS_OP_REG
from elftools.elf.elffile import ELFFile

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rnc_units  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
ELF_PATH = ROOT / "config" / "us" / "SCUS_971.99"
CONFIG_PATH = ROOT / "config" / "us" / "rnc1.us.yaml"
OUT_DIR = ROOT / "build" / "data-refs"
FUNCTIONS_PATH = ROOT / "config" / "overlays" / "us" / "functions.tsv"
DEFAULT_GP = 0x166C00
DEFAULT_CATALOG = ROOT / "build" / "data-refs" / "boot" / "data.yaml"
LEVEL_DATA_START = 0x15EF00  # D_LNN_ labels begin here; below it a level keeps the executable's names

SHF_ALLOC = 0x2
SHF_EXECINSTR = 0x4
# rnc1.us.yaml lists the main segment as file offsets: start 0x1000 at 0x100080.
MAIN_VRAM_DELTA = 0x100080 - 0x1000
# VU1 microcode is not MIPS code.
NOT_MIPS_CODE = {".vutext"}
MASK32 = 0xFFFFFFFF

GPRS = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
        "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"]
GP_VALUE_RE = re.compile(r"gp_value:\s*(0x[0-9a-fA-F]+)")
OVERLAY_FUNC_RE = re.compile(r"\bFUN_L\d\d_[0-9a-fA-F]{8}\b")
LABEL_RE = re.compile(r'__asm__\("D_([0-9A-Fa-f]{8})"\)')
# `extern struct HudState hud_state __asm__("D_0019A3E8");` -> type, name, array dims, address
DECL_RE = re.compile(
    r'^[ \t]*(?:extern[ \t]+)?([^;=\n]*?[ \t*])([A-Za-z_]\w*)[ \t]*((?:\[[^\]]*\])*)[ \t]*'
    r'__asm__\("D_([0-9A-Fa-f]{8})"\)', re.M)
CATALOG_HEADER = """\
# Data {what} code touches, found by scripts/data-refs.py --catalog
# from the retail code. Grouped by section, then by where the data comes from:
# {origins}
#
# One entry per line: [addr, name, type, width, loads, stores, address_taken, used_by]
#
#   name           the C name where src/ declares it, else its D_<addr> label
#   type           the C type where src/ declares it, else guessed from the access width
#   width          widest single access seen, in bytes (not the size of the object)
#   loads, stores  how many instructions read and write it
#   address_taken  how many times only its address is formed (a table, struct or string)
#   used_by        units whose code touches it
#
# Float constants in the literal pools (only loaded, through the FPU) are left out:
# the compiler makes them from the float literals in the C.
#
# Names and types come from the headers; rename there, then rerun.
{extra}"""
BOOT_ORIGINS = "the subsystem whose code uses it (`shared` = used from several)."
LEVEL_ORIGINS = ("the kind of code that uses it (`exe`: the executable's own code, `shared`: code\n"
                 "# in several levels, `level`: this level only) and its subsystem (`mixed` = several).")
CATALOG_COLUMNS = ("addr", "name", "type", "width", "loads", "stores", "address_taken", "used_by")
USERS_SHOWN = 110  # characters of used_by before the rest is summarised

WIDTH = {
    "lb": 1, "lbu": 1, "sb": 1, "lh": 2, "lhu": 2, "sh": 2,
    "lw": 4, "lwu": 4, "sw": 4, "lwl": 4, "lwr": 4, "swl": 4, "swr": 4,
    "lwc1": 4, "swc1": 4, "sc": 4, "ll": 4,
    "ld": 8, "sd": 8, "ldc1": 8, "sdc1": 8, "lld": 8, "scd": 8,
    "ldl": 8, "ldr": 8, "sdl": 8, "sdr": 8,
}
STORES = {"sb", "sh", "sw", "sd", "swl", "swr", "sdl", "sdr", "swc1", "sdc1", "sc", "scd"}
# Instructions that write no general register, so they change no constant.
NO_DEST = {"sb", "sh", "sw", "sd", "swl", "swr", "sdl", "sdr", "swc1", "sdc1",
           "mult", "multu", "dmult", "dmultu", "div", "divu", "ddiv", "ddivu"}
FP_MNEMS = {"lwc1", "swc1", "ldc1", "sdc1"}
# Prefetch and cache hints touch no data.
HINTS = {"pref", "cache"}
# capstone gives no group tag for jal, so calls and branches are told apart by name.
CALLS = {"jal", "jalr", "bal", "bgezal", "bltzal", "bgezall", "bltzall"}
TRANSFERS = CALLS | {
    "j", "jr", "eret", "b", "beq", "bne", "beql", "bnel", "bgez", "bgezl", "bgtz", "bgtzl",
    "blez", "blezl", "bltz", "bltzl", "bc1f", "bc1t", "bc1fl", "bc1tl",
}
# Registers a call may change; the rest survive it.
CALLER_SAVED = {"at", "v0", "v1", "a0", "a1", "a2", "a3", "ra",
                "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9"}
# EE-only words capstone does not decode as MIPS: MMI (0x1c), COP2 (0x12) and
# the VU vector loads and stores (0x32, 0x36, 0x3a, 0x3e). Their register
# effects are unknown here, so they forget every constant.
EE_OPAQUE_OPCODES = {0x12, 0x1C, 0x32, 0x36, 0x3A, 0x3E}
# EE 128-bit loads and stores, which capstone reads as other instructions.
LQ_OPCODE, SQ_OPCODE = 0x1E, 0x1F


@dataclass(frozen=True)
class Access:
    insn: int          # address of the instruction
    target: int        # address of the data it touches
    width: int         # bytes (0 for an address that is only formed)
    kind: str          # "load", "store" or "addr"
    fp: bool = False   # read or written through the FPU


@dataclass(frozen=True)
class Quad:
    """An EE lq or sq, decoded here because capstone does not know them."""
    address: int
    base: str
    disp: int
    rt: str
    store: bool


@dataclass(frozen=True)
class Opaque:
    """A word whose register effects are unknown; it forgets every constant."""
    address: int


def shown(path: Path) -> str:
    """The path relative to the checkout when it is inside it."""
    try:
        return str(path.resolve().relative_to(ROOT))
    except ValueError:
        return str(path)


def read_sections(elf_path: Path):
    """(code, data) sections of the executable as (name, addr, size, bytes)."""
    code, data = [], []
    with elf_path.open("rb") as fh:
        for section in ELFFile(fh).iter_sections():
            flags = section["sh_flags"]
            addr, size = section["sh_addr"], section["sh_size"]
            if not flags & SHF_ALLOC or addr == 0:
                continue  # DVP overlay blobs sit at address 0 and are not boot code
            if flags & SHF_EXECINSTR:
                if section.name not in NOT_MIPS_CODE:
                    code.append((section.name, addr, section.data()))
            else:
                data.append((section.name, addr, size))
    return code, data


def owners_table(config: Path) -> tuple[list[int], list[tuple[str, str]]]:
    """Sorted unit start addresses and their (owner, kind) from the main segment."""
    rows = rnc_units.parse_config_rows(config)
    starts, owners = [], []
    for offset, kind, owner in rows:
        starts.append(offset + MAIN_VRAM_DELTA)
        owners.append((owner, kind))
    return starts, owners


def owner_of(addr: int, starts: list[int], owners: list[tuple[str, str]]) -> str:
    index = bisect.bisect_right(starts, addr) - 1
    if index < 0:
        return "?"
    owner, kind = owners[index]
    if kind != "c":
        return f"{owner} [{kind}]"
    return f"{owner} [C]" if (ROOT / "src" / f"{owner}.c").exists() else owner


def level_image(overlays: Path, level: int) -> tuple[list, list]:
    """(code, data) of one level program, from extracted/overlays in the Tools checkout."""
    folder = overlays / f"level_{level:02d}"
    manifest = json.loads((folder / "manifest.json").read_text())
    code, data = [], []
    for record in manifest["records"]:
        if record["name"] == "text":
            code.append(("text", record["address"], (folder / "text.bin").read_bytes()))
        else:
            data.append((record["name"], record["address"], record["bytes"]))
    return code, data


def level_origin_of() -> Callable[[str], str]:
    """Where a level function comes from: exe/<subsystem>, shared/<subsystem> or level/<subsystem>."""
    kinds: dict[str, tuple[str, str]] = {}
    for line in FUNCTIONS_PATH.read_text().splitlines():
        fields = line.split("\t")
        if not line.startswith("#") and len(fields) >= 7:
            kinds[fields[0]] = (fields[1], fields[6])
    homes: dict[str, tuple[str, ...]] = {}
    base = ROOT / "src" / "overlays"
    for path in base.rglob("*.c"):
        parts = path.relative_to(base).parts
        for name in set(OVERLAY_FUNC_RE.findall(path.read_text(errors="replace"))):
            homes[name] = parts

    def origin(function: str) -> str:
        kind, exe_unit = kinds.get(function, ("?", ""))
        if kind == "exe":
            return f"exe/{subsystem_of(exe_unit)}"
        home = homes.get(function)
        if home and len(home) > 2:
            return f"{'shared' if home[0] == 'shared' else 'level'}/{home[1]}"
        return f"{kind if kind in ('shared', 'level') else 'other'}/other"

    return origin


def level_owner_of(level: int) -> Callable[[int], str]:
    """Owner of an address in a level: the nearest function at or below it."""
    tag = f"{level:02d}:"
    places = []
    for line in FUNCTIONS_PATH.read_text().splitlines():
        fields = line.split("\t")
        if line.startswith("#") or len(fields) < 6:
            continue
        for place in fields[5].split(","):
            if place.startswith(tag):
                places.append((int(place[len(tag):], 16), fields[0]))
    places.sort()
    starts = [address for address, _ in places]

    def owner(addr: int) -> str:
        index = bisect.bisect_right(starts, addr) - 1
        return places[index][1] if index >= 0 else "?"

    return owner


def disassemble(code: bytes, base: int) -> Iterator[Union[object, Quad, Opaque]]:
    """One item per 4-byte word: a capstone instruction, a Quad or an Opaque."""
    md = capstone.Cs(capstone.CS_ARCH_MIPS,
                     capstone.CS_MODE_MIPS64 | capstone.CS_MODE_LITTLE_ENDIAN)
    md.detail = True
    for off in range(0, len(code) - 3, 4):
        address = base + off
        word = int.from_bytes(code[off:off + 4], "little")
        opcode = word >> 26
        if opcode in (LQ_OPCODE, SQ_OPCODE):
            disp = word & 0xFFF0  # the low four bits are not part of the offset
            disp -= 0x10000 if disp & 0x8000 else 0
            yield Quad(address, GPRS[(word >> 21) & 31], disp,
                       GPRS[(word >> 16) & 31], opcode == SQ_OPCODE)
            continue
        if opcode in EE_OPAQUE_OPCODES:
            yield Opaque(address)
            continue
        decoded = list(md.disasm(code[off:off + 4], address, 1))
        if decoded:
            yield from decoded
        else:
            yield Opaque(address)


def branch_targets(items: list) -> set[int]:
    """Addresses that some jump or branch can reach; constants reset there."""
    targets = set()
    for item in items:
        if isinstance(item, (Quad, Opaque)) or item.mnemonic not in TRANSFERS:
            continue
        for op in item.operands:
            if op.type == MIPS_OP_IMM:
                targets.add(op.imm & MASK32)
    return targets


def scan(items: list, gp: int, in_data: Callable[[int], bool]) -> Iterator[Access]:
    """Accesses made by one code block, with the register constants it implies."""
    targets = branch_targets(items)
    known: dict[str, int] = {}
    after_jump = 0  # instructions until the code after a jump can only be a target

    def base_value(name: str) -> int | None:
        return gp if name == "gp" else known.get(name)

    for item in items:
        if item.address in targets:
            known = {}
        if after_jump:
            after_jump -= 1
            if after_jump == 0:
                known = {}

        if isinstance(item, Opaque):
            known = {}
            continue
        if isinstance(item, Quad):
            value = base_value(item.base)
            if value is not None:
                kind = "store" if item.store else "load"
                yield Access(item.address, (value + item.disp) & MASK32, 16, kind)
            if not item.store:
                known.pop(item.rt, None)
            continue

        insn = item
        mnem = insn.mnemonic
        ops = insn.operands
        if mnem in TRANSFERS:
            # A jump's delay slot runs first; eret has none.
            after_jump = 1 if mnem == "eret" else (2 if mnem in ("j", "jr") else after_jump)
        for op in ops:
            if op.type != MIPS_OP_MEM or mnem in HINTS:
                continue
            value = base_value(insn.reg_name(op.mem.base))
            if value is None:
                continue
            kind = "store" if mnem in STORES else "load"
            yield Access(insn.address, (value + op.mem.disp) & MASK32, WIDTH.get(mnem, 0), kind,
                         mnem in FP_MNEMS)

        if mnem in CALLS:
            known = {r: v for r, v in known.items() if r not in CALLER_SAVED}
            continue
        if mnem in NO_DEST or mnem in TRANSFERS or not ops or ops[0].type != MIPS_OP_REG:
            continue
        dest = insn.reg_name(ops[0].reg)
        value = None
        if mnem == "lui" and len(ops) == 2 and ops[1].type == MIPS_OP_IMM:
            value = (ops[1].imm & 0xFFFF) << 16
        elif mnem in ("addiu", "daddiu", "ori") and len(ops) == 3 \
                and ops[1].type == MIPS_OP_REG and ops[2].type == MIPS_OP_IMM:
            src = base_value(insn.reg_name(ops[1].reg))
            if src is not None:
                imm = ops[2].imm
                value = ((src | (imm & 0xFFFF)) if mnem == "ori" else (src + imm)) & MASK32
                if in_data(value):
                    yield Access(insn.address, value, 0, "addr")
        elif mnem == "move" and len(ops) == 2 and ops[1].type == MIPS_OP_REG:
            value = base_value(insn.reg_name(ops[1].reg))
        if value is None:
            known.pop(dest, None)
        else:
            known[dest] = value & MASK32


def scan_elf(code: list, data: list, gp: int) -> Iterator[Access]:
    ranges = sorted((addr, addr + size) for _, addr, size in data)
    starts = [start for start, _ in ranges]

    def in_data(value: int) -> bool:
        index = bisect.bisect_right(starts, value) - 1
        return index >= 0 and value < ranges[index][1]

    for _name, base, body in code:
        yield from scan(list(disassemble(body, base)), gp, in_data)


def section_name(addr: int, data: list[tuple[str, int, int]]) -> str | None:
    for name, start, size in data:
        if start <= addr < start + size:
            return name
    return None


def c_labels(level: int | None = None) -> set[int]:
    """Data addresses named by a ``D_`` label in the C sources.

    The boot executable's labels are those outside src/overlays/; a level's are
    the ``D_LNN_`` labels inside src/overlays/lNN/.
    """
    if level is None:
        pattern, files = LABEL_RE, [p for p in (ROOT / "src").rglob("*.c") if "overlays" not in p.parts]
    else:
        pattern = re.compile(rf'__asm__\("D_L{level:02d}_([0-9A-Fa-f]{{8}})"\)')
        files = list((ROOT / "src" / "overlays" / f"l{level:02d}").rglob("*.c"))
    labels = set()
    for path in files:
        labels.update(int(m, 16) for m in pattern.findall(path.read_text(errors="replace")))
    return labels


def c_declarations() -> dict[int, tuple[str, str]]:
    """address -> (name, C type) of the named ``D_`` declarations outside overlays."""
    files = sorted((ROOT / "include").rglob("*.h")) + sorted(
        p for p in (ROOT / "src").rglob("*.[ch]") if "overlays" not in p.parts)
    found: dict[int, tuple[str, str]] = {}
    for path in files:
        for prefix, name, dims, addr in DECL_RE.findall(path.read_text(errors="replace")):
            entry = (name, " ".join((prefix + dims).split()))
            old = found.get(int(addr, 16))
            if old is None or (old[0].startswith("D_") and not name.startswith("D_")):
                found[int(addr, 16)] = entry
    return found


LAYOUT_RE = re.compile(r"^\s*(\d+)(?::\d+-\d+)? \|( *)(.*?)\s*$")


def parse_layouts(text: str) -> dict[str, list[tuple[int, int, str, str]]]:
    """clang's record layouts as {record: [(offset, size, path, type)]}.

    Leaf members only; a nested struct's members are listed with the
    nested name in the path (``cd_mode.trycount``). Sizes are 0 when unknown.
    """
    out: dict[str, list] = {}
    for block in text.split("*** Dumping AST Record Layout")[1:]:
        lines = []
        for line in block.splitlines():
            m = LAYOUT_RE.match(line)
            if m:
                lines.append((int(m.group(1)), len(m.group(2)), m.group(3)))
        if not lines:
            continue
        fields: list[tuple[int, int, str, str]] = []
        stack: list[tuple[int, str]] = []  # (indent, name) of enclosing structs
        for i, (off, indent, text_) in enumerate(lines[1:], 1):
            while stack and stack[-1][0] >= indent:
                stack.pop()
            nxt = lines[i + 1] if i + 1 < len(lines) else None
            kind, _, name = text_.rpartition(" ")
            nested = nxt is not None and nxt[1] > indent
            if nested:
                stack.append((indent, name))
                continue
            path = ".".join([n for _, n in stack] + [name])
            fields.append((off, 0, path, kind))
        out[lines[0][2].replace("struct ", "", 1).replace("union ", "", 1)] = fields
    return out


def member_names(declared: dict[int, tuple[str, str]],
                 addresses: Iterable[int]) -> dict[int, tuple[str, str | None]]:
    """Names for addresses inside declared struct objects: ``object.field``."""
    clang = shutil.which("clang")
    if clang is None:
        return {}
    files = sorted((ROOT / "include").rglob("*.h")) + sorted(
        p for p in (ROOT / "src").rglob("*.[ch]") if "overlays" not in p.parts)
    layouts: dict[str, list] = {}
    with tempfile.TemporaryDirectory() as tmp:
        probe = Path(tmp) / "probe.c"
        for path in files:
            names = [name for _, name, _, _ in DECL_RE.findall(path.read_text(errors="replace"))]
            if not names:
                continue
            # clang only dumps the layout of a record the code uses: take each object's address.
            probe.write_text(f'#include "{path}"\n' + "".join(
                f"void *keep_{i} = (void *)&{name};\n" for i, name in enumerate(names)))
            run = subprocess.run(
                [clang, "--target=mipsel-linux-gnu", "-c", "-o", "/dev/null", f"-I{ROOT / 'include'}",
                 "-w", "-Xclang", "-fdump-record-layouts", str(probe)],
                capture_output=True, text=True, check=False)
            layouts.update(parse_layouts(run.stdout))
    bases = sorted((a, n, t) for a, (n, t) in declared.items())
    starts = [b[0] for b in bases]
    out: dict[int, tuple[str, str]] = {}
    for addr in addresses:
        if addr in declared:
            continue
        for i in range(bisect.bisect_right(starts, addr) - 1, -1, -1):
            base, name, ctype = bases[i]
            fields = layouts.get(re.sub(r"^(struct|union)\s+", "", ctype.strip()))
            if not fields:
                continue
            rel = addr - base
            inside = [f for f in fields if f[0] <= rel]
            if not inside:
                continue
            off, _size, path, kind = max(inside, key=lambda f: f[0])
            if rel >= off + type_size(kind):
                continue
            out[addr] = (f"{name}.{path}" + (f"+0x{rel - off:X}" if rel != off else ""),
                         kind if rel == off else None)
            break
    return out


def type_size(kind: str) -> int:
    """Byte size of a layout member type; unknown (struct) types count as 1."""
    count = 1
    m = re.match(r"(.*)\[(\d+)\]$", kind)
    if m:
        kind, count = m.group(1), int(m.group(2))
    if kind.endswith("*"):
        return 4 * count
    size = {"u8": 1, "s8": 1, "char": 1, "s16": 2, "u16": 2, "s32": 4, "u32": 4, "f32": 4,
            "float": 4, "int": 4, "s64": 8, "u64": 8, "f64": 8, "double": 8}.get(kind.strip(), 1)
    return size * count


# —— Data definitions ——

SCALARS = {"u8": "<B", "s8": "<b", "char": "<b", "u16": "<H", "s16": "<h", "u32": "<I",
           "s32": "<i", "int": "<i", "u64": "<Q", "s64": "<q", "f32": "<f", "float": "<f",
           "f64": "<d", "double": "<d"}
DATA_SECTIONS = ("core.data", ".data")  # PROGBITS sections a definition may fill


def layout_tree(dump: str, record: str) -> list | None:
    """``record``'s members from clang's layout dump as [(offset, type, name, children)]."""
    for block in dump.split("*** Dumping AST Record Layout")[1:]:
        rows = [(int(m.group(1)), len(m.group(2)), m.group(3)) for m in map(LAYOUT_RE.match, block.splitlines())
                if m]
        if not rows or rows[0][2].split(" ", 1)[-1] != record:
            continue

        def children(start: int, indent: int) -> tuple[list, int]:
            out, i = [], start
            while i < len(rows) and rows[i][1] > indent:
                off, ind, text = rows[i]
                kind, _, name = text.rpartition(" ")
                sub, nxt = children(i + 1, ind)
                out.append((off, kind, name, sub))
                i = nxt
            return out, i

        return children(1, rows[0][1])[0]
    return None


def c_value(kind: str, raw: bytes, symbols: dict[int, str]) -> str:
    """A scalar or pointer member as C source."""
    if kind.endswith("*"):
        word = struct.unpack("<I", raw[:4])[0]
        if word == 0:
            return "0"
        if word in symbols:
            return f"&{symbols[word]}"
        raise ValueError(f"pointer 0x{word:08X} names no known symbol")
    fmt = SCALARS.get(kind.strip())
    if fmt is None:
        raise ValueError(f"no scalar format for {kind!r}")
    value = struct.unpack(fmt, raw[:struct.calcsize(fmt)])[0]
    if fmt in ("<f", "<d"):
        return repr(value) + ("f" if fmt == "<f" else "")
    if value < -0xFFFF and fmt in ("<i", "<q"):
        return hex(value & (1 << 8 * struct.calcsize(fmt)) - 1)  # bit pattern, e.g. a packed color
    return str(value) if value < 0 or value < 10 else hex(value)


def c_initializer(members: list, size: int, raw: bytes, symbols: dict[int, str],
                  records: Callable[[str], list | None] = lambda name: None) -> str:
    """Brace initialiser for a record laid out by ``members`` over ``raw``.

    ``records`` gives the members of a nested record type clang did not expand
    (the element type of an array of structs). All-zero parts collapse to ``{0}``.
    """
    if not any(raw):
        return "{0}"
    parts = []
    for index, (off, kind, _name, sub) in enumerate(members):
        end = members[index + 1][0] if index + 1 < len(members) else size
        field = raw[off:end]
        if sub:
            parts.append(c_initializer([(o - off, k, n, s) for o, k, n, s in sub], end - off, field, symbols,
                                       records))
            continue
        array = re.fullmatch(r"(.*)\[(\d+)\]", kind)
        if array:
            elem, count = array.group(1), int(array.group(2))
            step = len(field) // count if count else 0
            inner = records(re.sub(r"^(struct|union)\s+", "", elem.strip()))
            parts.append("{" + ", ".join(
                c_initializer(inner, step, field[i * step:(i + 1) * step], symbols, records) if inner
                else c_value(elem, field[i * step:(i + 1) * step], symbols)
                for i in range(count)) + "}")
        else:
            parts.append(c_value(kind, field, symbols))
    return "{" + ", ".join(parts) + "}"


def emit_data(addr: int, elf: Path, sections: list, into: Path) -> Path:
    """Append the definition of the declared object at ``addr`` to ``into``,
    the C file of the only unit that reads it (listed in DATA_OVERLAYS)."""
    clang = shutil.which("clang")
    if clang is None:
        raise SystemExit("--emit needs clang to read the struct layout")
    section = section_name(addr, sections)
    if section not in DATA_SECTIONS:
        raise SystemExit(f"0x{addr:08X} is in {section}, not a data section a file can fill")
    header, name, ctype = None, None, None
    for path in sorted((ROOT / "include").rglob("*.h")):
        for prefix, found, dims, label in DECL_RE.findall(path.read_text(errors="replace")):
            if int(label, 16) == addr:
                header, name, ctype = path, found, " ".join((prefix + dims).split())
    if header is None:
        raise SystemExit(f"0x{addr:08X} has no declaration in include/: declare its type there first")
    elem = " ".join(ctype.replace("extern", "").split()).rpartition("[")[0] if "[" in ctype else ctype
    elem = re.sub(r"\s*\[.*$", "", ctype.replace("extern", "")).strip()
    counts = [int(n, 0) for n in re.findall(r"\[(0x[0-9A-Fa-f]+|\d+)\]", ctype)]
    if ctype.count("[") != len(counts):
        raise SystemExit(f"{ctype!r} has no complete size")
    record = re.sub(r"^(struct|union)\s+", "", elem)
    include = header.relative_to(ROOT / "include").as_posix()
    dump, members = "", None
    if elem.endswith("*") or elem in SCALARS:
        step = 4 if elem.endswith("*") else struct.calcsize(SCALARS[elem])
    else:
        with tempfile.TemporaryDirectory() as tmp:
            probe = Path(tmp) / "probe.c"
            probe.write_text(f'#include "types.h"\n#include "{include}"\n'
                             f"void *keep = (void *)&{name};\nint size = sizeof({name});\n")
            run = subprocess.run([clang, "--target=mipsel-linux-gnu", "-c", "-o", "/dev/null",
                                  f"-I{ROOT / 'include'}", "-w", "-Xclang", "-fdump-record-layouts", str(probe)],
                                 capture_output=True, text=True, check=False)
        dump = run.stdout
        members = layout_tree(dump, record)
        size = re.search(r"\| \[sizeof=(\d+)", dump[dump.find(f" {record}\n"):])
        if members is None or size is None:
            raise SystemExit(f"no layout for {ctype!r}")
        step = int(size.group(1))
    size = step
    for n in counts:
        size *= n
    with elf.open("rb") as fh:
        image = ELFFile(fh)
        for seg in image.iter_segments():
            if seg["p_type"] == "PT_LOAD" and seg["p_vaddr"] <= addr < seg["p_vaddr"] + seg["p_filesz"]:
                fh.seek(seg["p_offset"] + addr - seg["p_vaddr"])
                raw = fh.read(size)
                break
        else:
            raise SystemExit(f"0x{addr:08X} has no file bytes")
    symbols = {a: n for a, (n, _t) in c_declarations().items()}

    def one(chunk: bytes) -> str:
        if members is None:
            return c_value(elem, chunk, symbols)
        return c_initializer(members, step, chunk, symbols, lambda rec: layout_tree(dump, rec))

    def nest(chunk: bytes, dims: list) -> str:
        if not dims:
            return one(chunk)
        width = len(chunk) // dims[0]
        rows = [nest(chunk[i * width:(i + 1) * width], dims[1:]) for i in range(dims[0])]
        if members is not None and len(dims) == 1:  # one record per line
            return "{\n    " + ",\n    ".join(rows) + ",\n}"
        return "{" + ", ".join(rows) + "}"

    body = "{0}" if not any(raw) else nest(raw, counts)
    out = into
    dims = "".join(f"[{n}]" for n in counts)
    text = out.read_text()
    if re.search(rf"\b{name}\b[^;]*=", text):
        raise SystemExit(f"{shown(out)} already defines {name}")
    lines = text.rstrip("\n").splitlines()
    last = max(i for i, line in enumerate(lines) if line.startswith("#include"))
    if f'#include "{include}"' not in lines:
        lines.insert(last + 1, f'#include "{include}"')
    small = " NOT_SDA" if len(raw) <= 8 else ""
    out.write_text("\n".join(lines) + f"\n\n{elem} {name}{dims}{small} = {body};\n")
    unit = os.path.relpath(out.resolve().with_suffix(""), ROOT / "src")
    print(f'add to DATA_OVERLAYS in configure.py: "{unit}": '
          f"(0x{addr:X}, 0x{addr - 0xFF080:X})")
    return out


def unit_of(owner: str) -> str:
    """``runtime/state/foo [C]`` -> ``runtime/state/foo``."""
    return re.sub(r" \[\w+\]$", "", owner)


def subsystem_of(unit: str) -> str:
    parts = [part for part in unit.split("/") if part not in ("assembly", "textbin")]
    return parts[0] if len(parts) > 1 else "other"


def guess_type(row: dict) -> str:
    if not row["widths"]:
        return "unknown"  # only its address is formed: a table, struct or string
    width = max(row["widths"])
    if row["fp"]:
        return {4: "f32", 8: "f64"}.get(width, "unknown")
    return {1: "u8", 2: "s16", 4: "s32", 8: "s64", 16: "u128"}.get(width, "unknown")


def is_float_literal(row: dict) -> bool:
    """A constant in a literal pool: only loaded through the FPU, so the C float literal makes it."""
    return row["section"].endswith("lit") and row["fp"] and not row["store"] and not row["addr"]


def catalog_sections(rows: dict[int, dict], data: list, declared: dict,
                     origin_of: Callable[[str], str] = subsystem_of,
                     label_of: Callable[[int], str] = lambda addr: f"D_{addr:08X}",
                     several: str = "shared") -> dict:
    """{section: {origin: [entry, ...]}} in section order, entries by address."""
    out: dict[str, dict[str, list]] = {}
    for target in sorted(rows):
        row = rows[target]
        if is_float_literal(row):
            continue
        users = sorted({unit_of(owner) for owner in row["owners"]})
        origins = {origin_of(unit) for unit in users}
        origin = origins.pop() if len(origins) == 1 else several
        name, ctype = declared.get(target, (label_of(target), None))
        entry = {"addr": target, "name": name, "type": ctype or guess_type(row),
                 "width": max(row["widths"], default=0),
                 "loads": row["load"], "stores": row["store"],
                 "address_taken": row["addr"], "used_by": users}
        out.setdefault(row["section"], {}).setdefault(origin, []).append(entry)
    order = [name for name, _, _ in data]
    return {sec: dict(sorted(out[sec].items(), key=lambda kv: (kv[0] == several, kv[0])))
            for sec in order if sec in out}


def scalar(text: str) -> str:
    """A YAML flow scalar: plain when it is simple, quoted when a type has brackets or stars."""
    return text if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_ ]*", text) else json.dumps(text)


def users_text(users: list[str]) -> str:
    shown, size = [], 0
    for user in users:
        if shown and size + len(user) > USERS_SHOWN:
            break
        shown.append(user)
        size += len(user) + 2
    rest = len(users) - len(shown)
    return ", ".join(shown) + (f', "+{rest} more"' if rest else "")


def write_catalog(path: Path, sections: dict,
                  what: str = "the boot executable's", origins: str = BOOT_ORIGINS,
                  extra: str = "") -> None:
    lines = [CATALOG_HEADER.format(what=what, origins=origins, extra=extra)]
    for section, origins in sections.items():
        lines.append(f"{section}:")
        for origin, entries in origins.items():
            lines.append(f"  {origin}:")
            for entry in entries:
                fields = [f"0x{entry['addr']:08X}", scalar(entry["name"]), scalar(entry["type"]),
                          entry["width"], entry["loads"], entry["stores"],
                          entry["address_taken"], f"[{users_text(entry['used_by'])}]"]
                lines.append(f"    - [{', '.join(str(f) for f in fields)}]")
            lines.append("")
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines).rstrip("\n") + "\n")


def build(accesses: Iterable[Access], data: list, owner_for: Callable[[int], str],
          owner_filter: str | None):
    rows: dict[int, dict] = {}
    outside = 0
    for access in accesses:
        name = section_name(access.target, data)
        if name is None:
            outside += 1
            continue
        owner = owner_for(access.insn)
        if owner_filter and owner_filter not in owner:
            continue
        row = rows.setdefault(access.target, {
            "section": name, "widths": set(), "load": 0, "store": 0, "addr": 0,
            "owners": set(), "first_insn": access.insn, "fp": False})
        if access.width:
            row["widths"].add(access.width)
        row["fp"] = row["fp"] or access.fp
        row[access.kind] += 1
        row["owners"].add(owner)
    return rows, outside


def write_tsv(rows: dict[int, dict], path: Path) -> None:
    lines = ["target\tsection\twidths\tloads\tstores\taddr_only\towners\tfirst_insn"]
    for target in sorted(rows):
        row = rows[target]
        lines.append("\t".join([
            f"{target:08x}", row["section"],
            ",".join(str(w) for w in sorted(row["widths"])),
            str(row["load"]), str(row["store"]), str(row["addr"]),
            ";".join(sorted(row["owners"]))[:300],
            f"{row['first_insn']:08x}"]))
    path.write_text("\n".join(lines) + "\n")


def summarise(rows: dict[int, dict], data: list[tuple[str, int, int]], outside: int) -> list[str]:
    lines = ["section\tsize\ttouched\tconstant_refs"]
    for name, start, size in data:
        hits = [t for t, row in rows.items() if start <= t < start + size]
        if hits:
            lines.append(f"{name}\t{size}\t{len(hits)}\t{sum(rows[t]['load'] + rows[t]['store'] for t in hits)}")
    lines.append(f"(outside data sections)\t-\t{outside}\t-")
    return lines


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--elf", type=Path, default=ELF_PATH,
                        help=f"boot executable (default: {shown(ELF_PATH)})")
    parser.add_argument("--owner", help="only accesses made by units whose owner contains TEXT")
    parser.add_argument("--labels", action="store_true",
                        help="compare with the D_ labels in src/ (outside overlays)")
    parser.add_argument("--level", type=int, metavar="N",
                        help="scan level N's program instead of the boot executable")
    parser.add_argument("--overlays", type=Path, metavar="DIR",
                        help="extracted/overlays/ of the Tools checkout (with --level)")
    parser.add_argument("--catalog", nargs="?", type=Path, const=Path("-"), metavar="PATH",
                        help=f"write the grouped catalogue (default: {shown(DEFAULT_CATALOG)}, or "
                             "data.yaml in the output directory with --level)")
    parser.add_argument("--out", type=Path, default=None, help="output directory")
    parser.add_argument("--emit", nargs="+", metavar="ADDR",
                        help="define the declared objects at these addresses (with --into)")
    parser.add_argument("--into", type=Path, metavar="FILE",
                        help="with --emit: the C file of the only unit that reads them")
    args = parser.parse_args(argv)

    if args.emit:
        if args.into is None:
            parser.error("--emit needs --into FILE")
        _code, data = read_sections(args.elf)
        for addr in args.emit:
            print(f"wrote {shown(emit_data(int(addr, 16), args.elf, data, args.into))}")
        return 0

    if args.level is None:
        if not args.elf.exists():
            print(f"missing {args.elf}: copy your own SCUS_971.99 there (see docs/building.md)",
                  file=sys.stderr)
            return 1
        gp_match = GP_VALUE_RE.search(CONFIG_PATH.read_text())
        gp = int(gp_match.group(1), 16) if gp_match else DEFAULT_GP
        code, data = read_sections(args.elf)
        starts, owners = owners_table(CONFIG_PATH)
        owner_for = lambda addr: owner_of(addr, starts, owners)  # noqa: E731
        name = "boot"
    else:
        if args.overlays is None:
            print("--level needs --overlays DIR (extracted/overlays/ of Tools)", file=sys.stderr)
            return 1
        gp, name = DEFAULT_GP, f"level-{args.level:02d}"
        code, data = level_image(args.overlays, args.level)
        # A level keeps the executable's core.* data, which its records do not replace.
        if args.elf.exists():
            _, boot_data = read_sections(args.elf)
            data += [section for section in boot_data if section[0].startswith("core.")]
        owner_for = level_owner_of(args.level)

    out = args.out or OUT_DIR / name
    rows, outside = build(scan_elf(code, data, gp), data, owner_for, args.owner)

    out.mkdir(parents=True, exist_ok=True)
    write_tsv(rows, out / "refs.tsv")
    summary = summarise(rows, data, outside)
    if args.labels:
        labels = c_labels(args.level)
        reached = {t for t in rows if section_name(t, data)}
        summary.append(f"C D_ labels reached by the code\t{len(labels & reached)}\t"
                       f"of {len(labels)}\t-")
        summary.append(f"C D_ labels not reached\t{len(labels - reached)}\t-\t-")
    if args.catalog:
        if args.level is None:
            target = DEFAULT_CATALOG if str(args.catalog) == "-" else args.catalog
            declared = c_declarations()
            sections = catalog_sections(rows, data, {**member_names(declared, rows), **declared})
            write_catalog(target, sections)
        else:
            target = out / "data.yaml" if str(args.catalog) == "-" else args.catalog
            own = {addr: row for addr, row in rows.items() if not row["section"].startswith("core.")}
            sections = catalog_sections(
                own, data, {}, level_origin_of(),
                lambda addr: f"D_{addr:08X}" if addr < LEVEL_DATA_START
                else f"D_L{args.level:02d}_{addr:08X}", "mixed")
            write_catalog(target, sections,
                          what=f"level {args.level:02d}'s", origins=LEVEL_ORIGINS,
                          extra="# The executable's core.* data is in build/data-refs/boot/data.yaml.\n")
        summary.append(f"catalogue: {sum(len(e) for o in sections.values() for e in o.values())} "
                       f"entries in {shown(target)}")
    (out / "summary.txt").write_text("\n".join(summary) + "\n")
    print("\n".join(summary))
    print(f"wrote {shown(out / 'refs.tsv')}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
