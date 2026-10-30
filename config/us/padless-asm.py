#!/usr/bin/env python3
"""Normalize SN cc1 output for Ps2EeAs and drop section tail padding.

`normalize IN OUT` rewrites GNU `alias = function` assignments into
co-located labels (Ps2EeAs rejects the assignment form).
`finish PADDED OUT [REFERENCE]` removes only the `.text` section tail padding
after the last sized function and adds empty `.data`/`.bss` sections so object
comparison sees the GNU layout.  With a REFERENCE object assembled by GNU as
from the same source, data sections Ps2EeAs rounded up to their alignment are
trimmed back to the reference size: data sizes do not depend on the
assembler, and the removed tail must be zero bytes nothing refers to.  Every
check fails closed; no target bytes, addresses or expected lengths are inputs.
"""
import re
import struct
import sys


def normalize_aliases(text):
    labels = set(re.findall(r"^\s*([\w.$]+):\s*$", text, re.M))
    pattern = re.compile(r"^\s*([\w.$]+)\s*=\s*([\w.$]+)\s*$", re.M)
    aliases = {}
    for match in pattern.finditer(text):
        alias, target = match.groups()
        if target in labels:
            aliases.setdefault(target, []).append(alias)
    text = pattern.sub(lambda m: "" if m[2] in aliases else m[0], text)
    for target, names in aliases.items():
        text = re.sub(r"^(\s*)" + re.escape(target) + r":\s*$",
                      lambda m: "".join(name + ":\n" for name in names) + target + ":",
                      text, count=1, flags=re.M)
    return text


def pin_labels_before_align(text):
    """Keep a local label that precedes a loop alignment at its own address.

    cc1 can emit `$La:` `.p2align 3` `$Lb:` when a branch target ends up just
    before an aligned loop head.  GNU as (and retail) leave `$La` before the
    alignment nops; Ps2EeAs moves it past them.  `$La = .` defines the label
    in place without that move.  Only a local label that is followed, after
    the alignment, by another label is rewritten.
    """
    skip = r"(?:[ \t]*(?:\.set[ \t]+\w+|\.loc[^\n]*|#[^\n]*)?\r?\n)*"
    pattern = re.compile(r"^(\$L\w+):[ \t]*(\r?\n" + skip + r"[ \t]*\.p2align[ \t]+\d[^\n]*\r?\n"
                         + skip + r"\$L\w+:)", re.M)
    return pattern.sub(lambda m: m.group(1) + " = ." + m.group(2), text)


def unpad(data):
    if data[:7] != b"\x7fELF\x01\x01\x01" or struct.unpack_from("<H", data, 16)[0] != 1:
        raise ValueError("expected a little-endian ELF32 relocatable object")
    shoff = struct.unpack_from("<I", data, 32)[0]
    shsize, count, names_index = struct.unpack_from("<HHH", data, 46)
    if shsize != 40 or shoff + count * shsize > len(data):
        raise ValueError("invalid section table")
    headers = [struct.unpack_from("<10I", data, shoff + i * shsize) for i in range(count)]
    nh = headers[names_index]
    names = data[nh[4]:nh[4] + nh[5]]
    text_indices = [i for i, h in enumerate(headers)
                    if names[h[0]:].split(b"\0")[0] == b".text"]
    if len(text_indices) != 1:
        raise ValueError("expected exactly one .text section")
    index = text_indices[0]
    text = headers[index]
    if text[1] != 1 or text[2] & 6 != 6 or text[4] + text[5] > len(data):
        raise ValueError("invalid executable .text section")
    symbols = []
    for h in headers:
        if h[1] == 2:
            if h[9] != 16 or h[5] % 16:
                raise ValueError("invalid symbol table")
            for offset in range(h[4], h[4] + h[5], 16):
                sym = struct.unpack_from("<IIIBBH", data, offset)
                if sym[5] == index:
                    symbols.append(sym)
    funcs = [s for s in symbols if s[3] & 15 == 2 and s[2]]
    if not funcs:
        raise ValueError("no sized function symbols; cannot infer code extent")
    end = max(s[1] + s[2] for s in funcs)
    padding = text[5] - end
    if padding < 0 or padding >= max(text[8], 1) or end % 4:
        raise ValueError("function extent does not explain section tail padding")
    if any(data[text[4] + end:text[4] + text[5]]):
        raise ValueError("nonzero bytes after final function")
    if any(s[1] > end or (s[1] == end and s[3] & 15 not in (0, 3))
           or (s[3] & 15 != 3 and s[1] + s[2] > end) for s in symbols):
        raise ValueError("symbol refers to removed padding")
    for h in headers:
        if h[1] in (4, 9) and h[7] == index:
            stride = 12 if h[1] == 4 else 8
            if h[9] != stride or h[5] % stride:
                raise ValueError("invalid relocation table")
            if any(struct.unpack_from("<I", data, offset)[0] >= end
                   for offset in range(h[4], h[4] + h[5], stride)):
                raise ValueError("relocation refers to removed padding")
    result = bytearray(data)
    struct.pack_into("<I", result, shoff + index * shsize + 20, end)
    return bytes(result)


