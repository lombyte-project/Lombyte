#include "types.h"
extern u32 D_001312EC[];
extern u8 D_001324C0[];
extern u8 D_00132900[];
extern u8 D_00132D08[];
extern s32 SignalSema();
extern s32 cd_check_scmd() __asm__("func_00120D40");
extern s32 sceSifCallRpc();
extern void sceSifWriteBackDCache();
s32 sceCdMmode(s32 arg0) {
    register u8 *state;
    register s32 mode;
    s32 result;

    state = D_00132900;
    mode = arg0;
    if (cd_check_scmd(0x22) == 0) {
        return 0;
    }
    *(s32 *)D_00132900 = mode;
    sceSifWriteBackDCache(state, 4);
    if (sceSifCallRpc(D_00132D08, 0x22, 0, state, 4, D_001324C0, 4, 0, 0) < 0) {
        SignalSema(D_001312EC[0]);
        return 0;
    }
    result = *(u32 *)((u32)D_001324C0 | 0x20000000);
    SignalSema(D_001312EC[0]);
    return result;
}
