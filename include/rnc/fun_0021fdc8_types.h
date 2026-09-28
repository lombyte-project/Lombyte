#ifndef RNC_FUN_0021FDC8_TYPES_H
#define RNC_FUN_0021FDC8_TYPES_H

#include "types.h"

struct M2c_D_0013D290 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0xC8];
    s32 unkD4;
    u8 pad_D8[0x4];
    s32 unkDC;
};

struct M2c_D_001D5BF4 {
    u8 pad_0[0x40];
    struct M2c_D_001D5BF4_unk40 * unk40;
};

struct M2c_D_001D5BF4_unk40 {
    u8 pad_0[0x3C];
    s32 unk3C;
    s32 unk40;
};

struct M2c_arg0 {
    u8 pad_0[0x30];
    s32 unk30;
    s32 unk34;
    u8 pad_38[0x8];
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
};

struct M2c_temp_2_207 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_2_215 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_2_295 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_2_303 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_2_355 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_2_363 {
    s32 unk0;
    s32 unk4;
};

struct M2c_temp_4_139 {
    u8 pad_0[0x34];
    s32 unk34;
    u8 pad_38[0x8];
    s32 unk40;
};

struct M2c_temp_4_80 {
    u8 pad_0[0x3C];
    s32 unk3C;
    s32 unk40;
    u8 pad_44[0x4];
    s32 unk48;
};

#endif /* RNC_FUN_0021FDC8_TYPES_H */