def _sections(data):
    shoff = struct.unpack_from("<I", data, 32)[0]
    shsize, count, names_index = struct.unpack_from("<HHH", data, 46)
    headers = [struct.unpack_from("<10I", data, shoff + i * shsize) for i in range(count)]
    nh = headers[names_index]
    names = data[nh[4]:nh[4] + nh[5]]
    return shoff, shsize, headers, [names[h[0]:].split(b"\0")[0] for h in headers]


def trim_data_sections(data, reference):
    shoff, shsize, headers, names = _sections(data)
    _, _, ref_headers, ref_names = _sections(reference)
    ref_size = {n: h[5] for n, h in zip(ref_names, ref_headers) if h[1] == 1}
    result = bytearray(data)
    for index, (h, name) in enumerate(zip(headers, names)):
        if h[1] != 1 or h[2] & 4 or name not in ref_size or h[5] <= ref_size[name]:
            continue
        end = ref_size[name]
        if h[5] - end >= max(h[8], 1) or any(data[h[4] + end:h[4] + h[5]]):
            raise ValueError("%s: tail beyond the reference is not alignment padding" % name.decode())
        for s in headers:
            if s[1] == 2:
                for offset in range(s[4], s[4] + s[5], 16):
                    sym = struct.unpack_from("<IIIBBH", data, offset)
                    if sym[5] == index and sym[1] > end:
                        raise ValueError("symbol refers to removed padding")
            if s[1] in (4, 9) and s[7] == index:
                stride = 12 if s[1] == 4 else 8
                if any(struct.unpack_from("<I", data, offset)[0] >= end
                       for offset in range(s[4], s[4] + s[5], stride)):
                    raise ValueError("relocation refers to removed padding")
        struct.pack_into("<I", result, shoff + index * shsize + 20, end)
    return bytes(result)


def fix_gprel_externs(data):
    """Rebase GPREL16 addends against undefined symbols for GNU ld.

    Ps2EeAs relaxes a small-data access to $gp and writes the in-place
    addend as A - gp0 (gp0 = the .reginfo gp_value) for every symbol.  GNU
    ld adds gp0 back only for local symbols, so a relaxed access to an
    undefined symbol that has no `.extern` size is off by gp0 and overflows GPREL16 at link
    time.  GNU as writes such an addend without the -gp0 term, so add gp0
    back for exactly those relocations; the instruction bytes after linking
    are what Ps2EeAs meant.  No-op for objects without that case.
    """
    shoff, shsize, headers, names = _sections(data)
    gp0 = 0
    for h, name in zip(headers, names):
        if h[1] == 0x70000006:  # SHT_MIPS_REGINFO
            gp0 = struct.unpack_from("<i", data, h[4] + 20)[0]
    if not gp0:
        return data
    result = bytearray(data)
    for h in headers:
        if h[1] != 9 or h[9] != 8:
            continue
        symtab = headers[h[6]]
        target = headers[h[7]]
        for offset in range(h[4], h[4] + h[5], 8):
            r_offset, r_info = struct.unpack_from("<II", data, offset)
            if r_info & 0xFF != 7:  # R_MIPS_GPREL16
                continue
            sym = struct.unpack_from("<IIIBBH", data, symtab[4] + (r_info >> 8) * 16)
            # Only undefined symbols without an `.extern NAME, SIZE`: Ps2EeAs
            # writes a correct (gp0-free) addend for the sized ones.
            if sym[3] >> 4 == 0 or sym[5] != 0 or sym[2] != 0:
                continue
            where = target[4] + r_offset
            word = struct.unpack_from("<I", result, where)[0]
            addend = ((word & 0xFFFF) ^ 0x8000) - 0x8000
            addend += gp0
            if not -0x8000 <= addend < 0x8000:
                raise ValueError("GPREL16 addend out of range after rebase")
            struct.pack_into("<I", result, where, (word & 0xFFFF0000) | (addend & 0xFFFF))
    return bytes(result)


