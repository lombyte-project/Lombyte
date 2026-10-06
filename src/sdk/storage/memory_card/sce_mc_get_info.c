#include "types.h"
struct sceSifClientData {
    u8 pad_0[0x24];
    s32 unk24;
};

struct McGetInfoRequest {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
};

extern u8 D_00123D10[];
extern u32 D_00132DA8[];
extern u32 D_00132DAC[];
extern struct sceSifClientData D_00159A00;
extern u32 D_00159A28[];
extern u32 D_00159A2C[];
extern u32 D_00159A30[];
extern struct McGetInfoRequest D_00159A80;
extern u8 D_00159F00[];
extern u8 D_0015AFC0[];
extern s32 PollSema();
extern s32 SignalSema();
extern s32 sceSifCallRpc();
extern s32 sceSifWriteBackDCache();
s32 sceMcGetInfo(s32 port, s32 slot, s32 type, s32 free_blocks, s32 format) {
    s32 result;

    if (D_00159A00.unk24 == 0) {
        return -0x64;
    }
    if (PollSema(D_00132DAC[0]) < 0) {
        return -0xC8;
    }
    D_00159A80.unk4 = port;
    D_00159A80.unk8 = slot;
    D_00159A80.unk1C = D_00159F00;
    if (type != 0) {
        D_00159A80.unk14 = 1;
    } else {
        D_00159A80.unk14 = 0;
    }
    if (free_blocks != 0) {
        D_00159A80.unk10 = 1;
    } else {
        D_00159A80.unk10 = 0;
    }
    if (format != 0) {
        D_00159A80.unkC = 1;
    } else {
        D_00159A80.unkC = 0;
    }
    D_00159A28[0] = type;
    D_00159A2C[0] = free_blocks;
    D_00159A30[0] = (u32)format;
    sceSifWriteBackDCache(D_00159F00, 0xC0);
    result =
        sceSifCallRpc(&D_00159A00, 1, 1, &D_00159A80, 0x30, D_0015AFC0, 4, D_00123D10, D_00159F00);
    if (result == 0) {
        D_00132DA8[0] = 1;
    } else {
        SignalSema(D_00132DAC[0]);
    }
    return result;
}
