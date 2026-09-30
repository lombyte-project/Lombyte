#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/draw_boot_image/FUN_002012b8.s", FUN_002012b8);
#else
#include "types.h"

extern s32 D_0015ED80[];
extern s32 D_0015EE84[];
extern u8 *D_00160F00;

void FUN_002012b8(s32 arg0) {
    s32 n;
    s32 y;
    s32 x;
    s32 rem;
    s32 m;
    s32 t5a;
    s32 t5b;
    u8 *base;
    s64 v1;
    s64 v2;
    s64 v3;
    s64 v4;

    n = (D_0015ED80[0] != 0) ? 0x1C0 : 0x1A0;
    x = arg0;
    y = D_0015EE84[0] >> 8;

    do {
        rem = n - 0x80;
        m = 0x80;
        m = (rem >= 0) ? m : n;
        v1 = (s64)y << 16;
        v2 = (s64)m << 7;
        v3 = (s64)m << 16;
        v4 = ((s64)m << 7) | 0x0000080000008000L;
        y += (s32)(((s64)m << 19) >> 16);
        n = rem;
        t5a = (s32)((v2 | 0x30000000L) << 16);
        t5b = (s32)(v2 << 16);
        x += (s32)(((s64)m << 27) >> 16);
        v1 = v1 | 0x0000000800000000L;
        v3 = v3 | 0x200L;
        t5b = t5b | 0x50000000;

        *(u32 *)(D_00160F00 + 0x00) = 0x10000006;
        *(u32 *)(D_00160F00 + 0x04) = 0;
        *(u32 *)(D_00160F00 + 0x08) = 0;
        *(u32 *)(D_00160F00 + 0x0C) = 0x50000006;
        base = D_00160F00;
        D_00160F00 = base + 0x10;
        *(s64 *)(base + 0x10) = 0x0000400000000001L;
        *(s64 *)(base + 0x20) = v1;
        *(s64 *)(base + 0x40) = v3;
        *(s64 *)(base + 0x60) = v4;
        *(s64 *)(base + 0x68) = 0;
        *(s64 *)(base + 0x18) = 0x0EEEEEEE;
        *(s64 *)(base + 0x28) = 0x50;
        *(s64 *)(base + 0x30) = 0;
        *(s64 *)(base + 0x38) = 0x51;
        *(s64 *)(base + 0x48) = 0x52;
        *(s64 *)(base + 0x50) = 0;
        *(s64 *)(base + 0x58) = 0x53;
        *(u32 *)(base + 0x70) = t5a;
        D_00160F00 = base + 0x70;
        *(u32 *)(D_00160F00 + 0x00) = t5a;
        *(u32 *)(D_00160F00 + 0x04) = x;
        *(u32 *)(D_00160F00 + 0x08) = 0;
        *(u32 *)(D_00160F00 + 0x0C) = t5b;
        D_00160F00 += 0x10;
    } while (n > 0);

    *(u32 *)(D_00160F00 + 0x10) = 0x10000002;
    *(u32 *)(D_00160F00 + 0x04) = 0;
    *(u32 *)(D_00160F00 + 0x08) = 0;
    *(u32 *)(D_00160F00 + 0x0C) = 0x50000002;
    base = D_00160F00;
    D_00160F00 = base + 0x10;
    *(s64 *)(base + 0x10) = 0x0000100000008001L;
    *(s64 *)(base + 0x28) = 0x3F;
    *(s64 *)(base + 0x18) = 0xE;
    *(s64 *)(base + 0x20) = 0;
    D_00160F00 = base + 0x30;
}
#endif /* NON_MATCHING */
