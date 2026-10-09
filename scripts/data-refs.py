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
"""

from __future__ import annotations

import argparse
import bisect
import re
import sys
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
LABEL_RE = re.compile(r'__asm__\("D_([0-9A-Fa-f]{8})"\)')

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
            yield Access(insn.address, (value + op.mem.disp) & MASK32, WIDTH.get(mnem, 0), kind)

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


def c_labels() -> set[int]:
    """Data addresses named by a ``D_`` label in the C sources, outside overlays."""
    labels = set()
    for path in (ROOT / "src").rglob("*.c"):
        if "overlays" in path.parts:
            continue
        labels.update(int(m, 16) for m in LABEL_RE.findall(path.read_text(errors="replace")))
    return labels


def build(accesses: Iterable[Access], data: list, owner_filter: str | None):
    starts, owners = owners_table(CONFIG_PATH)
    rows: dict[int, dict] = {}
    outside = 0
    for access in accesses:
        name = section_name(access.target, data)
        if name is None:
            outside += 1
            continue
        owner = owner_of(access.insn, starts, owners)
        if owner_filter and owner_filter not in owner:
            continue
        row = rows.setdefault(access.target, {
            "section": name, "widths": set(), "load": 0, "store": 0, "addr": 0,
            "owners": set(), "first_insn": access.insn})
        if access.width:
            row["widths"].add(access.width)
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
    parser.add_argument("--out", type=Path, default=OUT_DIR, help="output directory")
    args = parser.parse_args(argv)

    if not args.elf.exists():
        print(f"missing {args.elf}: copy your own SCUS_971.99 there (see docs/building.md)",
              file=sys.stderr)
        return 1
    gp_match = GP_VALUE_RE.search(CONFIG_PATH.read_text())
    gp = int(gp_match.group(1), 16) if gp_match else 0x166C00

    code, data = read_sections(args.elf)
    rows, outside = build(scan_elf(code, data, gp), data, args.owner)

    args.out.mkdir(parents=True, exist_ok=True)
    write_tsv(rows, args.out / "refs.tsv")
    summary = summarise(rows, data, outside)
    if args.labels:
        labels = c_labels()
        reached = {t for t in rows if section_name(t, data)}
        summary.append(f"C D_ labels reached by the code\t{len(labels & reached)}\t"
                       f"of {len(labels)}\t-")
        summary.append(f"C D_ labels not reached\t{len(labels - reached)}\t-\t-")
    (args.out / "summary.txt").write_text("\n".join(summary) + "\n")
    print("\n".join(summary))
    print(f"wrote {shown(args.out / 'refs.tsv')}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
