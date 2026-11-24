#!/usr/bin/env python3
"""Static listing of EE instructions from the local boot ELF, for decoding assembly-only helpers
into portable C by hand. It prints text; it does not execute anything. Unknown encodings are
shown as raw words rather than guessed.

Usage: browser/spike/ee-disasm.py 0x1fa688 [count=24] [--elf build/rea/SCUS_971.99]
       browser/spike/ee-disasm.py 0x250df8 [count] --level 0     (level program text, extracted by
                                                                  scripts/overlay-extract.py)
"""
import struct
import sys

R = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
     's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7', 't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']
SPECIAL = {0x00: 'sll', 0x02: 'srl', 0x03: 'sra', 0x04: 'sllv', 0x06: 'srlv', 0x07: 'srav', 0x08: 'jr',
           0x09: 'jalr', 0x0A: 'movz', 0x0B: 'movn', 0x10: 'mfhi', 0x12: 'mflo', 0x18: 'mult', 0x19: 'multu',
           0x1A: 'div', 0x1B: 'divu', 0x20: 'add', 0x21: 'addu', 0x22: 'sub', 0x23: 'subu', 0x24: 'and',
           0x25: 'or', 0x26: 'xor', 0x27: 'nor', 0x2A: 'slt', 0x2B: 'sltu', 0x2C: 'dadd', 0x2D: 'daddu',
           0x2F: 'dsubu', 0x38: 'dsll', 0x3A: 'dsrl', 0x3B: 'dsra', 0x3C: 'dsll32', 0x3E: 'dsrl32', 0x3F: 'dsra32'}
IMM = {0x08: 'addi', 0x09: 'addiu', 0x0A: 'slti', 0x0B: 'sltiu', 0x0C: 'andi', 0x0D: 'ori', 0x0E: 'xori',
       0x18: 'daddi', 0x19: 'daddiu'}
MEM = {0x20: 'lb', 0x21: 'lh', 0x23: 'lw', 0x24: 'lbu', 0x25: 'lhu', 0x27: 'lwu', 0x28: 'sb', 0x29: 'sh',
       0x2B: 'sw', 0x37: 'ld', 0x3F: 'sd', 0x1E: 'lq', 0x1F: 'sq'}
BRANCH = {0x04: 'beq', 0x05: 'bne', 0x14: 'beql', 0x15: 'bnel'}
BRANCHZ = {0x06: 'blez', 0x07: 'bgtz', 0x16: 'blezl', 0x17: 'bgtzl'}
REGIMM = {0x00: 'bltz', 0x01: 'bgez', 0x02: 'bltzl', 0x03: 'bgezl'}
FPU_S = {0x00: 'add.s', 0x01: 'sub.s', 0x02: 'mul.s', 0x03: 'div.s', 0x04: 'sqrt.s', 0x05: 'abs.s',
         0x06: 'mov.s', 0x07: 'neg.s', 0x16: 'rsqrt.s', 0x18: 'adda.s', 0x19: 'suba.s', 0x1A: 'mula.s',
         0x1C: 'madd.s', 0x1D: 'msub.s', 0x24: 'cvt.w.s', 0x28: 'max.s', 0x29: 'min.s',
         0x30: 'c.f.s', 0x32: 'c.eq.s', 0x34: 'c.lt.s', 0x36: 'c.le.s'}

BC = 'xyzw'
VU_NORMAL = {0x1C: 'vmulq', 0x1D: 'vmaxi', 0x1E: 'vmuli', 0x1F: 'vminii', 0x20: 'vaddq', 0x21: 'vmaddq',
             0x22: 'vaddi', 0x23: 'vmaddi', 0x24: 'vsubq', 0x25: 'vmsubq', 0x26: 'vsubi', 0x27: 'vmsubi',
             0x28: 'vadd', 0x29: 'vmadd', 0x2A: 'vmul', 0x2B: 'vmax', 0x2C: 'vsub', 0x2D: 'vmsub',
             0x2E: 'vopmsub', 0x2F: 'vmini', 0x30: 'viadd', 0x31: 'visub', 0x32: 'viaddi', 0x34: 'viand',
             0x35: 'vior', 0x38: 'vcallms', 0x39: 'vcallmsr'}
for _base, _name in ((0x00, 'vadd'), (0x04, 'vsub'), (0x08, 'vmadd'), (0x0C, 'vmsub'), (0x10, 'vmax'),
                     (0x14, 'vmini'), (0x18, 'vmul')):
    for _i in range(4):
        VU_NORMAL[_base + _i] = _name + BC[_i]
