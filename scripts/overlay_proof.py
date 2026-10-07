"""The byte proof for a level overlay function (FUN_LNN_xxxxxxxx).

The compiled object is placed so the function sits at its canonical address
(level NN, address xxxxxxxx), every relocation is resolved to that level's
address of its symbol (``_gp`` = 0x166C00), and the result must equal the
level's retail text byte for byte. Method ``overlay-place-bytes-v1``.
"""
from __future__ import annotations

import os
import re
import sys
from functools import lru_cache
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rnc_overlays as ov  # noqa: E402

METHOD = "overlay-place-bytes-v1"

R_MIPS_32 = 2
R_MIPS_26 = 4
R_MIPS_HI16 = 5
R_MIPS_LO16 = 6
R_MIPS_GPREL16 = 7

NAMED_SYMBOL = re.compile(r"^(?:FUN|func)_(?:L\d{2}_)?[0-9A-Fa-f]{8}$|^D_(?:L\d{2}_)?[0-9A-Fa-f]{8}(?:_gp)?$"
                          r"|^jtbl_(?:L\d{2}_)?[0-9A-Fa-f]{8}$")
JTBL_REF = re.compile(r"%(?:hi|lo)\((jtbl_L(\d{2})_([0-9A-Fa-f]{8}))\)")
ASSIGN_RE = re.compile(r"^\s*([A-Za-z_.$][\w.$]*)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;")
LABEL_RE = re.compile(r'__asm__\s*\(\s*"((?:FUN|func)_[0-9A-Fa-f]{8})"\s*\)')


def sign16(v: int) -> int:
    return v - 0x10000 if v & 0x8000 else v


class Unresolved(Exception):
    def __init__(self, what):
        self.what = what
        super().__init__(what)


@lru_cache(maxsize=1)
def exe_symbols(game_root: Path = ov.ROOT) -> dict[str, int]:
    """Named executable symbols: the config's symbol files plus every promoted
    unit's C name (the ``__asm__("FUN_x")`` label in its source)."""
    values: dict[str, int] = {}
    config = game_root / "config/us"
    for name in ("symbol_addrs.txt", "undefined_syms.txt", "undefined_funcs_auto.txt"):
        path = config / name
        if path.is_file():
            for line in path.read_text().splitlines():
                m = ASSIGN_RE.match(line)
                if m:
                    values.setdefault(m.group(1), int(m.group(2), 0))
    for owner, vram, _size in ov.exe_units():
        values.setdefault(f"FUN_{vram:08x}", vram)
        if owner.startswith("assembly/"):
            continue
        source = game_root / "src" / f"{owner}.c"
        if not source.is_file():
            continue
        text = source.read_text(errors="replace")
        # a declaration's label, `T name(...) __asm__("FUN_x");`, binds the C name
        for m in re.finditer(r"\b([A-Za-z_]\w*)\s*\([^;{)]*\)\s*__asm__\s*\(\s*\"((?:FUN|func)_[0-9A-Fa-f]{8})\"\s*\)", text):
            cname, label = m.group(1), m.group(2)
            addr = int(label[-8:], 16)
            values.setdefault(cname, addr)
            values.setdefault(label, addr)
    return values


@lru_cache(maxsize=None)
def loose_key(level: int, address: int, size: int) -> bytes:
    """The bytes at a place with every address field and constant masked: two
    rows that differ only in a number (a split catalogue row) share it."""
    b = ov.level_text_bytes(level, address, size)
    return b"".join(_mask(int.from_bytes(b[i:i + 4], "little")).to_bytes(4, "little")
                    for i in range(0, len(b), 4))


@lru_cache(maxsize=None)
def loose_places(name: str, level: int) -> tuple[int, ...]:
    """Places in LEVEL of the function NAME calls when the catalogue split it:
    the rows whose masked bytes equal NAME's own."""
    m = ov.OVERLAY_FUNC.match(name)
    row = ov.read_catalogue().get(name) or (m and ov.row_at().get((int(m.group(1)), int(m.group(2), 16))))
    if not row:
        return ()
    key = loose_key(*row.places[0], row.size)
    return tuple(sorted(a for other in ov.read_catalogue().values() if other.size == row.size
                        for lv, a in other.places if lv == level and loose_key(lv, a, other.size) == key))


def place_in_level(name: str, level: int, near: int) -> int:
    matches = ov.places_in_level(name, level) or loose_places(name, level)
    if not matches:
        raise Unresolved(name)
    return min(matches, key=lambda a: abs(a - near))


