#ifndef RNC_FUN_0021E698_TYPES_H
#define RNC_FUN_0021E698_TYPES_H

#include "types.h"

struct M2c_D_00186F40 {
    u8 pad_0[0x140];
    f32 unk140;
    f32 unk144;
    f32 unk148;
};

struct M2c_arg0 {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad_1C[0x2C];
    s32 unk48;
    u8 pad_4C[0x2C];
    struct M2c_temp_2_14 * unk78;
};

struct M2c_temp_2_14 {
    struct M2c_temp_3_15 * unk0;
    u8 pad_4[0x8];
    s32 unkC;
};

struct M2c_temp_2_41 {
    u8 pad_0[0x8];
    s32 unk8;
    s32 unkC;
    u8 pad_10[0x8];
    s32 unk18;
    s32 unk1C;
};

struct M2c_temp_3_15 {
    u8 pad_0[0x30];
    s32 unk30;
    u8 pad_34[0x4];
    s32 unk38;
};

#endif /* RNC_FUN_0021E698_TYPES_H */
