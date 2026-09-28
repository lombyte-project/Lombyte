#ifndef RNC_SDK_RPC_SCE_SIF_INIT_RPC_TYPES_H
#define RNC_SDK_RPC_SCE_SIF_INIT_RPC_TYPES_H

#include "types.h"

struct M2c_D_00156800 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
};

struct M2c_temp_5_80 {
    u8 pad_0[0xC];
    s32 unkC;
};

#endif /* RNC_SDK_RPC_SCE_SIF_INIT_RPC_TYPES_H */
