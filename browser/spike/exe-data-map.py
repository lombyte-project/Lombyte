#!/usr/bin/env python3
"""Where a level program keeps its copy of the executable's data.

A level program is linked with the executable's game units, so the engine functions it contains
are copies at other addresses, and the data they use is a copy at other addresses too
(scripts/lib/overlay_exe_data.py: "where the executable's code reads a data object at A, the same
function's copy in level N reads A + d"). The recovered boot sources name such data by its
executable address (`D_001D80B0`), level sources by its level address (`D_L00_001BC1B0`). In one
address space they are the same object, so the browser build has to pin both names at the level
address.

This compares each executable function with its level copy (config/us/levels/functions.tsv) and
pairs the %hi/%lo address constants the two form at the same offsets, using the same pairing as
overlay_exe_data.py. Static comparison of local files only. Output:
build/spike/exe-data-lNN.json = { "0x<executable address>": "0x<level address>", ... }

Usage: browser/spike/exe-data-map.py [level=0] [--elf build/rea/SCUS_971.99]
"""
import json
import os
import struct
import sys

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')


def addresses(code):
    """Offset of each %lo use -> the address its %hi/%lo pair forms."""
    hi = {}
    out = {}
    for off in range(0, len(code) - 3, 4):
        w = int.from_bytes(code[off:off + 4], 'little')
        op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
        if op == 0x0F:  # lui
            hi[rt] = imm << 16
            continue
        if rs in hi and (op in (0x09, 0x0D, 0x19) or op >= 0x20):
            lo = imm - 0x10000 if imm & 0x8000 and op != 0x0D else imm
            out[off] = (hi[rs] + lo) & 0xFFFFFFFF
            if op in (0x09, 0x0D, 0x19) and rt == rs:
                hi.pop(rs, None)
    return out


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    elf_path = 'build/rea/SCUS_971.99'
    if '--elf' in sys.argv:
        elf_path = sys.argv[sys.argv.index('--elf') + 1]
        args.remove(elf_path)
    level = int(args[0]) if args else 0
    elf = open(os.path.join(ROOT, elf_path), 'rb').read()
    phoff, phnum = struct.unpack_from('<I', elf, 0x1C)[0], struct.unpack_from('<H', elf, 0x2C)[0]
    segments = [struct.unpack_from('<5I', elf, phoff + i * 0x20) for i in range(phnum)]

    def boot(address, size):
        for kind, offset, vaddr, _, filesz in segments:
            if kind == 1 and vaddr <= address and address + size <= vaddr + filesz:
                return elf[offset + address - vaddr:offset + address - vaddr + size]
        return None

    with open(os.path.join(ROOT, f'config/us/levels/level-{level:02d}.json'), encoding='utf-8') as handle:
        text = next(s for s in json.load(handle)['sections'] if s['name'] == '.text')
    code = open(os.path.join(ROOT, f'config/us/overlays/level_{level:02d}/text.bin'), 'rb').read()

    votes = {}
    functions = compared = 0
    with open(os.path.join(ROOT, 'config/us/levels/functions.tsv'), encoding='utf-8') as handle:
        for line in handle:
            fields = line.rstrip('\n').split('\t')
            if line.startswith('#') or len(fields) < 6 or fields[1] != 'exe':
                continue
            place = next((p for p in fields[5].split(',') if p.startswith(f'{level:02d}:')), None)
            if not place:
                continue
            functions += 1
            size = int(fields[2])
            boot_address = int(fields[0].split('_')[-1], 16)
            level_address = int(place.split(':')[1], 16)
            original = boot(boot_address, size)
            if original is None or not text['addr'] <= level_address <= text['addr'] + len(code) - size:
                continue
            copy = code[level_address - text['addr']:level_address - text['addr'] + size]
            a, b = addresses(original), addresses(copy)
            compared += 1
            for off, source in a.items():
                target = b.get(off)
                if target is not None:
                    votes.setdefault(source, {}).setdefault(target, 0)
                    votes[source][target] += 1

    mapping = {}
    ambiguous = 0
    for source, targets in votes.items():
        if len(targets) == 1:
            target = next(iter(targets))
            if target != source:
                mapping[f'0x{source:08x}'] = f'0x{target:08x}'
        else:
            ambiguous += 1
    out = os.path.join(ROOT, 'build', 'spike', f'exe-data-l{level:02d}.json')
    with open(out, 'w', encoding='utf-8') as handle:
        json.dump(dict(sorted(mapping.items())), handle, indent=0)
    print(f'level {level}: {compared} of {functions} executable functions compared, '
          f'{len(mapping)} data addresses move, {ambiguous} ambiguous (left alone) -> {os.path.relpath(out, ROOT)}')


if __name__ == '__main__':
    main()
