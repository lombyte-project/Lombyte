#ifndef RNC_FUN_001F92B0_TYPES_H
#define RNC_FUN_001F92B0_TYPES_H

#include "types.h"

struct M2c_D_00160F00 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct M2c_D_0018CD00 {
    u8 pad_0[0x1A8];
    f32 unk1A8;
    u8 pad_1AC[0x64];
    s32 unk210;
};

struct M2c_temp_16_167 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    u8 pad_14[0x4];
    s32 unk18;
    u8 pad_1C[0x4];
    s32 unk20;
    u8 pad_24[0x4];
    s64 unk28;
    s32 unk30;
    u8 pad_34[0x4];
    s32 unk38;
    u8 pad_3C[0x4];
    s64 unk40;
    s32 unk48;
    u8 pad_4C[0x4];
    s32 unk50;
    u8 pad_54[0x4];
    s64 unk58;
    s32 unk60;
    u8 pad_64[0x4];
    s32 unk68;
    u8 pad_6C[0x4];
    s64 unk70;
    s32 unk78;
};

struct M2c_temp_18_34 {
    u8 pad_0[0x10];
    u16 unk10;
    s32 unk12;
    u8 pad_16[0x2];
    s32 unk18;
    s32 unk1C;
};

struct M2c_temp_5_160 {
    u8 pad_0[0x10];
    s64 unk10;
    u8 pad_18[0x8];
    s32 unk20;
};

#endif /* RNC_FUN_001F92B0_TYPES_H */
