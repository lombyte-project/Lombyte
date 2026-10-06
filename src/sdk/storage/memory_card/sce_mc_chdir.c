#include "types.h"
struct sceSifClientData {
    u8 pad_0[0x24];
    s32 unk24;
};

struct McChdirRequest {
    s32 unk0;
    s32 unk4;
    u8 pad_8[0x8];
    s32 unk10;
    u8 pad_14[0x3FF];
    s32 unk413;
};

extern u32 D_00132DA8[];
extern u32 D_00132DAC[];
extern struct sceSifClientData D_00159A00;
extern struct McChdirRequest D_00159AB0;
extern u8 D_00159FC0[];
extern u8 D_0015AFC0[];
extern s32 PollSema();
extern s32 SignalSema();
extern s32 sceSifCallRpc();
extern s32 sceSifWriteBackDCache();
extern s32 strncpy(s8 *, s8 *, s32);
extern void mceStorePwd();
s32 sceMcChdir(s32 port, s32 slot, s8 *path, s32 pwd) {
    s32 result;
    s32 var_2_28;

    if (D_00159A00.unk24 == 0) {
        return -0x64;
    }
    var_2_28 = -0xC8;
    if (PollSema(D_00132DAC[0]) >= 0) {
        if ((path == NULL) || (*path == 0)) {
            SignalSema(D_00132DAC[0]);
            return -0xD2;
        }
        D_00159AB0.unk0 = port;
        D_00159AB0.unk10 = D_00159FC0;
        D_00159AB0.unk4 = slot;
        strncpy((s8 *)((u8 *)&D_00159AB0 + 0x14), path, 0x3FF);
        *(u8 *)((u8 *)&D_00159AB0 + 0x413) = 0;
        sceSifWriteBackDCache(D_00159FC0, 0x400);
        result = sceSifCallRpc(&D_00159A00, 0xC, 1, &D_00159AB0, 0x414, D_0015AFC0, 4,
                                  &mceStorePwd, pwd);
        if (result == 0) {
            D_00132DA8[0] = 0xC;
        } else {
            SignalSema(D_00132DAC[0]);
        }
        var_2_28 = result;
        /* Duplicate return node #10. Try simplifying control flow for better match */
        return var_2_28;
    }
    return var_2_28;
}
