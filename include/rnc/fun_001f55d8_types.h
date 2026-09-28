#ifndef RNC_FUN_001F55D8_TYPES_H
#define RNC_FUN_001F55D8_TYPES_H

#include "types.h"

struct M2c_D_0013E500 {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
};

struct M2c_D_00160F00 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct M2c_temp_3_84 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    u8 pad_14[0x4];
    s64 unk18;
    s64 unk20;
    s64 unk28;
    s64 unk30;
    s64 unk38;
    s64 unk40;
    s64 unk48;
    s64 unk50;
    s32 unk58;
};

struct M2c_temp_7_78 {
    u8 pad_0[0x10];
    s64 unk10;
    u8 pad_18[0x8];
    s32 unk20;
};

#endif /* RNC_FUN_001F55D8_TYPES_H */