def resolve_symbol(name: str, level: int, near: int) -> int:
    """A symbol's address in LEVEL, by name."""
    if name == "_gp":
        return ov.GP
    m = ov.OVERLAY_FUNC.match(name)
    if m:
        mm, y = int(m.group(1)), int(m.group(2), 16)
        if mm == level:
            return y
        try:
            return place_in_level(name, level, near)
        except Unresolved:
            return translate_data(name, level)
    m = ov.OVERLAY_DATA.match(name) or ov.OVERLAY_JTBL.match(name)
    if m:
        mm, y = int(m.group(1)), int(m.group(2), 16)
        if mm == level:
            return y
        return translate_data(name, level)
    m = ov.EXE_FUNC.match(name)
    if m:
        y = int(m.group(1), 16)
        if y < ov.RESIDENT_MAX:
            return y
        return place_in_level(f"FUN_{y:08x}", level, near)
    m = re.match(r"^D_([0-9A-Fa-f]{8})(?:_gp)?$", name)
    if m:
        y = int(m.group(1), 16)
        if y < ov.RESIDENT_MAX:
            return y
        raise Unresolved(name)
    addr = exe_symbols().get(name)
    if addr is None:
        # a promoted unit's C name spelled with another hex case in its label
        low = name.lower()
        addr = next((v for k, v in exe_symbols().items() if k.lower() == low), None)
    if addr is None:
        raise Unresolved(name)
    if addr < ov.RESIDENT_MAX:
        return addr
    return place_in_level(f"FUN_{addr:08x}", level, near)


def section_name(elf: ELFFile, shndx) -> str | None:
    if not isinstance(shndx, int):
        return None
    sec = elf.get_section(shndx)
    return sec.name if sec is not None else None


def retail_jtbls(name: str, level: int) -> list[int]:
    """The jump tables the function's retail asm names, in address order
    (translated to LEVEL when the function is a copy there)."""
    path = ov.ASM_DIR / f"{name}.s"
    if not path.exists():
        return []
    out = set()
    for full, mm, z in JTBL_REF.findall(path.read_text()):
        if int(mm) == level:
            out.add(int(z, 16))
        else:
            try:
                out.add(translate_data(full, level))
            except Unresolved:
                pass
    return sorted(out)


# Level data a shared function names (D_LNN_x, jtbl_LNN_x) lives elsewhere in
# every other level. The listing names it only in the canonical copy; each
# copy's %hi/%lo pair or $gp offset at the same place gives that level's
# address. One canonical object must map to one address per level across
# every function that names it, or the copies are not linked alike.
PLACE_INSN = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+(\S+)\s+([^\n]*)")
LEVEL_SYM = re.compile(r"%(hi|lo)\(((?:D|jtbl)_L(\d{2})_([0-9A-Fa-f]{8}))\)")
GP_SYM = re.compile(r"(-?0x[0-9A-Fa-f]+|\d+)\(\$28\)\s*/\* ((?:D|jtbl)_L(\d{2})_([0-9A-Fa-f]{8})) \*/")


def _imm(buf: bytes, off: int) -> int:
    return int.from_bytes(buf[off:off + 4], "little") & 0xFFFF


