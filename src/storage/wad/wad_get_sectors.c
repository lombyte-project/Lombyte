#include "types.h"
extern u8 D_0015ED58;
extern s32 sceCdGetError();
extern s32 sceCdRead();
extern s32 sceCdSync();
extern s32 sceGsSyncV();
s32 wad_get_sectors(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_0012f208");

s32 wad_get_sectors(s32 arg0, s32 arg1, s32 arg2) {
    u8 sp_slot[0x8];
    s32 shift = arg1 << 0xB;
    sp_slot[0] = 0x20;
    sp_slot[1] = D_0015ED58;
    sp_slot[2] = 0;
    sp_slot[3] = 0;
loop_1:
    sceCdRead(arg0, arg1, arg2, sp_slot);
    goto loop_3;
block_2:
    sceGsSyncV(0);
loop_3:
    if (sceCdSync(1) != 0) {
        goto block_2;
    }
    if (sceCdGetError() != 0) {
        goto loop_1;
    }
    return shift;
}