def add_empty_sections(data):
    shoff = struct.unpack_from("<I", data, 32)[0]
    shsize, count, names_index = struct.unpack_from("<HHH", data, 46)
    headers = [list(struct.unpack_from("<10I", data, shoff + i * shsize)) for i in range(count)]
    nh = headers[names_index]
    names = bytearray(data[nh[4]:nh[4] + nh[5]])
    present = {bytes(names[h[0]:]).split(b"\0")[0] for h in headers}
    added = []
    for name, kind in ((b".data", 1), (b".bss", 8)):
        if name in present:
            continue
        headers.append([len(names), kind, 3, 0, len(data), 0, 0, 0, 1, 0])
        names.extend(name + b"\0")
        added.append(name.decode())
    if not added:
        return data
    result = bytearray(data)
    nh[4], nh[5] = len(result), len(names)
    result.extend(names)
    result.extend(b"\0" * (-len(result) % 4))
    struct.pack_into("<I", result, 32, len(result))
    struct.pack_into("<H", result, 48, len(headers))
    for h in headers:
        result.extend(struct.pack("<10I", *h))
    return bytes(result)


def apply_la_gprel_policy(assembly):
    """Hoist `.extern` size directives for `la`-only small-data symbols.

    Ps2EeAs is a single-pass assembler: a bare symbol reference is relaxed to
    gp-relative only when the symbol's `.extern NAME, SIZE` directive has
    already been seen, while cc1 emits every directive at end of file.  A
    symbol that is only ever the address operand of `la` therefore expands to
    lui+addiu instead of retail's single `addiu $r,$gp,%gprel(NAME)`.  Only
    symbols that (a) declare a size in 1..8 and (b) never appear as a memory
    operand are moved; hoisting memory-operand externs over-relaxes accesses
    that retail keeps absolute (measured regression).
    """
    import re
    extern_line = re.compile(r"^[ \t]*\.extern[ \t]+([\w.$]+)[ \t]*,[ \t]*(\d+)[ \t]*(?:#.*)?$")
    la_line = re.compile(r"^[ \t]*la[ \t]+\$?\w+[ \t]*,[ \t]*([\w.$]+)[ \t]*(?:#.*)?$")
    label_line = re.compile(r"^[ \t]*([\w.$]+):")
    lines = assembly.splitlines(keepends=True)
    # SN's Windows driver writes CRLF; strip the CR for matching only.
    stripped = [line.rstrip("\r\n") for line in lines]
    entries = []
    for index, line in enumerate(stripped):
        match = extern_line.match(line)
        if match:
            entries.append((index, match.group(1), int(match.group(2))))
    if not entries:
        return assembly
    extern_indices = {index for index, _, _ in entries}
    labels = {m.group(1) for m in (label_line.match(line) for line in stripped) if m}
    la_lines = {index for index, line in enumerate(stripped) if la_line.match(line)}
    hoisted = set()
    for index, name, size in entries:
        if name in labels or not 1 <= size <= 8:
            continue
        pattern = re.compile(r"(?<![\w.$])" + re.escape(name) + r"(?![\w.$])")
        mentions = [i for i, line in enumerate(stripped) if pattern.search(line) and i not in extern_indices]
        if not mentions or any(mention not in la_lines for mention in mentions):
            continue
        if min(mentions) >= index:
            continue  # directive already precedes its first use
        hoisted.add(name)
    if not hoisted:
        return assembly
    moved = [line for index, name, _ in entries if name in hoisted for line in [lines[index]]]
    body = [line for index, line in enumerate(lines)
            if not (index in extern_indices and extern_line.match(stripped[index]).group(1) in hoisted)]
    insert_at = next((i for i, line in enumerate(stripped) if line.strip() == ".text"), 0)
    return "".join(body[:insert_at] + moved + body[insert_at:])


