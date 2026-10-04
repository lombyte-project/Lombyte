#include "types.h"

struct View {
    u8 pad0[0x150];
    s16 height;
    s16 width;
};

struct Metrics {
    u8 pad0[4];
    s32 color;
    u64 enable;
    s32 unk10;
    s32 unk14;
    u64 unk18;
    s32 unk20;
    s32 unk24;
    u64 unk28;
};

extern struct View D_00151780;
extern struct Metrics *D_0015F350 __attribute__((sda));
extern void func_00233980(s32, u64);
extern void func_001F52A0(s32, s32, s32, s32, u64);


void draw_screen_effect(void) __asm__("FUN_001f4fb8");

void draw_screen_effect(void) {
    s32 i;
    s16 w;
    u64 mask;
    s32 c;
    u64 e;
    s32 v14;
    u64 e18;
    s32 v24;
    u64 e28;

    i = 0;
    w = D_00151780.width;
    e = D_0015F350->enable;
    if (e != 0) {
        func_00233980(0x42, e & 0xFF000000FFULL);
    }
    c = D_0015F350->color;
    if (c & 0xFF000000) {
        func_001F52A0(0, w, 0, D_00151780.height, (u64) ((s64) c << 0x20) >> 0x20);
    }
    if (w > 0) {
        mask = 0xFF000000FFULL;
        do {
            e18 = D_0015F350->unk18;
            if (e18 != 0) {
                func_00233980(0x42, e18 & mask);
            }
            v14 = D_0015F350->unk14;
            if (v14 & 0xFF000000) {
                func_001F52A0(i, (i + D_0015F350->unk10 < w - 1) ? i + D_0015F350->unk10 : w - 1, 0, D_00151780.height, (u64) ((s64) v14 << 0x20) >> 0x20);
            }
            i = i + D_0015F350->unk10;
            e28 = D_0015F350->unk28;
            if (e28 != 0) {
                func_00233980(0x42, e28 & mask);
            }
            v24 = D_0015F350->unk24;
            if (v24 & 0xFF000000) {
                func_001F52A0(i, (i + D_0015F350->unk20 < w - 1) ? i + D_0015F350->unk20 : w - 1, 0, D_00151780.height, (u64) ((s64) v24 << 0x20) >> 0x20);
            }
            i = i + D_0015F350->unk20;
        } while (i < w);
    }
}

extern __typeof__(draw_screen_effect) func_001F4FB8 __attribute__((alias("FUN_001f4fb8")));
/* Recovered original symbol name. */
extern __typeof__(draw_screen_effect) DrawScreenEffect__Fv __attribute__((alias("FUN_001f4fb8")));