@lru_cache(maxsize=None)
def place_map() -> tuple[dict, dict]:
    """({(level, symbol): address}, {(level, symbol): {addresses}} conflicts)."""
    seen: dict[tuple[int, str], set[int]] = {}
    for name, row in ov.read_catalogue().items():
        if len(row.places) < 2:
            continue
        path = ov.ASM_DIR / f"{name}.s"
        if not path.exists():
            continue
        lv0, a0 = row.places[0]
        refs = []   # (kind, offset, symbol)
        for line in path.read_text(errors="replace").splitlines():
            m = PLACE_INSN.search(line)
            if not m:
                continue
            off = int(m.group(1), 16) - a0
            for kind, sym, mm, _z in LEVEL_SYM.findall(m.group(3)):
                if int(mm) == lv0:
                    refs.append((kind, off, sym))
            if m.group(2) == "jal":
                j = re.match(r"\s*(FUN_L(\d{2})_[0-9a-f]{8})", m.group(3))
                if j and int(j.group(2)) == lv0:
                    refs.append(("jal", off, j.group(1)))
            g = GP_SYM.search(m.group(3))
            if g and int(g.group(3)) == lv0:
                refs.append(("gp", off, g.group(2)))
            elif not g:
                # an unnamed $gp address (addiu rX, $28, N) of level data
                a = re.search(r"(-?0x[0-9A-Fa-f]+|-?\d+)\(\$28\)|\$28, (-?0x[0-9A-Fa-f]+|-?\d+)\s*$", m.group(3))
                if a:
                    v = (ov.GP + int(a.group(1) or a.group(2), 0)) & 0xFFFFFFFF
                    if v >= ov.RESIDENT_MAX:
                        refs.append(("gp", off, f"D_L{lv0:02d}_{v:08X}"))
        if not refs:
            continue
        try:
            canon = ov.level_text_bytes(lv0, a0, row.size)
        except ValueError:
            continue
        copies = []
        for lv, a in row.places[1:]:
            try:
                copies.append((lv, ov.level_text_bytes(lv, a, row.size)))
            except ValueError:
                pass
        his: dict[str, int] = {}
        for kind, off, sym in refs:
            base = int(sym[-8:], 16)
            if kind == "hi":
                his[sym] = off
                continue
            if kind == "lo" and sym not in his:
                continue
            for lv, buf in copies:
                if kind == "jal":
                    w = int.from_bytes(buf[off:off + 4], "little")
                    seen.setdefault((lv, sym), set()).add(((w & 0x03FFFFFF) << 2) | 0x00000000)
                    continue
                if kind == "gp":
                    theirs = ov.GP + sign16(_imm(buf, off))
                    mine = ov.GP + sign16(_imm(canon, off))
                else:
                    h = his[sym]
                    theirs = (_imm(buf, h) << 16) + sign16(_imm(buf, off))
                    mine = (_imm(canon, h) << 16) + sign16(_imm(canon, off))
                seen.setdefault((lv, sym), set()).add((base + theirs - mine) & 0xFFFFFFFF)
    good = {k: next(iter(v)) for k, v in seen.items() if len(v) == 1}
    bad = {k: v for k, v in seen.items() if len(v) > 1}
    return good, bad


def translate_data(name: str, level: int) -> int:
    good, bad = place_map()
    m = re.match(r"^(D|jtbl)_L(\d{2})_([0-9A-Fa-f]{8})$", name)
    if m and (level, name) not in good:
        name = f"{m.group(1)}_L{m.group(2)}_{m.group(3).upper()}"
    if (level, name) in bad:
        raise Unresolved(f"{name} is several objects in level {level:02d}")
    if (level, name) not in good:
        raise Unresolved(f"{name} in level {level:02d}")
    return good[(level, name)]


_ELVES: dict = {}


def open_elf(path) -> ELFFile:
    """One parsed ELFFile per object (rebuilt when the file changes): the
    canonical check and every copy's check read the same object."""
    path = str(path)
    stamp = os.stat(path).st_mtime_ns
    hit = _ELVES.get(path)
    if hit and hit[0] == stamp:
        return hit[1]
    if hit:
        hit[2].close()
    handle = open(path, "rb")
    elf = ELFFile(handle)
    _ELVES[path] = (stamp, elf, handle)
    return elf


def _symbols(elf: ELFFile) -> list:
    syms = getattr(elf, "_symbols", None)
    if syms is None:
        syms = elf._symbols = list(elf.get_section_by_name(".symtab").iter_symbols())
    return syms


def _is_named(sym) -> bool:
    return bool(sym.name) and (NAMED_SYMBOL.match(sym.name) is not None or sym.name in exe_symbols())