def hoist_sda_externs(assembly, source):
    """Move `.extern NAME, SIZE` up to where the source declares NAME small.

    SN cc1 writes its `.extern` directives at the end of the file and leaves
    the choice between $gp and lui to the assembler.  Ps2EeAs is single-pass:
    it relaxes a bare symbol access to $gp only when the size is already
    known.  The source says which globals retail reads through $gp: those
    declared `__attribute__((sda))` (the game compiler's spelling, ignored by
    SN cc1); their directives go first.  When a `<name>_lui` alias of
    the symbol is declared before its first sda declaration, the functions
    in between read it through lui (retail mixes both when a unit joins files
    that did and did not define the symbol), so the directive goes in front
    of the first function defined after the sda declaration instead.  A
    NON_MATCHING `#else` draft is not compiled, so its declarations do not
    count, unless the file defines NON_MATCHING to compile the drafts.
    """
    if not re.search(r"^#\s*define\s+NON_MATCHING\b", source, re.M):
        source = re.sub(r"^(#ifndef NON_MATCHING\n(?:INCLUDE_ASM[^\n]*\n)*)#else\n.*?^#endif",
                        r"\1#endif", source, flags=re.M | re.S)
    sda = {}
    for match in re.finditer(r"^extern[^;]*__attribute__\(\(sda\)\)[^;]*;", source, re.M):
        decl = match.group(0)
        label = re.search(r'__asm__\s*\(\s*"([\w.$]+)"', decl)
        name = re.search(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*(?:__asm__|__attribute__)", decl)
        key = label.group(1) if label else name.group(1) if name else None
        if key and key not in sda:
            sda[key] = match.start()
    absolute = {}
    for match in re.finditer(r'^extern[^;(]*\b\w+_lui\s*__asm__\s*\(\s*"([\w.$]+)"\s*\)[^;]*;', source, re.M):
        absolute.setdefault(match.group(1), match.start())
    lines = assembly.split("\n")
    functions = []  # (asm line of the function's .globl/.ent, source offset of its definition)
    for index, line in enumerate(lines):
        match = re.match(r"\s*\.ent\s+([\w.$]+)", line)
        if not match:
            continue
        start = index
        while start > 0 and re.match(r"\s*\.(?:globl|align|type)\b", lines[start - 1]):
            start -= 1
        # A named function is defined under its C name and labelled with __asm__("FUN_...").
        names = [match.group(1)] + re.findall(
            r"\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*__asm__\s*\(\s*\"" + re.escape(match.group(1)) + r"\"", source)
        body = re.search(r"^[^\n;#]*\b(?:" + "|".join(map(re.escape, names)) + r")\s*\([^;{]*\)\s*\{", source, re.M)
        functions.append((start, body.start() if body else -1))
    placed = {}
    for line in lines:
        match = re.match(r"\s*\.extern\s+([\w.$]+)\s*,", line)
        if match and match.group(1) in sda:
            symbol = match.group(1)
            after = [start for start, offset in functions if offset > sda[symbol]]
            late = absolute.get(symbol, len(source)) < sda[symbol] and after
            placed.setdefault(min(after) if late else 0, []).append(line)
    result = []
    for index, line in enumerate(lines):
        result.extend(placed.get(index, []))
        match = re.match(r"\s*\.extern\s+([\w.$]+)\s*,", line)
        if not (match and match.group(1) in sda):
            result.append(line)
    return "\n".join(result)


def reference_assembly(assembly):
    """Drop `.set noreorder` and `.set nomacro` from the copy GNU as reads.

    GNU as only measures the data sections for `finish`, so its .text does
    not matter.  It asserts (tc-mips.c:11454) when a relaxable access sits in
    a noreorder delay slot and an unsized symbol is loaded later in the same
    frag; in reorder mode the access gets its own frag.
    """
    return re.sub(r"^[ \t]*\.set[ \t]+no(?:reorder|macro)[ \t]*\r?\n", "", assembly, flags=re.M)


def main(argv):
    if len(argv) == 5 and argv[1] == "externs":
        _, _, source, destination, c_source = argv
        assembly = hoist_sda_externs(open(source).read(), open(c_source, errors="replace").read())
        open(destination, "w").write(assembly)
        return
    if len(argv) == 4 and argv[1] == "reference":
        open(argv[3], "w").write(reference_assembly(open(argv[2]).read()))
        return
    if len(argv) not in (4, 5):
        raise SystemExit("usage: padless-asm.py normalize IN OUT [POLICY] | finish IN OUT [REFERENCE] | "
                         "externs IN OUT SOURCE | reference IN OUT")
    mode, source, destination = argv[1:4]
    policy = argv[4] if len(argv) == 5 else "none"
    data = open(source, "rb").read()
    if mode == "normalize":
        assembly = pin_labels_before_align(normalize_aliases(data.decode()))
        if policy == "la-gprel":
            assembly = apply_la_gprel_policy(assembly)
        elif policy != "none":
            raise SystemExit("unknown assembler policy: " + policy)
        open(destination, "w").write(assembly)
    elif mode == "finish":
        data = fix_gprel_externs(unpad(data))
        if len(argv) == 5:
            data = trim_data_sections(data, open(argv[4], "rb").read())
        open(destination, "wb").write(add_empty_sections(data))
    else:
        raise SystemExit("unknown mode: " + mode)


if __name__ == "__main__":
    main(sys.argv)
