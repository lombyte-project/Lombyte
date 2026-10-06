#include "types.h"
struct M2c_D_00159A00 {
    u8 pad_0[0x24];
    s32 unk24;
};

struct M2c_D_00159AB0 {
    s32 unk0;
    s32 unk4;
    u8 pad_8[0x8];
    s32 unk10;
    u8 pad_14[0x3FF];
    s32 unk413;
};

extern u32 D_00132DA8[];
extern u32 D_00132DAC[];
extern struct M2c_D_00159A00 D_00159A00;
extern struct M2c_D_00159AB0 D_00159AB0;
extern u8 D_00159FC0[];
extern u8 D_0015AFC0[];
extern s32 PollSema();
extern s32 SignalSema();
extern s32 sceSifCallRpc();
extern s32 sceSifWriteBackDCache();
extern s32 strncpy(s8 *, s8 *, s32);
extern void mceStorePwd();
s32 sceMcChdir(s32 arg0, s32 arg1, s8 *arg2, s32 arg3) {
    s32 temp_2_64;
    s32 var_2_28;

    if (D_00159A00.unk24 == 0) {
        return -0x64;
    }
    var_2_28 = -0xC8;
    if (PollSema(D_00132DAC[0]) >= 0) {
        if ((arg2 == NULL) || (*arg2 == 0)) {
            SignalSema(D_00132DAC[0]);
            return -0xD2;
        }
        D_00159AB0.unk0 = arg0;
        D_00159AB0.unk10 = D_00159FC0;
        D_00159AB0.unk4 = arg1;
        strncpy((s8 *)((u8 *)&D_00159AB0 + 0x14), arg2, 0x3FF);
        *(u8 *)((u8 *)&D_00159AB0 + 0x413) = 0;
        sceSifWriteBackDCache(D_00159FC0, 0x400);
        temp_2_64 = sceSifCallRpc(&D_00159A00, 0xC, 1, &D_00159AB0, 0x414, D_0015AFC0, 4,
                                  &mceStorePwd, arg3);
        if (temp_2_64 == 0) {
            D_00132DA8[0] = 0xC;
        } else {
            SignalSema(D_00132DAC[0]);
        }
        var_2_28 = temp_2_64;
        /* Duplicate return node #10. Try simplifying control flow for better match */
        return var_2_28;
    }
    return var_2_28;
}
