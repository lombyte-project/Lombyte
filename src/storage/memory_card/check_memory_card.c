#include "types.h"

struct McBlock {
    s32 unk0;
    s32 unk4;
    u8 pad_8[0x10];
};

struct McDirBlk {
    u8 pad_0[0x4];
    s32 unk4;
    u8 pad_8[0xB4];
    s32 unkBC;
};

extern struct McBlock D_0013D290;
extern u8 D_0013D1D0[];
extern u8 D_0013D348[];
extern s32 sceMcGetInfo(s32, s32, s32, s32, s32);
extern s32 sceMcSync(s32, s32 *, s32 *);
extern s32 sceMcGetDir(s32, s32, s8 *, s32, s32, s32);
extern s32 sceGsSyncV(s32);

s32 check_memory_card(void) __asm__("FUN_00209168");

s32 check_memory_card(void) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 *blk;
    struct McDirBlk *dir;

    blk = &D_0013D290.unk0;
    sp10 = sceMcGetInfo(blk[0], blk[1], &sp0, &sp4, &sp8);

    while (sceMcSync(1, D_0013D348, D_0013D348 + 4) == 0) {
        sceGsSyncV(0);
    }
    dir = (struct McDirBlk *)((u8 *)D_0013D348 - 0xB8);
    if (dir->unkBC == -5) {
        return 1;
    }
    if (dir->unkBC < -9) {
        return 1;
    }
    if (dir->unkBC == -2) {
        return 0;
    }
    if (sp0 != 2) {
        return 1;
    }
    if (sp8 == 0) {
        return 0;
    }
    sp10 = sceMcGetDir(D_0013D290.unk0, dir->unk4, D_0013D1D0, 0, -1, 0);
    while (sceMcSync(1, &spC, &sp10) == 0) {
        sceGsSyncV(0);
    }
    if (sp10 > 0) {
        return 0;
    }
    if (sp4 < 0x15E) {
        return 2;
    }
    return 0;
}

extern __typeof__(check_memory_card) func_00209168 __attribute__((alias("FUN_00209168")));
