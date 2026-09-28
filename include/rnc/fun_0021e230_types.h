#ifndef RNC_FUN_0021E230_TYPES_H
#define RNC_FUN_0021E230_TYPES_H

#include "types.h"

struct M2c_D_001863D0 {
    u8 pad_0[0x5C];
    s32 unk5C;
};

struct M2c_D_00186F40 {
    u8 pad_0[0x140];
    s32 unk140;
    s32 unk144;
    s32 unk148;
};

struct M2c_D_001D5BF0 {
    u8 pad_0[0x4];
    struct M2c_D_001D5BF0_unk4 * unk4;
    u8 pad_8[0x110];
    s32 unk118;
    s32 unk11C;
    s32 unk120;
    u8 pad_124[0x1C];
    s32 unk140;
    s32 unk144;
};

struct M2c_D_001D5BF0_unk4 {
    u8 pad_0[0x40];
    s32 unk40;
};

struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x18];
    s32 unk30;
    s32 unk34;
    s32 unk38;
    u8 pad_3C[0x8];
    struct M2c_temp_3_216 * unk44;
    s32 unk48;
};

struct M2c_temp_2_127 {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad_1C[0x8];
    struct M2c_temp_2_127_unk24 * unk24;
    u8 pad_28[0xC];
    s32 unk34;
    u8 pad_38[0x8];
    s32 unk40;
    s32 unk44;
    s32 unk48;
    u8 pad_4C[0x28];
    s32 unk74;
    struct M2c_temp_7_182 * unk78;
};

struct M2c_temp_2_127_unk24 {
    u8 pad_0[0xC];
    u8 unkC;
    u8 pad_D[0x3];
};

struct M2c_temp_2_169 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 unkC;
    u8 pad_D[0x3];
    s32 unk10;
    s32 unk14;
};

struct M2c_temp_2_210 {
    u8 pad_0[0x10];
    s64 unk10;
    u8 pad_18[0xC];
    struct M2c_temp_2_210_unk24 * unk24;
    u8 pad_28[0xC];
    s32 unk34;
    u8 pad_38[0x8];
    s64 unk40;
    u8 pad_48[0x2C];
    s32 unk74;
    struct M2c_temp_3_228 * unk78;
};

struct M2c_temp_2_210_unk24 {
    u8 pad_0[0xC];
    u8 unkC;
    u8 pad_D[0x3];
};

struct M2c_temp_3_20 {
    u8 pad_0[0x3C];
    s32 unk3C;
    u8 pad_40[0x8];
    s32 unk48;
};

struct M2c_temp_3_216 {
    u8 pad_0[0x10];
    s32 unk10;
    u8 pad_14[0x2C];
    s32 unk40;
};

struct M2c_temp_3_228 {
    u8 unk0;
    u8 pad_1[0xB];
    u8 unkC;
    u8 pad_D[0x3];
};

struct M2c_temp_7_182 {
    u8 unk0;
    u8 pad_1[0xB];
    u8 unkC;
    u8 pad_D[0x3];
};

struct M2c_temp_7_19 {
    u8 pad_0[0xA6];
    s32 unkA6;
};

#endif /* RNC_FUN_0021E230_TYPES_H */