class Placer:
    """Resolves relocations for one function placed at (level, address)."""

    def __init__(self, elf: ELFFile, name: str, level: int, address: int):
        self.elf, self.name, self.level, self.address = elf, name, level, address
        self.text = elf.get_section_by_name(".text")
        self.rodata = elf.get_section_by_name(".rodata")
        self.symtab = elf.get_section_by_name(".symtab")
        if self.symtab is None or self.text is None:
            raise Unresolved("object has no .text/.symtab")
        sym = next((s for s in _symbols(self.elf) if s.name == name), None)
        if sym is None:
            sym = next((s for s in _symbols(self.elf) if s.name.lower() == name.lower()
                        and s["st_size"]), None)
        if sym is None or not sym["st_size"]:
            raise Unresolved(f"{name} not defined by the candidate")
        self.sym = sym
        self.off, self.size = sym["st_value"], sym["st_size"]
        self.base_text = address - self.off
        self.unresolved: list[str] = []
        self.unknown_types: list[tuple[int, int]] = []
        self.rodata_map = None
        self.rodata_error = None
        self.retail = None

    def place_text_offset(self, toff: int, near: int) -> int:
        owner = None
        for sym in _symbols(self.elf):
            if (sym["st_shndx"] == self.sym["st_shndx"] and sym["st_size"]
                    and sym["st_value"] <= toff < sym["st_value"] + sym["st_size"]
                    and sym["st_info"]["type"] == "STT_FUNC"):
                owner = sym
                break
        if owner is None or owner.name == self.name:
            return self.base_text + toff
        return resolve_symbol(owner.name, self.level, near) + (toff - owner["st_value"])

    def is_text_section(self, sym) -> bool:
        return (sym["st_info"]["type"] == "STT_SECTION"
                and section_name(self.elf, sym["st_shndx"]) == ".text")

    def note_unresolved(self, what: str):
        if what not in self.unresolved:
            self.unresolved.append(what)

    def relocations(self):
        # parsed once per object: pyelftools re-parses every entry on each walk
        rels = getattr(self.elf, "_text_rels", None)
        if rels is None:
            rels = [rel for sec in self.elf.iter_sections()
                    if isinstance(sec, RelocationSection) and sec.name == ".rel.text"
                    for rel in sec.iter_relocations()]
            self.elf._text_rels = rels
        for rel in rels:
            if self.off <= rel["r_offset"] < self.off + self.size:
                yield rel

    def sym_of(self, rel):
        return _symbols(self.elf)[rel["r_info_sym"]]

    @staticmethod
    def word_at(buf: bytes, off: int) -> int:
        return int.from_bytes(buf[off:off + 4], "little")

    def hi_los(self, orig: bytes):
        """(hi_rel, lo_rel, hi_imm, lo_imm) pairs. A LO16 pairs with the HI16 of
        the same symbol that precedes it in the relocation table (gas keeps a
        HI16 right before its LO16 even when the lui sits later in the text,
        a loop's back edge), else with any unused HI16 of that symbol; a LO16
        with no HI16 at all (the register was loaded by a literal `lui`)
        stands alone and its field is just the symbol's low half."""
        rels = list(self.relocations())
        his, out = [], []
        for rel in rels:
            if rel["r_info_type"] == R_MIPS_HI16:
                his.append({"rel": rel, "sym": self.sym_of(rel).name, "used": False,
                            "imm": self.word_at(orig, rel["r_offset"]) & 0xFFFF})
        for i, rel in enumerate(rels):
            if rel["r_info_type"] != R_MIPS_LO16:
                continue
            sym = self.sym_of(rel).name
            lo_imm = self.word_at(orig, rel["r_offset"]) & 0xFFFF
            before = [h for h in his if h["sym"] == sym and rels.index(h["rel"]) < i]
            hi = before[-1] if before else next((h for h in his if h["sym"] == sym), None)
            if hi is None:
                out.append((None, rel, 0, lo_imm))
                continue
            hi["used"] = True
            out.append((hi["rel"], rel, hi["imm"], lo_imm))
        for h in his:
            if not h["used"]:
                out.append((h["rel"], None, h["imm"], 0))
        return out

    def local_rodata_offset(self, sym) -> int | None:
        secname = section_name(self.elf, sym["st_shndx"])
        if sym["st_info"]["type"] == "STT_SECTION":
            return 0 if secname == ".rodata" else None
        if secname == ".rodata" and not (sym.name and ov.OVERLAY_JTBL.match(sym.name)):
            return sym["st_value"]
        return None

    def scan_rodata(self, orig: bytes):
        """Place the candidate's unnamed .rodata (its switch tables): the distinct
        offsets it touches, in order, are retail's jump tables in address order."""
        if self.rodata is None:
            return
        touches = []
        for rel in self.relocations():
            rtype = rel["r_info_type"]
            if rtype in (R_MIPS_HI16, R_MIPS_LO16):
                continue
            local = self.local_rodata_offset(self.sym_of(rel))
            if local is None:
                continue
            word = self.word_at(orig, rel["r_offset"])
            if rtype == R_MIPS_GPREL16:
                touches.append(local + sign16(word & 0xFFFF))
            elif rtype == R_MIPS_32:
                touches.append(local + word)
            else:
                touches.append(local)
        for hi_rel, lo_rel, hi_imm, lo_imm in self.hi_los(orig):
            local = self.local_rodata_offset(self.sym_of(hi_rel if hi_rel is not None else lo_rel))
            if local is None:
                continue
            touches.append(local + (hi_imm << 16) + sign16(lo_imm))
        distinct = sorted(set(touches))
        if not distinct:
            return
        tables = retail_jtbls(self.name, self.level)
        if len(tables) != len(distinct):
            self.rodata_error = (f"RODATA candidate references {len(distinct)} table(s), "
                                 f"retail names {len(tables)} for {self.name}")
            return
        self.rodata_map = list(zip(distinct, tables))

    def rodata_address(self, local: int) -> int:
        if self.rodata_map is None:
            raise Unresolved(".rodata (unplaced)")
        off, z = max((pair for pair in self.rodata_map if pair[0] <= local), default=self.rodata_map[0])
        return z + (local - off)

    def resolve_address(self, sym, near: int, addend: int = 0) -> int:
        """SYM + ADDEND in this level. The addend is folded in here because a
        .rodata offset is placed by the table it falls in, not by the section's
        start (several switch tables, each at its own retail address)."""
        secname = section_name(self.elf, sym["st_shndx"])
        is_section = sym["st_info"]["type"] == "STT_SECTION"
        if is_section or (isinstance(sym["st_shndx"], int) and not _is_named(sym)):
            if secname == ".text":
                return self.base_text + sym["st_value"] + addend
            if secname == ".rodata":
                return self.rodata_address(sym["st_value"] + addend)
            if secname and secname.startswith(".rodata.jtbl_"):
                return resolve_symbol(secname[len(".rodata."):], self.level, near) + sym["st_value"] + addend
            raise Unresolved(f"section {secname or sym['st_shndx']}")
        return resolve_symbol(sym.name, self.level, near) + addend

    def apply(self, ours: bytearray, orig: bytes):
        for hi_rel, lo_rel, hi_imm, lo_imm in self.hi_los(orig):
            if hi_rel is None:
                lo_off = lo_rel["r_offset"]
                try:
                    value = self.resolve_address(self.sym_of(lo_rel), self.base_text + lo_off, sign16(lo_imm))
                except Unresolved as e:
                    self.note_unresolved(str(e.what))
                    continue
                ours[lo_off:lo_off + 4] = ((self.word_at(orig, lo_off) & 0xFFFF0000) | (value & 0xFFFF)).to_bytes(4, "little")
                continue
            hi_off = hi_rel["r_offset"]
            ahl = (hi_imm << 16) + sign16(lo_imm)
            try:
                value = self.resolve_address(self.sym_of(hi_rel), self.base_text + hi_off, ahl) & 0xFFFFFFFF
            except Unresolved as e:
                self.note_unresolved(str(e.what))
                continue
            if self.is_text_section(self.sym_of(hi_rel)):
                try:
                    value = self.place_text_offset(ahl, self.base_text + hi_off) & 0xFFFFFFFF
                except Unresolved as e:
                    self.note_unresolved(str(e.what))
                    continue
            if self.retail is not None and lo_rel is not None and self.sym_of(hi_rel).name:
                # a function with several copies in this level: retail's copy is the one
                r_hi = self.word_at(self.retail, hi_off - self.off) & 0xFFFF
                r_lo = self.word_at(self.retail, lo_rel["r_offset"] - self.off) & 0xFFFF
                theirs = ((r_hi << 16) + sign16(r_lo)) & 0xFFFFFFFF
                if theirs != value and same_function(self.level, value, theirs):
                    value = theirs
            new_lo = value & 0xFFFF
            new_hi = ((value - sign16(new_lo)) >> 16) & 0xFFFF
            ours[hi_off:hi_off + 4] = ((self.word_at(orig, hi_off) & 0xFFFF0000) | new_hi).to_bytes(4, "little")
            if lo_rel is not None:
                lo_off = lo_rel["r_offset"]
                ours[lo_off:lo_off + 4] = ((self.word_at(orig, lo_off) & 0xFFFF0000) | new_lo).to_bytes(4, "little")
        for rel in self.relocations():
            rtype = rel["r_info_type"]
            if rtype in (R_MIPS_HI16, R_MIPS_LO16):
                continue
            o = rel["r_offset"]
            word = self.word_at(orig, o)
            near = self.base_text + o
            if rtype == R_MIPS_26:
                try:
                    s = self.resolve_address(self.sym_of(rel), near)
                except Unresolved as e:
                    self.note_unresolved(str(e.what))
                    continue
                addend = (word & 0x03FFFFFF) << 2
                p = self.base_text + o
                target = ((addend + s) & 0x0FFFFFFF) | (p & 0xF0000000)
                if self.is_text_section(self.sym_of(rel)):
                    try:
                        target = self.place_text_offset(addend, near)
                    except Unresolved as e:
                        self.note_unresolved(str(e.what))
                        continue
                if self.retail is not None:
                    r = self.word_at(self.retail, o - self.off)
                    theirs = ((r & 0x03FFFFFF) << 2) | (p & 0xF0000000)
                    if theirs != target and same_function(self.level, target, theirs):
                        target = theirs
                ours[o:o + 4] = ((word & 0xFC000000) | ((target >> 2) & 0x03FFFFFF)).to_bytes(4, "little")
            elif rtype == R_MIPS_GPREL16:
                try:
                    s = self.resolve_address(self.sym_of(rel), near, sign16(word & 0xFFFF))
                except Unresolved as e:
                    self.note_unresolved(str(e.what))
                    continue
                value = (s - ov.GP) & 0xFFFF
                ours[o:o + 4] = ((word & 0xFFFF0000) | value).to_bytes(4, "little")
            elif rtype == R_MIPS_32:
                try:
                    s = self.resolve_address(self.sym_of(rel), near, word)
                except Unresolved as e:
                    self.note_unresolved(str(e.what))
                    continue
                new_word = s & 0xFFFFFFFF
                if self.is_text_section(self.sym_of(rel)):
                    try:
                        new_word = self.place_text_offset(word, near) & 0xFFFFFFFF
                    except Unresolved as e:
                        self.note_unresolved(str(e.what))
                        continue
                ours[o:o + 4] = new_word.to_bytes(4, "little")
            else:
                self.unknown_types.append((o - self.off, rtype))


