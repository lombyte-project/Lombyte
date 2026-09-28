#ifndef RNC_FUN_001FB740_TYPES_H
#define RNC_FUN_001FB740_TYPES_H

#include "types.h"

struct M2c_D_00160F00 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct M2c_temp_15_61 {
    u8 pad_0[0x10];
    s64 unk10;
};

struct M2c_temp_2_62 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    u8 pad_14[0x4];
    s32 unk18;
    u8 pad_1C[0x4];
    s64 unk20;
    s32 unk28;
    u8 pad_2C[0x4];
    s32 unk30;
    u8 pad_34[0x4];
    s64 unk38;
    s64 unk40;
    s32 unk48;
};

struct M2c_var_5_87 {
    s64 unk0;
    s32 unk8;
};

#endif /* RNC_FUN_001FB740_TYPES_H */
