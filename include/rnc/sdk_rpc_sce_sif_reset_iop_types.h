#ifndef RNC_SDK_RPC_SCE_SIF_RESET_IOP_TYPES_H
#define RNC_SDK_RPC_SCE_SIF_RESET_IOP_TYPES_H

#include "types.h"

struct M2c_D_00158440 {
    s64 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    s32 unk14;
};

struct M2c_temp_3_25 {
    u8 pad_0[0x18];
    s32 unk18;
};

#endif /* RNC_SDK_RPC_SCE_SIF_RESET_IOP_TYPES_H */
