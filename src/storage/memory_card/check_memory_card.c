#include "types.h"


struct McDirBlk {
    u8 pad_0[0x4];
    s32 unk4;
    u8 pad_8[0xB4];
    s32 unkBC;
};

#include "rnc/storage/memory_card/memory_card_state.h"
extern u8 D_0013D1D0[];
extern u8 D_0013D348[];
extern s32 sceMcGetInfo(s32, s32, s32, s32, s32);
extern s32 sceMcSync(s32, s32 *, s32 *);
extern s32 sceMcGetDir(s32, s32, s8 *, s32, s32, s32);
extern s32 sceGsSyncV(s32);

s32 check_memory_card(void) __asm__("FUN_00209168");

s32 check_memory_card(void) {
    s32 card_type;
    s32 free_blocks;
    s32 format;
    s32 cmd_code;
    s32 result;
    s32 *blk;
    struct McDirBlk *dir;

    blk = &memory_card_state.card[0].port;
    result = sceMcGetInfo(blk[0], blk[1], &card_type, &free_blocks, &format);

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
    if (card_type != 2) {
        return 1;
    }
    if (format == 0) {
        return 0;
    }
    result = sceMcGetDir(memory_card_state.card[0].port, dir->unk4, D_0013D1D0, 0, -1, 0);
    while (sceMcSync(1, &cmd_code, &result) == 0) {
        sceGsSyncV(0);
    }
    if (result > 0) {
        return 0;
    }
    if (free_blocks < 0x15E) {
        return 2;
    }
    return 0;
}

extern __typeof__(check_memory_card) func_00209168 __attribute__((alias("FUN_00209168")));
