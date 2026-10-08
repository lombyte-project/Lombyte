#include "types.h"
#include "sifrpc.h"
struct McGetDirRequest {
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
extern struct sceSifClientData D_00159A00;
extern struct McGetDirRequest D_00159AB0;
extern u8 D_0015AFC0[];
extern s32 PollSema();
extern s32 SignalSema();
extern s32 sceSifCallRpc();
extern s32 sceSifWriteBackDCache();
extern s32 strncpy(s8 *, s8 *, s32);
s32 sceMcGetDir(s32 port, s32 slot, s8 *name, s32 mode, s32 maxent, s32 table) {
    s32 result;
    s32 var_2_30;
    register s32 path_arg = slot;

    if (D_00159A00.serve == 0) {
        return -0x64;
    }
    var_2_30 = -0xC8;
    if (PollSema(D_00132DAC[0]) >= 0) {
        if ((name == NULL) || (*name == 0)) {
            SignalSema(D_00132DAC[0]);
            return -0xD2;
        }
        D_00159AB0.unk0 = port;
        D_00159AB0.unk4 = path_arg;
        D_00159AB0.unk8 = mode;
        D_00159AB0.unkC = maxent;
        D_00159AB0.unk10 = table;
        strncpy((s8 *)((u8 *)&D_00159AB0 + 0x14), name, 0x3FF);
        *(u8 *)((u8 *)&D_00159AB0 + 0x413) = 0;
        if (maxent >= 0) {
            sceSifWriteBackDCache(table, maxent << 6);
        }
        result = sceSifCallRpc(&D_00159A00, 0xD, 1, &D_00159AB0, 0x414, D_0015AFC0, 4, 0, 0);
        if (result == 0) {
            D_00132DA8[0] = 0xD;
        } else {
            SignalSema(D_00132DAC[0]);
        }
        var_2_30 = result;
        /* Duplicate return node #12. Try simplifying control flow for better match */
        return var_2_30;
    }
    return var_2_30;
}
