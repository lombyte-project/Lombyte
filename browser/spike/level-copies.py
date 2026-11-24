#!/usr/bin/env python3
"""Finds level-program functions that are word-for-word copies of boot functions.

The level program carries its own copies of some boot math helpers (FUN_L00_001ff408 is
vector_length_xy at 0x1f9b20). For every FUN_L00_001fxxxx / func_L00_001FXXXX the recovered
sources name, the words from its address through `jr ra` and its delay slot are searched for in
the boot ELF's loadable segments; a copy that sits at exactly one boot address is printed as
`level_symbol boot_symbol`. Copies with jal/j (address-relative) words are compared with the
target field masked. Static analysis only; it prints, it does not edit.

Usage: browser/spike/level-copies.py [level=0] [--elf build/rea/SCUS_971.99]
"""
import json
import pathlib
import re
import struct
import sys

level = int(next((a for a in sys.argv[1:] if not a.startswith('--') and sys.argv[sys.argv.index(a) - 1] != '--elf'), '0'))
elf_path = sys.argv[sys.argv.index('--elf') + 1] if '--elf' in sys.argv else 'build/rea/SCUS_971.99'
with open(f'config/us/levels/level-{level:02d}.json', encoding='utf-8') as handle:
    text = next(s for s in json.load(handle)['sections'] if s['name'] == '.text')
code = open(f'config/us/overlays/level_{level:02d}/text.bin', 'rb').read()
elf = open(elf_path, 'rb').read()
phoff, phnum = struct.unpack_from('<I', elf, 0x1C)[0], struct.unpack_from('<H', elf, 0x2C)[0]
segments = []
for index in range(phnum):
    kind, offset, vaddr, _, filesz = struct.unpack_from('<5I', elf, phoff + index * 0x20)
    if kind == 1:
        segments.append((vaddr, elf[offset:offset + filesz]))

def mask(word):
    return word & 0xFC000000 if (word >> 26) in (2, 3) else word

names = set()
prefix = f'L{level:02d}_'
for path in list(pathlib.Path('src').rglob('*.c')) + list(pathlib.Path('include').rglob('*.h')):
    for match in re.finditer(rf'\b(?:FUN|func)_{prefix}(00[0-9a-fA-F]{{6}})\b', path.read_text(errors='ignore')):
        names.add(int(match.group(1), 16))

for address in sorted(names):
    start = address - text['addr']
    if not 0 <= start < len(code):
        continue
    words = []
    for at in range(start, min(start + 4096, len(code)), 4):
        words.append(struct.unpack_from('<I', code, at)[0])
        if len(words) >= 2 and words[-2] == 0x03E00008:
            break
    else:
        continue
    pattern = [mask(w) for w in words]
    found = []
    for vaddr, data in segments:
        first = struct.pack('<I', words[0]) if mask(words[0]) == words[0] else None
        positions = range(0, len(data) - 4 * len(words) + 1, 4)
        if first:
            positions, at = [], data.find(first)
            while at >= 0:
                if at % 4 == 0:
                    positions.append(at)
                at = data.find(first, at + 1)
        for at in positions:
            if at + 4 * len(pattern) <= len(data) and all(mask(struct.unpack_from('<I', data, at + 4 * k)[0]) == p for k, p in enumerate(pattern)):
                found.append(vaddr + at)
    # Beyond the 0x1fxxxx math helpers: at least 12 words, and every masked jal/j must point the
    # same distance away in both copies (the level copy calls the level's copies of the same
    # callees, laid out alike), so a different callee cannot pass for a copy.
    if len(found) == 1 and (address >> 16) != 0x1f:
        boot = next(data for vaddr, data in segments if vaddr <= found[0] < vaddr + len(data))
        base = next(vaddr for vaddr, data in segments if vaddr <= found[0] < vaddr + len(data))
        deltas = set()
        for k, w in enumerate(words):
            if (w >> 26) in (2, 3):
                b = struct.unpack_from('<I', boot, found[0] - base + 4 * k)[0]
                deltas.add(((w & 0x3FFFFFF) - (b & 0x3FFFFFF)) * 4)
        deltas.add(address - found[0])
        if len(words) < 12 or len(deltas) != 1:
            print(f'# FUN_{prefix}{address:08x}: one boot match, not accepted ({len(words)} words, call offsets {sorted(deltas)})', file=sys.stderr)
            continue
    if len(found) == 1:
        print(f'FUN_{prefix}{address:08x} FUN_{found[0]:08x} {len(words)}')
    else:
        print(f'# FUN_{prefix}{address:08x}: {len(found)} boot matches', file=sys.stderr)
