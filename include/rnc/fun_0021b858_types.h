#ifndef RNC_FUN_0021B858_TYPES_H
#define RNC_FUN_0021B858_TYPES_H

#include "types.h"

struct M2c_D_0013C940 {
    u8 pad_0[0x1C4];
    s32 unk1C4;
};

struct M2c_D_0013F350 {
    u8 pad_0[0x1FF5];
    u8 unk1FF5;
    u8 pad_1FF6[0x1];
    u8 unk1FF7;
};

struct M2c_D_001D5BF0 {
    s32 unk0;
    struct M2c_D_001D5BF0_unk4 * unk4;
    s32 unk8;
    u8 pad_C[0x118];
    s32 unk124;
    u8 pad_128[0xC];
    s32 unk134;
    s32 unk138;
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
    u8 pad_34[0x8];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    struct M2c_temp_17_307 * unk48;
    struct M2c_var_5_68 * unk4C;
    struct M2c_var_5_154 * unk50;
    u8 * unk54;
    u8 * unk58;
};

struct M2c_temp_17_307 {
    u8 pad_0[0x6];
    s16 unk6;
};

struct M2c_var_5_154 {
    u8 pad_0[0x30];
    s32 unk30;
    u8 pad_34[0x8];
    s32 unk3C;
    u8 pad_40[0x4];
    s32 unk44;
    u8 pad_48[0x8];
    struct M2c_var_5_154 * unk50;
};

struct M2c_var_5_68 {
    u8 pad_0[0x30];
    s32 unk30;
    u8 pad_34[0x8];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    u8 pad_48[0x4];
    struct M2c_var_5_68 * unk4C;
};

#endif /* RNC_FUN_0021B858_TYPES_H */