def same_function(level: int, a: int, b: int) -> bool:
    index = ov.places_index().get(level, {})
    owner = index.get(a)
    return owner is not None and owner == index.get(b)


RESIDENT_SYM = re.compile(r"\b(?:D|FUN|func)_([0-9A-Fa-f]{8})\b")
STUB_INSN = re.compile(r"\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+(\S+)\s+(.*)")


def symbol_offsets(name: str, address: int, resident_too: bool = False) -> dict[int, str]:
    """Offsets where retail's asm references a symbol (%hi/%lo, calls); the
    candidate needs a relocation at each, or a literal address would pass
    here and be wrong in the other levels. Resident symbols are excepted
    unless RESIDENT_TOO (every relocated word, for a masked comparison)."""
    path = ov.ASM_DIR / f"{name}.s"
    out = {}
    if not path.exists():
        return out
    for line in path.read_text(errors="replace").splitlines():
        m = STUB_INSN.match(line)
        if not m:
            continue
        op, args = m.group(2), m.group(3)
        args = args.split("/*", 1)[0]
        if "%hi(" in args or "%lo(" in args or (op in ("jal", "j") and args.strip().startswith("FUN_")):
            r = RESIDENT_SYM.search(args)
            if r and int(r.group(1), 16) < ov.RESIDENT_MAX and not resident_too:
                continue
            out[int(m.group(1), 16) - address] = f"{op} {args.strip()}"
    return out


