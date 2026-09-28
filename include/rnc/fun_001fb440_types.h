#ifndef RNC_FUN_001FB440_TYPES_H
#define RNC_FUN_001FB440_TYPES_H

#include "types.h"

struct M2c_D_00151780 {
    u8 pad_0[0x160];
    s16 unk160;
    s16 unk162;
    s16 unk164;
    s16 unk166;
    u8 pad_168[0x4];
    u16 unk16C;
    s16 unk16E;
    u8 pad_170[0x2];
};

struct M2c_D_00160F00 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct M2c_temp_17_70 {
    u8 pad_0[0x10];
    s64 unk10;
};

struct M2c_temp_18_72 {
    u8 pad_0[0x10];
    s64 unk10;
};

struct M2c_temp_2_107 {
    u8 pad_0[0x80];
    s64 unk80;
};

struct M2c_temp_2_63 {
    u8 pad_0[0x10];
    s64 unk10;
};

struct M2c_temp_3_64 {
    u8 pad_0[0x8];
    s32 unk8;
};

struct M2c_temp_4_108 {
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
    s32 unk38;
    u8 pad_3C[0x4];
    s64 unk40;
    s64 unk48;
};

#endif /* RNC_FUN_001FB440_TYPES_H */
