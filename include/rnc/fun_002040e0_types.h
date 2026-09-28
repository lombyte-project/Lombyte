#ifndef RNC_FUN_002040E0_TYPES_H
#define RNC_FUN_002040E0_TYPES_H

#include "types.h"

struct M2c_D_00160EA0 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
};

struct M2c_arg0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct M2c_temp_18_140 {
    u8 pad_0[0x10];
    s64 unk10;
};

struct M2c_temp_18_150 {
    u8 pad_0[0x10];
    s64 unk10;
};

struct M2c_temp_18_76 {
    s64 unk0;
    u8 pad_8[0x8];
    s64 unk10;
    s32 unk14;
    u8 pad_18[0x8];
    s32 unk20;
    s32 unk24;
};

struct M2c_temp_20_82 {
    u8 pad_0[0x4];
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    s32 unkE;
};

struct M2c_temp_2_67 {
    u8 pad_0[0x10];
    struct M2c_temp_18_76 * unk10;
    u8 pad_14[0x8];
    s32 unk1C;
};

struct M2c_var_3_40 {
    u8 pad_0[0x10];
    s32 unk10;
};

#endif /* RNC_FUN_002040E0_TYPES_H */
