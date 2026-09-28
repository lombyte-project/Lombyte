#ifndef RNC_FUN_0021A328_TYPES_H
#define RNC_FUN_0021A328_TYPES_H

#include "types.h"

struct M2c_D_001D5BF4 {
    u8 pad_0[0x40];
    struct M2c_D_001D5BF4_unk40 * unk40;
};

struct M2c_D_001D5BF4_unk40 {
    u8 pad_0[0x40];
    s32 unk40;
};

struct M2c_arg0 {
    u8 pad_0[0x20];
    s32 unk20;
    s32 unk24;
    u8 pad_28[0x8];
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    u8 pad_40[0x4];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
};

struct M2c_temp_2_121 {
    u8 pad_0[0x34];
    s32 unk34;
    u8 pad_38[0x8];
    s32 unk40;
};

struct M2c_temp_3_136 {
    u8 pad_0[0x3C];
    s32 unk3C;
    u8 pad_40[0x8];
    s32 unk48;
};

struct M2c_temp_3_91 {
    u8 pad_0[0x3C];
    s32 unk3C;
    u8 pad_40[0x8];
    struct M2c_temp_5_95 * unk48;
};

struct M2c_temp_5_95 {
    u8 pad_0[0x4];
    s16 unk4;
    s32 unk6;
};

#endif /* RNC_FUN_0021A328_TYPES_H */
