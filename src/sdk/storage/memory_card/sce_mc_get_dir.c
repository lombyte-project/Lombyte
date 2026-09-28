#include "types.h"
struct M2c_D_00159A00 {
    u8 pad_0[0x24];
    s32 unk24;
};

struct M2c_D_00159AB0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad_14[0x3FF];
    s32 unk413;
};

extern u32 D_00132DA8[];
extern u32 D_00132DAC[];
extern struct M2c_D_00159A00 D_00159A00;
extern struct M2c_D_00159AB0 D_00159AB0;
extern u8 D_0015AFC0[];
extern s32 PollSema();
extern s32 SignalSema();
extern s32 sceSifCallRpc();
extern s32 sceSifWriteBackDCache();
extern s32 strncpy(s8 *, s8 *, s32);
s32 sceMcGetDir(s32 arg0, s32 arg1, s8 *arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 temp_2_67;
    s32 var_2_30;
    register s32 path_arg = arg1;

    if (D_00159A00.unk24 == 0) {
        return -0x64;
    }
    var_2_30 = -0xC8;
    if (PollSema(D_00132DAC[0]) >= 0) {
        if ((arg2 == NULL) || (*arg2 == 0)) {
            SignalSema(D_00132DAC[0]);
            return -0xD2;
        }
        D_00159AB0.unk0 = arg0;
        D_00159AB0.unk4 = path_arg;
        D_00159AB0.unk8 = arg3;
        D_00159AB0.unkC = arg4;
        D_00159AB0.unk10 = arg5;
        strncpy((s8 *)((u8 *)&D_00159AB0 + 0x14), arg2, 0x3FF);
        *(u8 *)((u8 *)&D_00159AB0 + 0x413) = 0;
        if (arg4 >= 0) {
            sceSifWriteBackDCache(arg5, arg4 << 6);
        }
        temp_2_67 = sceSifCallRpc(&D_00159A00, 0xD, 1, &D_00159AB0, 0x414, D_0015AFC0, 4, 0, 0);
        if (temp_2_67 == 0) {
            D_00132DA8[0] = 0xD;
        } else {
            SignalSema(D_00132DAC[0]);
        }
        var_2_30 = temp_2_67;
        /* Duplicate return node #12. Try simplifying control flow for better match */
        return var_2_30;
    }
    return var_2_30;
}