VU_SPECIAL = {0x10: 'vitof0', 0x11: 'vitof4', 0x12: 'vitof12', 0x13: 'vitof15', 0x14: 'vftoi0', 0x15: 'vftoi4',
              0x16: 'vftoi12', 0x17: 'vftoi15', 0x1C: 'vmulaq', 0x1D: 'vabs', 0x1E: 'vmulai', 0x1F: 'vclipw',
              0x20: 'vaddaq', 0x21: 'vmaddaq', 0x22: 'vaddai', 0x23: 'vmaddai', 0x24: 'vsubaq', 0x25: 'vmsubaq',
              0x26: 'vsubai', 0x27: 'vmsubai', 0x28: 'vadda', 0x29: 'vmadda', 0x2A: 'vmula', 0x2C: 'vsuba',
              0x2D: 'vmsuba', 0x2E: 'vopmula', 0x2F: 'vnop', 0x30: 'vmove', 0x31: 'vmr32', 0x34: 'vlqi',
              0x35: 'vsqi', 0x36: 'vlqd', 0x37: 'vsqd', 0x38: 'vdiv', 0x39: 'vsqrt', 0x3A: 'vrsqrt',
              0x3B: 'vwaitq', 0x3C: 'vmtir', 0x3D: 'vmfir', 0x3E: 'vilwr', 0x3F: 'viswr', 0x40: 'vrnext',
              0x41: 'vrget', 0x42: 'vrinit', 0x43: 'vrxor'}
for _base, _name in ((0x00, 'vadda'), (0x04, 'vsuba'), (0x08, 'vmadda'), (0x0C, 'vmsuba'), (0x18, 'vmula')):
    for _i in range(4):
        VU_SPECIAL[_base + _i] = _name + BC[_i]


def vu0_macro(word, rs, rt, rd, sa, fn):
    """COP2 macro-mode instruction: fd=sa, fs=rd, ft=rt; dest mask in the low four bits of rs."""
    dest = ''.join(c for c, bit in zip(BC, (8, 4, 2, 1)) if (rs & 0xF) & bit)
    if fn < 0x3C:
        name = VU_NORMAL.get(fn)
        if name is None:
            return f'.word 0x{word:08x}'
        if name.startswith('vi'):
            return f'{name} vi{sa}, vi{rd}, vi{rt}'
        return f'{name}.{dest} vf{sa}, vf{rd}, vf{rt}'
    name = VU_SPECIAL.get((sa << 2) | (fn & 3))
    if name is None:
        return f'.word 0x{word:08x}'
    fsf, ftf = BC[(word >> 21) & 3], BC[(word >> 23) & 3]
    if name == 'vdiv':
        return f'vdiv Q, vf{rd}{fsf}, vf{rt}{ftf}'
    if name in ('vsqrt',):
        return f'vsqrt Q, vf{rt}{ftf}'
    if name == 'vrsqrt':
        return f'vrsqrt Q, vf{rd}{fsf}, vf{rt}{ftf}'
    if name in ('vwaitq', 'vnop'):
        return name
    if name in ('vmove', 'vmr32', 'vabs', 'vitof0', 'vitof4', 'vitof12', 'vitof15',
                'vftoi0', 'vftoi4', 'vftoi12', 'vftoi15'):
        return f'{name}.{dest} vf{rt}, vf{rd}'
    return f'{name}.{dest} ACC?, vf{rd}, vf{rt}  (ft/fs as encoded)'


def s16(value):
    return value - 0x10000 if value & 0x8000 else value


# EE MMI (opcode 0x1C; EE Core Instruction Set Manual): funct names, and the sa-field tables of
# MMI0 (funct 0x08), MMI1 (0x28), MMI2 (0x09) and MMI3 (0x29).
MMI = {0x00: 'madd', 0x01: 'maddu', 0x04: 'plzcw', 0x10: 'mfhi1', 0x11: 'mthi1', 0x12: 'mflo1',
       0x13: 'mtlo1', 0x18: 'mult1', 0x19: 'multu1', 0x1A: 'div1', 0x1B: 'divu1', 0x20: 'madd1',
       0x21: 'maddu1', 0x30: 'pmfhl', 0x31: 'pmthl', 0x34: 'psllh', 0x36: 'psrlh', 0x37: 'psrah',
       0x3C: 'psllw', 0x3E: 'psrlw', 0x3F: 'psraw'}
