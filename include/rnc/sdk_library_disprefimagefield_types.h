#ifndef RNC_SDK_LIBRARY_DISPREFIMAGEFIELD_TYPES_H
#define RNC_SDK_LIBRARY_DISPREFIMAGEFIELD_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x80];
    s32 unk80;
    u8 pad_84[0x4];
    s32 unk88;
    u8 pad_8C[0x24];
    s32 unkB0;
    s32 unkB4;
    s32 unkB8;
    u8 pad_BC[0x4];
    s32 unkC0;
    s32 unkC4;
    u8 pad_C8[0x4];
    s32 unkCC;
    s32 unkD0;
    u8 pad_D4[0xA0];
    s32 unk174;
    u8 pad_178[0x6E0];
    struct M2c_temp_7_45 * unk858;
};

struct M2c_arg1 {
    u8 pad_0[0x10];
    s32 unk10;
    u8 pad_14[0x14];
    s32 unk28;
};

struct M2c_arg2 {
    u8 pad_0[0x28];
    s32 unk28;
};

struct M2c_temp_7_35 {
    u8 pad_0[0x10];
    s32 unk10;
};

struct M2c_temp_7_45 {
    u8 pad_0[0x20];
    s64 unk20;
    s32 unk28;
    u8 pad_2C[0xC];
    s64 unk38;
};

struct M2c_var_19_19 {
    u8 pad_0[0x44];
    s32 unk44;
    u8 pad_48[0x8];
    s32 unk50;
    u8 pad_54[0x8];
    s32 unk5C;
    s32 unk60;
};

struct M2c_var_20_20 {
    u8 pad_0[0x48];
    s32 unk48;
    u8 pad_4C[0x8];
    s32 unk54;
};

#endif /* RNC_SDK_LIBRARY_DISPREFIMAGEFIELD_TYPES_H */
