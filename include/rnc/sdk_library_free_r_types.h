#ifndef RNC_SDK_LIBRARY_FREE_R_TYPES_H
#define RNC_SDK_LIBRARY_FREE_R_TYPES_H

#include "types.h"

struct M2c_D_0012F788 {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
};

struct M2c_temp_3_65 {
    u8 pad_0[0xC];
    s32 unkC;
};

struct M2c_temp_3_84 {
    u8 pad_0[0x8];
    s32 unk8;
    s32 unkC;
};

struct M2c_temp_6_20 {
    u8 pad_0[0x4];
    s32 unk4;
    struct M2c_temp_6_20_unk8 * unk8;
    struct M2c_temp_8_96 * unkC;
};

struct M2c_temp_6_20_unk8 {
    u8 pad_0[0xC];
    s32 unkC;
};

struct M2c_temp_7_118 {
    u8 pad_0[0xC];
    s32 unkC;
};

struct M2c_temp_8_116 {
    u8 pad_0[0x8];
    struct M2c_temp_7_118 * unk8;
};

struct M2c_temp_8_32 {
    u8 pad_0[0x8];
    s32 unk8;
};

struct M2c_temp_8_71 {
    u8 pad_0[0x8];
    s32 unk8;
};

struct M2c_temp_8_96 {
    u8 pad_0[0x8];
    s32 unk8;
};

struct M2c_var_10_12 {
    u8 pad_0[0x4];
    s32 unk4;
    struct M2c_temp_3_65 * unk8;
    struct M2c_temp_8_71 * unkC;
};

struct M2c_var_7_170 {
    u8 pad_0[0x4];
    s32 unk4;
    struct M2c_var_7_170 * unk8;
    struct M2c_var_8_169 * unkC;
};

struct M2c_var_8_169 {
    u8 pad_0[0x8];
    struct M2c_var_7_170 * unk8;
};

#endif /* RNC_SDK_LIBRARY_FREE_R_TYPES_H */