MMI_SA = {
    0x08: {0: 'paddw', 1: 'psubw', 2: 'pcgtw', 3: 'pmaxw', 4: 'paddh', 5: 'psubh', 6: 'pcgth',
           7: 'pmaxh', 8: 'paddb', 9: 'psubb', 10: 'pcgtb', 16: 'paddsw', 17: 'psubsw', 18: 'pextlw',
           19: 'ppacw', 20: 'paddsh', 21: 'psubsh', 22: 'pextlh', 23: 'ppach', 24: 'paddsb',
           25: 'psubsb', 26: 'pextlb', 27: 'ppacb', 30: 'pext5', 31: 'ppac5'},
    0x28: {1: 'pabsw', 2: 'pceqw', 3: 'pminw', 4: 'padsbh', 5: 'pabsh', 6: 'pceqh', 7: 'pminh',
           10: 'pceqb', 16: 'padduw', 17: 'psubuw', 18: 'pextuw', 20: 'padduh', 21: 'psubuh',
           22: 'pextuh', 24: 'paddub', 25: 'psubub', 26: 'pextub', 27: 'qfsrv'},
    0x09: {0: 'pmaddw', 2: 'psllvw', 3: 'psrlvw', 4: 'pmsubw', 8: 'pmfhi', 9: 'pmflo', 10: 'pinth',
           12: 'pmultw', 13: 'pdivw', 14: 'pcpyld', 16: 'pmaddh', 17: 'phmadh', 18: 'pand',
           19: 'pxor', 20: 'pmsubh', 21: 'phmsbh', 26: 'pexeh', 27: 'prevh', 28: 'pmulth',
           29: 'pdivbw', 30: 'pexew', 31: 'prot3w'},
    0x29: {0: 'pmadduw', 3: 'psravw', 8: 'pmthi', 9: 'pmtlo', 10: 'pinteh', 12: 'pmultuw',
           13: 'pdivuw', 14: 'pcpyud', 18: 'por', 19: 'pnor', 26: 'pexch', 27: 'pcpyh', 30: 'pexcw'},
}


