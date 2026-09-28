#ifndef RNC_FUN_00203B08_TYPES_H
#define RNC_FUN_00203B08_TYPES_H

#include "types.h"

struct M2c_D_0019E540 {
    u8 pad_0[0x18];
    s32 unk18;
};

struct M2c_arg0 {
    u8 pad_0[0x10];
    s32 unk10;
    u8 pad_14[0x12];
    u16 unk26;
};

struct M2c_arg3 {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    s32 unkE;
};

struct M2c_temp_17_256 {
    u8 pad_0[0x4];
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    s32 unkE;
};

struct M2c_temp_18_82 {
    u8 pad_0[0x10];
    s64 unk10;
    s32 unk14;
    u8 pad_18[0x8];
    s64 unk20;
    u8 pad_28[0x8];
    s64 unk30;
};

struct M2c_temp_3_233 {
    u8 pad_0[0x10];
    s32 unk10;
};

struct M2c_var_12_57 {
    u8 pad_0[0x16];
    u16 unk16;
    s32 unk18;
    u8 * unk1C;
    u8 pad_20[0x8];
    s32 unk28;
    u8 * unk2C;
};

struct M2c_var_13_234 {
    u8 pad_0[0x4];
    s32 unk4;
};

struct M2c_var_20_240 {
    s64 unk0;
    s32 unk4;
    u8 pad_8[0x8];
    s64 unk10;
    s32 unk14;
    u8 pad_18[0x8];
    s64 unk20;
    u8 pad_28[0x8];
    s64 unk30;
};

#endif /* RNC_FUN_00203B08_TYPES_H */
