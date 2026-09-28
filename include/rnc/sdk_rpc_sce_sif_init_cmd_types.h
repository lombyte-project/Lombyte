#ifndef RNC_SDK_RPC_SCE_SIF_INIT_CMD_TYPES_H
#define RNC_SDK_RPC_SCE_SIF_INIT_CMD_TYPES_H

#include "types.h"

struct M2c_D_00154E40 {
    u8 pad_0[0xC];
    s32 unkC;
    s32 unk10;
};

struct M2c_D_00154E58 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
};

struct M2c_D_00154E80 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct M2c_var_3_42 {
    s32 unk0;
    s32 unk4;
};

#endif /* RNC_SDK_RPC_SCE_SIF_INIT_CMD_TYPES_H */