def check(obj_path, name: str, show: bool = False, place: tuple[int, int] | None = None) -> dict:
    """The verdict for NAME in OBJ_PATH: dict(verdict, exact, size, diff, ...).

    verdict is one of EXACT, BYTES n/size, SIZE ours/retail, LINK <what>,
    RODATA <what>; ``exact`` is True only for EXACT."""
    result = {"method": METHOD, "exact": False, "symbol": name}
    m = ov.OVERLAY_FUNC.match(name)
    if not m:
        result["verdict"] = f"LINK not an overlay function name: {name}"
        return result
    level, address = int(m.group(1)), int(m.group(2), 16)
    canon_address = address
    if place is not None:
        level, address = place
    row = ov.read_catalogue().get(name)
    if row is None:
        result["verdict"] = f"LINK {name} is not in the catalogue"
        return result
    result.update(level=level, address=address, size=row.size)
    elf = open_elf(obj_path)
    try:
        placer = Placer(elf, name, level, address)
    except Unresolved as e:
        result["verdict"] = f"LINK {e.what}"
        return result
    if placer.size != row.size:
        result["verdict"] = f"SIZE ours {placer.size} / retail {row.size}"
        result["ours_size"] = placer.size
        return result
    retail = ov.level_text_bytes(level, address, row.size)
    orig = bytes(elf.get_section_by_name(".text").data())
    placer.scan_rodata(orig)
    if placer.rodata_error:
        result["verdict"] = placer.rodata_error
        return result
    ours = bytearray(orig)
    placer.retail = retail
    placer.apply(ours, orig)
    if placer.unresolved or placer.unknown_types:
        parts = list(placer.unresolved)
        parts += [f"unknown relocation type {t} at +0x{o:x}" for o, t in placer.unknown_types]
        result["verdict"] = "LINK " + ", ".join(parts)
        return result
    ours_func = bytes(ours[placer.off:placer.off + placer.size])
    rows = []
    for i in range(0, row.size, 4):
        a = int.from_bytes(ours_func[i:i + 4], "little")
        b = int.from_bytes(retail[i:i + 4], "little")
        if a != b:
            rows.append((i, a, b))
    if not rows:
        relocated = {rel["r_offset"] - placer.off for rel in placer.relocations()}
        missing = sorted(set(symbol_offsets(name, canon_address)) - relocated)
        if missing:
            refs = symbol_offsets(name, canon_address)
            result["verdict"] = ("LINK literal address where retail references a symbol: "
                                 + ", ".join(f"+0x{o:x} ({refs[o]})" for o in missing[:4]))
            return result
        result.update(verdict="EXACT", exact=True, differing_words=0)
        return result
    result.update(verdict=f"BYTES {len(rows)}/{row.size // 4}", differing_words=len(rows),
                  words=[[i, a, b] for i, a, b in rows])
    if show:
        import rabbitizer as rz
        lines = []
        for i, a, b in rows:
            da = rz.Instruction(a, vram=address + i, category=rz.InstrCategory.R5900).disassemble()
            db = rz.Instruction(b, vram=address + i, category=rz.InstrCategory.R5900).disassemble()
            lines.append(f"  +{i:4x}  ours {da:40s} retail {db}")
        result["diff"] = lines
    return result


