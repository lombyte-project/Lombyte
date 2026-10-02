#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00204428/FUN_00204428.s", FUN_00204428);
#else
#include "types.h"
#include "sda.h"

struct Tbl {
    u8 pad_0[0x2968];
    s32 unk2968;
    s32 unk296C;
    s32 unk2970;
    s32 unk2974;
    s32 unk2978;
    s32 unk297C;
};

struct Streams {
    u8 pad_0[0x12C8];
    s32 unk12C8;
    s32 unk12CC;
};

struct Hdr {
    u8 pad_0[8];
    s32 unk8;
};

extern struct Tbl D_00137B80;
extern s16 D_0013E056[];
extern s32 D_0015ED58 MACRO_ADDR;
extern s32 D_0015ED5C MACRO_ADDR;
extern s32 D_0015ED80 MACRO_ADDR;
extern u16 D_0015EE48 MACRO_ADDR;
extern s16 D_0015EE4A;
extern struct Hdr *D_0015EE4C;
extern u8 *D_0015EE50;
extern u8 *D_0015EE54;
extern s32 D_0015EEBC __attribute__((sda));
extern s32 D_0015EEC0 MACRO_ADDR;
extern u8 D_1FF8000[];
extern void FUN_0022dd78();
extern s32 func_0012DC80();
extern void func_0012E088(s32, s32, u64);
extern void func_0012E1A8();
extern s32 func_0012E1D8(s32);
extern s32 func_0012E3B8();
extern void func_00216728(s32, s32, s32);
extern s32 sceCdBreak();
extern s32 sceCdGetError();
extern s32 sceCdSync(s32);

s32 FUN_00204428(void) {
    s32 st;
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    u8 *p;
    u8 *m1;
    u8 *m2;
    struct Streams *q;
    s32 n;
    struct Hdr *r;

    n = D_0013E056[0] + 1;
    if (sceCdSync(1) != 0) {
        D_0015EEBC = D_0015EEBC + 1;
        if (D_0015ED58 == 1) {
            if (D_0015EEBC >= 0x2D1) {
                D_0015EEC0 = D_0015ED58;
                x = D_0015EE48 - 1;
                D_0015ED58 = 0;
                if ((u16)x < 3) {
                    D_0015EE48 = x;
                }
                sceCdBreak();
            }
        }
        return 0;
    }
    if (sceCdGetError() != 0) {
        if (D_0015EEC0 == 0) {
            D_0015EEC0 = 1;
            x = D_0015EE48 - 1;
            D_0015ED58 = 0;
            if ((u16)x < 3) {
                D_0015EE48 = x;
            }
        }
    }
    st = (s16)D_0015EE48;
    switch (st) {
    case 0:
        if (D_0015ED80 != 0) {
            y = ((D_00137B80.unk297C << 11) + 0xFFF) & 0xFFFFF000;
        } else {
            y = ((D_00137B80.unk2974 << 11) + 0xFFF) & 0xFFFFF000;
        }
        p = D_1FF8000 - y;
        q = (struct Streams *)((u8 *)&D_00137B80 + n * 8);
        z = q->unk12CC;
        m1 = p - (((z << 11) + 0xFFF) & 0xFFFFF000);
        w = D_00137B80.unk296C;
        m2 = m1 - (((w << 11) + 0xFFF) & 0xFFFFF000);
        y = D_00137B80.unk2968;
        D_0015EE54 = m1;
        D_0015EE50 = p;
        D_0015EE4C = (struct Hdr *)m2;
        func_00216728((s32)m2, y, w);
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 1:
        q = (struct Streams *)((u8 *)&D_00137B80 + n * 8);
        p = D_0015EE54;
        z = q->unk12CC;
        w = q->unk12C8;
        func_00216728((s32)p, w, z);
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 2:
        if (D_0015ED80 != 0) {
            func_00216728((s32)D_0015EE50, D_00137B80.unk2978, D_00137B80.unk297C);
        } else {
            func_00216728((s32)D_0015EE50, D_00137B80.unk2970, D_00137B80.unk2974);
        }
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 3:
        if (D_0015EE4A != 0) {
            return 0;
        }
        func_0012E3B8();
        if (D_0015ED5C != 0) {
            D_0015EE48 = D_0015EE48 + 1;
        } else {
            D_0015EE48 = 6;
        }
        break;
    case 4:
        if (func_0012DC80() != 0) {
            return 0;
        }
        func_0012E1D8(D_0015ED5C);
        D_0015ED5C = 0;
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 5:
        if (func_0012DC80() != 0) {
            return 0;
        }
        func_0012E1A8();
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 6:
        if (func_0012DC80() != 0) {
            return 0;
        }
        r = D_0015EE4C;
        D_0015ED5C = -1;
        func_0012E088(r->unk8 + (s32)r, (s32)FUN_0022dd78, (u32)&D_0015ED5C);
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 7:
        if (func_0012DC80() != 0) {
            return 0;
        }
        if (D_0015ED5C == -1) {
            return 0;
        }
        func_0012E1A8();
        return 1;
    }
    return 0;
}
#endif /* NON_MATCHING */
