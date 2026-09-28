#ifndef RNC_FUN_00200468_TYPES_H
#define RNC_FUN_00200468_TYPES_H

#include "types.h"

struct M2c_D_0013E500 {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
};

struct M2c_D_00160F00 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct M2c_D_0019A3E8 {
    u8 pad_0[0xC];
    s32 unkC;
};

struct M2c_temp_12_50 {
    u8 pad_0[0x10];
    s64 unk10;
};

struct M2c_temp_7_54 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    u8 pad_14[0x4];
    s32 unk18;
    u8 pad_1C[0x4];
    s64 unk20;
    s64 unk28;
    s64 unk30;
    s64 unk38;
    s64 unk40;
    s32 unk48;
};

#endif /* RNC_FUN_00200468_TYPES_H */