MEM_OPS = {0x1A, 0x1B, 0x1E, 0x1F, *range(0x20, 0x30), 0x31, 0x36, 0x37, 0x39, 0x3E, 0x3F}


def _mask(w: int) -> int:
    """The word without its address field (jal target, %hi, %lo, $gp offset)."""
    op, rs = w >> 26, (w >> 21) & 31
    if op in (2, 3):
        return w & 0xFC000000
    if op == 0x0F or (rs == 28 and op >= 8):
        return w & 0xFFFF0000
    if (op in MEM_OPS or op in (0x09, 0x0D, 0x19)) and rs != 29:
        return w & 0xFFFF0000
    return w


def copy_check(obj_path, name: str, place: tuple[int, int]) -> dict:
    """A copy of an EXACT function: placed and compared like the canonical one
    when every symbol resolves in that level; otherwise the copy must equal the
    canonical retail bytes except in address fields our object relocates (a
    jal target, %hi/%lo or $gp offset), so the same C builds it once linked."""
    full = check(obj_path, name, place=place)
    if full["exact"]:
        return full
    row = ov.read_catalogue()[name]
    m = ov.OVERLAY_FUNC.match(name)
    canon = ov.level_text_bytes(int(m.group(1)), int(m.group(2), 16), row.size)
    copy = ov.level_text_bytes(place[0], place[1], row.size)
    try:
        placer = Placer(open_elf(obj_path), name, *place)
    except Unresolved:
        return full
    relocated = {rel["r_offset"] - placer.off for rel in placer.relocations()}
    for i in range(0, row.size, 4):
        a = int.from_bytes(canon[i:i + 4], "little")
        b = int.from_bytes(copy[i:i + 4], "little")
        if a != b and (i not in relocated or _mask(a) != _mask(b)):
            # a word our object does not relocate differs: the same C cannot
            # build this place, it is another function the catalogue merged
            return {**full, "not_a_copy": i not in relocated}
    return {**full, "verdict": "EXACT", "exact": True, "differing_words": 0, "method": METHOD + "+copy"}