def decode_mmi(word):
    rs, rt, rd = (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
    sa, fn = (word >> 6) & 31, word & 63
    if fn in MMI_SA and sa in MMI_SA[fn]:
        name = MMI_SA[fn][sa]
        if name in ('pmfhi', 'pmflo'):
            return f'{name} {R[rd]}'
        if name in ('pmthi', 'pmtlo'):
            return f'{name} {R[rs]}'
        if name in ('pabsw', 'pabsh', 'pext5', 'ppac5', 'prevh', 'pexeh', 'pexew', 'prot3w', 'pexch',
                    'pcpyh', 'pexcw'):
            return f'{name} {R[rd]}, {R[rt]}'
        return f'{name} {R[rd]}, {R[rs]}, {R[rt]}'
    if fn in MMI:
        name = MMI[fn]
        if name in ('psllh', 'psrlh', 'psrah', 'psllw', 'psrlw', 'psraw'):
            return f'{name} {R[rd]}, {R[rt]}, {sa}'
        if name == 'pmfhl':
            return f'pmfhl.{["lw", "uw", "slw", "lh", "sh"][sa] if sa < 5 else sa} {R[rd]}'
        if name in ('mfhi1', 'mflo1'):
            return f'{name} {R[rd]}'
        if name in ('mthi1', 'mtlo1'):
            return f'{name} {R[rs]}'
        if name == 'plzcw':
            return f'plzcw {R[rd]}, {R[rs]}'
        return f'{name} {R[rd]}, {R[rs]}, {R[rt]}'
    return None


def decode(word, pc):
    op, rs, rt, rd = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
    sa, fn, imm = (word >> 6) & 31, word & 63, word & 0xFFFF
    target = pc + 4 + (s16(imm) << 2)
    if word == 0:
        return 'nop'
    if op == 0 and fn in SPECIAL:
        name = SPECIAL[fn]
        if name in ('sll', 'srl', 'sra', 'dsll', 'dsrl', 'dsra', 'dsll32', 'dsrl32', 'dsra32'):
            return f'{name} {R[rd]}, {R[rt]}, {sa}'
        if name == 'jr':
            return f'jr {R[rs]}'
        if name == 'jalr':
            return f'jalr {R[rs]}'
        if name in ('mfhi', 'mflo'):
            return f'{name} {R[rd]}'
        if name in ('mult', 'multu', 'div', 'divu'):
            return f'{name} {R[rs]}, {R[rt]}' + (f' -> {R[rd]}' if rd else '')
        return f'{name} {R[rd]}, {R[rs]}, {R[rt]}'
    if op == 0x1C:
        mmi = decode_mmi(word)
        if mmi:
            return mmi
    if op in (0x1E, 0x1F):
        return f"{'lq' if op == 0x1E else 'sq'} {R[rt]}, {s16(imm):#x}({R[rs]})"
    if op == 1 and rt in REGIMM:
        return f'{REGIMM[rt]} {R[rs]}, 0x{target:x}'
    if op in (2, 3):
        return f"{'j' if op == 2 else 'jal'} 0x{((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2):x}"
    if op in BRANCH:
        return f'{BRANCH[op]} {R[rs]}, {R[rt]}, 0x{target:x}'
    if op in BRANCHZ:
        return f'{BRANCHZ[op]} {R[rs]}, 0x{target:x}'
    if op in IMM:
        shown = imm if IMM[op] in ('andi', 'ori', 'xori') else s16(imm)
        return f'{IMM[op]} {R[rt]}, {R[rs]}, {shown:#x}'
    if op == 0x0F:
        return f'lui {R[rt]}, 0x{imm:x}'
    if op in MEM:
        return f'{MEM[op]} {R[rt]}, {s16(imm):#x}({R[rs]})'
    if op == 0x31:
        return f'lwc1 f{rt}, {s16(imm):#x}({R[rs]})'
    if op == 0x39:
        return f'swc1 f{rt}, {s16(imm):#x}({R[rs]})'
    if op == 0x36:
        return f'lqc2 vf{rt}, {s16(imm):#x}({R[rs]})'
    if op == 0x3E:
        return f'sqc2 vf{rt}, {s16(imm):#x}({R[rs]})'
    if op == 0x11:
        if rs == 0x00:
            return f'mfc1 {R[rt]}, f{rd}'
        if rs == 0x04:
            return f'mtc1 {R[rt]}, f{rd}'
        if rs == 0x08:
            return f"bc1{'t' if rt & 1 else 'f'}{'l' if rt & 2 else ''} 0x{target:x}"
        if rs == 0x10 and fn in FPU_S:
            name = FPU_S[fn]
            ft, fs, fd = rt, rd, sa
            if name.startswith('c.'):
                return f'{name} f{fs}, f{ft}'
            if name in ('adda.s', 'suba.s', 'mula.s'):
                return f'{name} f{fs}, f{ft}'
            if name in ('sqrt.s',):
                return f'{name} f{fd}, f{ft}'
            if name in ('abs.s', 'mov.s', 'neg.s', 'cvt.w.s'):
                return f'{name} f{fd}, f{fs}'
            return f'{name} f{fd}, f{fs}, f{ft}'
        if rs == 0x14 and fn == 0x20:
            return f'cvt.s.w f{sa}, f{rd}'
    if op == 0x12:
        if rs == 0x01:
            return f'qmfc2 {R[rt]}, vf{rd}'
        if rs == 0x05:
            return f'qmtc2 {R[rt]}, vf{rd}'
        if rs == 0x02:
            return f'cfc2 {R[rt]}, vi{rd}'
        if rs == 0x06:
            return f'ctc2 {R[rt]}, vi{rd}'
        if rs & 0x10:
            return vu0_macro(word, rs, rt, rd, sa, fn)
    return f'.word 0x{word:08x}'


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    elf_path = 'build/rea/SCUS_971.99'
    for flag in ('--elf', '--level'):
        if flag in sys.argv:
            value = sys.argv[sys.argv.index(flag) + 1]
            args.remove(value)
            if flag == '--elf':
                elf_path = value
    address = int(args[0], 16)
    count = int(args[1]) if len(args) > 1 else 24
    if '--level' in sys.argv:
        import json
        level = int(sys.argv[sys.argv.index('--level') + 1])
        folder = f'config/us/overlays/level_{level:02d}'
        with open(f'config/us/levels/level-{level:02d}.json', encoding='utf-8') as handle:
            text = next(s for s in json.load(handle)['sections'] if s['name'] == '.text')
        code = open(f'{folder}/text.bin', 'rb').read()
        if not text['addr'] <= address < text['addr'] + len(code):
            sys.exit(f'0x{address:x} is not in level {level} text')
        for step in range(count):
            pc = address + step * 4
            word = struct.unpack_from('<I', code, pc - text['addr'])[0]
            print(f'{pc:08x}: {word:08x}  {decode(word, pc)}')
        return
    elf = open(elf_path, 'rb').read()
    phoff, phnum = struct.unpack_from('<I', elf, 0x1C)[0], struct.unpack_from('<H', elf, 0x2C)[0]
    for index in range(phnum):
        kind, offset, vaddr, _, filesz = struct.unpack_from('<5I', elf, phoff + index * 0x20)
        if kind == 1 and vaddr <= address < vaddr + filesz:
            for step in range(count):
                pc = address + step * 4
                word = struct.unpack_from('<I', elf, offset + pc - vaddr)[0]
                print(f'{pc:08x}: {word:08x}  {decode(word, pc)}')
            return
    sys.exit(f'0x{address:x} is not in a loadable segment of {elf_path}')


if __name__ == '__main__':
    main()
