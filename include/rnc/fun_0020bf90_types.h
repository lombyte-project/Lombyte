#ifndef RNC_FUN_0020BF90_TYPES_H
#define RNC_FUN_0020BF90_TYPES_H

#include "types.h"

struct M2c_D_0013F350 {
    u8 pad_0[0x80];
    s32 unk80;
    s32 unk84;
    s32 unk88;
};

struct M2c_D_001A00F0 {
    u8 pad_0[0x20];
    struct M2c_D_001A00F0_unk20 * unk20;
};

struct M2c_D_001A00F0_unk20 {
    u8 pad_0[0x4];
    u16 unk4;
    u8 pad_6[0x2];
};

struct M2c_D_001A2C10 {
    u8 pad_0[0x10];
    s32 unk10;
};

struct M2c_temp_16_107 {
    s16 unk0;
    u8 pad_2[0x16];
    s32 unk18;
    s32 unk1C;
};

struct M2c_temp_16_247 {
    s16 unk0;
    u16 unk2;
    u16 unk4;
    u8 pad_6[0x8];
    u16 unkE;
    u16 unk10;
    u8 pad_12[0x12];
    s16 unk24;
    u8 pad_26[0x2];
};

struct M2c_temp_2_191 {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
};

struct M2c_temp_2_92 {
    u8 pad_0[0x4];
    u16 unk4;
    u8 pad_6[0x2];
};

struct M2c_temp_3_283 {
    u8 pad_0[0x4];
    u16 unk4;
    u8 pad_6[0x4];
    s16 unkA;
    u8 pad_C[0x2];
};

struct M2c_temp_5_66 {
    u8 pad_0[0x104];
    s32 unk104;
    u8 pad_108[0x4C];
    s32 unk154;
};

#endif /* RNC_FUN_0020BF90_TYPES_H */
