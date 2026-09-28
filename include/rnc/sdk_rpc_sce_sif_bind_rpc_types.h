#ifndef RNC_SDK_RPC_SCE_SIF_BIND_RPC_TYPES_H
#define RNC_SDK_RPC_SCE_SIF_BIND_RPC_TYPES_H

#include "types.h"

struct M2c_arg0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    u8 pad_14[0x10];
    s32 unk24;
};

struct M2c_temp_2_15 {
    u8 pad_0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
};

#endif /* RNC_SDK_RPC_SCE_SIF_BIND_RPC_TYPES_H */
