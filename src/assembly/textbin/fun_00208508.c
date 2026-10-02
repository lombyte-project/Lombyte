#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00208508/FUN_00208508.s", FUN_00208508);
#else
/* Ported from rac1-decomp, the PAL decompilation notes (func_00208D38, menu.c). */
#include "types.h"
#include "sda.h"

struct Entry {
    f32 x;
    f32 y;
    s32 unk8;
    u32 unkC;
};

struct Global9A3E8 {
    u8 pad_0[0x20];
    s32 tab;
    s32 rows;
};

struct GlobalA00F0 {
    u8 pad_0[0x1C];
    struct Entry *entries;
    u8 pad_20[0x90];
    s32 count;
    u8 pad_B4[0x170];
    s32 idx;
};

extern struct Global9A3E8 D_0019A3E8 NOT_SDA;
extern struct GlobalA00F0 D_001A00F0;
extern s32 D_0015ED84 MACRO_ADDR;
extern s32 D_0015FDB0 MACRO_ADDR;
extern f32 D_001A01A4[];
extern void func_00208408(s32 *, s32 *, s32, f32, f32);
extern s32 func_001FF960(s32, u32);
extern void func_00200E08(s32, s32, s32, s32, long, long);
extern void func_00200080(s32, s32, s32, s32, s32, s32);

extern struct GlobalA00F0 D_001A00F0_far __asm__("D_001A00F0") __attribute__((section(".data")));
void FUN_00208508(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    struct Entry *p;
    s32 res0;
    s32 res1;
    s32 ix;
    s32 iy;
    s32 x;
    s32 y;
    s32 i;
    s32 r;
    s16 *tab;
    u8 *row;
    u8 h;
    u8 w;

    i = D_001A00F0.count;
    p = D_001A00F0.entries;
    i--;
    for (; i != -1; i--) {
        func_00208408(&res0, &res1, D_0015ED84, p->x, p->y);
        ix = (s32)(((f32)(arg2 - arg0)) * (*(f32 *)&res0));
        iy = (s32)(((f32)(arg3 - arg1)) * (*(f32 *)&res1));
        x = arg0 + ix;
        y = arg1 + iy;
        if (x < -0x199 || y < -0x199 || x >= 0x219a || y >= 0x1b9a) {
        } else if (p->unk8 == -1) {
            func_00200E08(x - D_0015FDB0, y - D_0015FDB0,
                          x + D_0015FDB0, y + D_0015FDB0, p->unkC, 1);
        } else {
            f32 scale;

            r = func_001FF960(p->unk8, p->unkC);
            tab = (s16 *)D_0019A3E8.tab;
            row = (u8 *)D_0019A3E8.rows + tab[r * 2 + 1] * 8;
            h = row[6];
            w = row[7];
            scale = (2.0f * D_001A01A4[D_001A00F0_far.idx] + 5.0f) / 13.0f;
            func_00200080(r, x - (s32)(scale * (f32)(1 << (h + 3))),
                             y - (s32)(scale * (f32)(1 << (w + 3))),
                             (s32)(scale * (f32)(1 << (h + 4))),
                             (s32)(scale * (f32)(1 << (w + 4))), 0x80);
        }
        p++;
    }
}
#endif /* NON_MATCHING */
