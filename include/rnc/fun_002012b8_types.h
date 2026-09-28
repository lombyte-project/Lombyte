#ifndef RNC_FUN_002012B8_TYPES_H
#define RNC_FUN_002012B8_TYPES_H

#include "types.h"

struct M2c_D_00160F00 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct M2c_temp_2_143 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    u8 pad_14[0x4];
    s32 unk18;
};

struct M2c_temp_2_81 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    u8 pad_14[0x4];
    s32 unk18;
    u8 pad_1C[0x4];
    s32 unk20;
    u8 pad_24[0x4];
    s32 unk28;
    u8 pad_2C[0x4];
    s64 unk30;
    s32 unk38;
    u8 pad_3C[0x4];
    s32 unk40;
    u8 pad_44[0x4];
    s32 unk48;
    u8 pad_4C[0x4];
    s64 unk50;
    s32 unk58;
};

struct M2c_temp_3_111 {
    u8 pad_0[0x10];
    s32 unk10;
};

struct M2c_temp_3_141 {
    u8 pad_0[0x10];
    s64 unk10;
};

struct M2c_temp_3_80 {
    u8 pad_0[0x10];
    s64 unk10;
    u8 pad_18[0x58];
    s32 unk70;
};

#endif /* RNC_FUN_002012B8_TYPES_H */
