#include "types.h"
extern u32 D_001312EC[];
extern volatile u32 D_00131314[];
extern u8 D_001324C0[];
extern u8 D_00132D08[];
extern s32 SignalSema(s32 semaId);
extern s32 cd_check_scmd(s32 arg0) __asm__("func_00120D40");
extern s32 sceSifCallRpc(void *pkt, u32 cmd, s32 mode, void *send, s32 sendsize, void *recv,
                         s32 recvsize, void *endfunc, void *endparam);
s32 sceCdBreak(void) {
    s32 temp_16_44;

    if (cd_check_scmd(0x1E) == 0) {
        return 0;
    }
    D_00131314[0] = 8;
    if (sceSifCallRpc(D_00132D08, 0x16, 0, 0, 0, D_001324C0, 4, 0, 0) < 0) {
        s32 sema = *(volatile u32 *)D_001312EC;
        SignalSema(sema);
        D_00131314[0] = 0;
        return 0;
    }
    D_00131314[0] = 0;
    temp_16_44 = *(u32 *)((u32)D_001324C0 | 0x20000000);
    {
        s32 sema2 = *(volatile u32 *)D_001312EC;
        SignalSema(sema2);
    }
    return temp_16_44;
}
