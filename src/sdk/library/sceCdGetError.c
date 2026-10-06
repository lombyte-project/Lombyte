#include "types.h"
extern u32 D_001312EC[];
extern u8 D_001324C0[];
extern u8 D_00132D08[];
extern s32 SignalSema();
extern s32 cd_check_scmd() __asm__("func_00120D40");
extern s32 sceSifCallRpc();
s32 sceCdGetError(void) {
    s32 error;

    if (cd_check_scmd(3) == 0) {
        return -1;
    }
    if (sceSifCallRpc(D_00132D08, 4, 0, 0, 0, D_001324C0, 4, 0, 0) < 0) {
        SignalSema(D_001312EC[0]);
        return -1;
    }
    error = *(u32 *)((u32)D_001324C0 | 0x20000000);
    SignalSema(D_001312EC[0]);
    return error;
}
