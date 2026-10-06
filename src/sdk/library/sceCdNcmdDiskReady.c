#include "types.h"
extern u32 D_001312E8[];
extern u8 D_00131340[];
extern u8 D_00132490[];
extern s32 SignalSema();
extern s32 cd_check_ncmd() __asm__("func_00120A28");
extern s32 sceSifCallRpc();
s32 sceCdNcmdDiskReady(void) {
    s32 temp_16_37;

    if (cd_check_ncmd(2) == 0) {
        return 0;
    }
    if (sceSifCallRpc(D_00132490, 0xE, 0, 0, 0, D_00131340, 4, 0, 0) < 0) {
        SignalSema(D_001312E8[0]);
        return 0;
    }
    temp_16_37 = *(u32 *)((u32)D_00131340 | 0x20000000);
    SignalSema(D_001312E8[0]);
    return temp_16_37;
}
