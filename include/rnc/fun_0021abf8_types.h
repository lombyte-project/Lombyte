#ifndef RNC_FUN_0021ABF8_TYPES_H
#define RNC_FUN_0021ABF8_TYPES_H

#include "types.h"

struct M2c_D_0013C940 {
    u8 pad_0[0x1B4];
    s32 unk1B4;
    u8 pad_1B8[0xC];
    s32 unk1C4;
};

struct M2c_D_001D5BF0 {
    u8 pad_0[0x4];
    struct M2c_D_001D5BF0_unk4 * unk4;
    s32 unk8;
    u8 pad_C[0x118];
    s32 unk124;
};

struct M2c_D_001D5BF0_unk4 {
    u8 pad_0[0x38];
    s32 unk38;
    u8 pad_3C[0x4];
    s32 unk40;
    u8 pad_44[0x3C];
    s32 unk80;
};

struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x18];
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
};

struct M2c_temp_2_366 {
    u8 pad_0[0xC];
    s16 unkC;
    u8 pad_E[0x2];
};

struct M2c_temp_3_273 {
    u8 pad_0[0x4];
    s32 unk4;
    u8 pad_8[0x4];
    s32 unkC;
    u8 pad_10[0xE0];
    s32 unkF0;
    s32 unkF4;
};

struct M2c_var_3_32 {
    u8 pad_0[0xA];
    s32 unkA;
};

struct M2c_var_7_201 {
    u8 pad_0[0x4];
    s32 unk4;
    u8 pad_8[0x4];
    s32 unkC;
    u8 pad_10[0xD4];
    s32 unkE4;
    u8 pad_E8[0x8];
    s32 unkF0;
    s32 unkF4;
};

#endif /* RNC_FUN_0021ABF8_TYPES_H */
