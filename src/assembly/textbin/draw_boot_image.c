#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/draw_boot_image/FUN_002012b8.s", FUN_002012b8);
#else
#include "types.h"

struct DmaTag {
    u32 w0;
    u32 addr;
    u32 w2;
    u32 w3;
};

struct TagPtr {
    struct DmaTag *p;
};

extern struct TagPtr D_00160F00;
extern s32 D_0015ED80;
extern s32 D_0015EE84;

void FUN_002012b8(u32 addr) {
    struct DmaTag *tag;
    struct DmaTag *next;
    u64 *q;
    s32 rows;
    s32 dbp;
    s64 h;
    s32 rem;
    s64 t7;

    rows = D_0015ED80 ? 0x1C0 : 0x1A0;
    dbp = D_0015EE84 >> 8;
    do {
        rem = rows - 0x80;
        h = 0x80;
        if (rem < 0) {
            h = rows;
        }
        D_00160F00.p->w0 = 0x10000006;
        D_00160F00.p->addr = 0;
        D_00160F00.p->w2 = 0;
        D_00160F00.p->w3 = 0x50000006;
        tag = D_00160F00.p;
        next = tag + 7;
        D_00160F00.p = tag + 1;
        q = (u64 *)(tag + 1);
        q[0] = 0x4000000000000001;
        q[1] = 0xEEEEEEE;
        q[2] = ((u64)dbp << 32) | 0x0008000000000000;
        q[3] = 0x50;
        q[4] = 0;
        q[5] = 0x51;
        q[6] = ((u64)h << 32) | 0x200;
        q[7] = 0x52;
        q[8] = 0;
        q[9] = 0x53;
        t7 = h << 7;
        q[10] = ((u64)t7) | 0x0800000000008000;
        q[11] = 0;
        D_00160F00.p = next;
        D_00160F00.p->w0 = t7 | 0x30000000;
        D_00160F00.p->addr = addr;
        { s64 t = h << 11; addr += t; }
        D_00160F00.p->w2 = 0;
        D_00160F00.p->w3 = (s32)t7 | 0x50000000;
        D_00160F00.p++;
        { s64 t = h << 3; dbp += t; }
        rows = rem;
    } while (rows > 0);
    D_00160F00.p->w0 = 0x10000002;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000002;
    tag = D_00160F00.p;
    next = tag + 3;
    q = (u64 *)(tag + 1);
    D_00160F00.p = tag + 1;
    q[0] = 0x1000000000008001;
    q[1] = 0xE;
    q[2] = 0;
    q[3] = 0x3F;
    D_00160F00.p = next;
}
#endif /* NON_MATCHING */
