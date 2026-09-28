#ifndef RNC_SDK_RPC_SCE_SIF_CALL_RPC_TYPES_H
#define RNC_SDK_RPC_SCE_SIF_CALL_RPC_TYPES_H

#include "types.h"

struct M2c_arg0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad_C[0x8];
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
};

struct M2c_temp_2_23 {
    u8 pad_0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
};

#endif /* RNC_SDK_RPC_SCE_SIF_CALL_RPC_TYPES_H */
