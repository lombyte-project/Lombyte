#ifndef RNC_FUN_0021F990_TYPES_H
#define RNC_FUN_0021F990_TYPES_H

#include "types.h"

struct M2c_D_0013D290 {
    u8 pad_0[0xD4];
    s32 unkD4;
    u8 pad_D8[0x4];
    s32 unkDC;
};

struct M2c_D_001A00F0 {
    u8 pad_0[0x244];
    s32 unk244;
    s32 unk248;
    u8 pad_24C[0xC];
    s32 unk258;
};

struct M2c_D_001D5BF4 {
    u8 pad_0[0x40];
    struct M2c_D_001D5BF4_unk40 * unk40;
};

struct M2c_D_001D5BF4_unk40 {
    u8 pad_0[0x3C];
    s32 unk3C;
    s32 unk40;
};

struct M2c_arg0 {
    u8 pad_0[0x30];
    s32 unk30;
    s32 unk34;
    u8 pad_38[0xC];
    s32 unk44;
    struct M2c_temp_16_170 * unk48;
    u8 pad_4C[0x4];
    s32 unk50;
    u8 pad_54[0x4];
    s32 unk58;
    u8 pad_5C[0x4];
    s32 unk60;
};

struct M2c_temp_16_170 {
    u8 pad_0[0x8];
    s32 unk8;
    s32 unkC;
};

struct M2c_temp_2_131 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_2_139 {
    s32 unk0;
    s32 unk4;
};

struct M2c_var_5_0 {
    u8 pad_0[0x40];
    struct M2c_var_5_0_unk40 * unk40;
};

struct M2c_var_5_0_unk40 {
    u8 pad_0[0x40];
    s32 unk40;
};

#endif /* RNC_FUN_0021F990_TYPES_H */
