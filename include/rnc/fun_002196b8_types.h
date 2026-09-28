#ifndef RNC_FUN_002196B8_TYPES_H
#define RNC_FUN_002196B8_TYPES_H

#include "types.h"

struct M2c_D_001D5BF0 {
    u8 pad_0[0x4];
    s32 unk4;
    u8 pad_8[0xD0];
    s32 unkD8;
};

struct M2c_temp_16_91 {
    s32 unk0;
    u8 pad_4[0xC];
    s32 unk10;
    u8 pad_14[0xC];
    s32 unk20;
    u8 pad_24[0xC];
    s32 unk30;
    u8 pad_34[0x1C];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
};

struct M2c_temp_2_124 {
    u8 pad_0[0x20];
    s32 unk20;
};

struct M2c_temp_3_229 {
    u8 pad_0[0x50];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
};

struct M2c_temp_4_187 {
    u8 pad_0[0x4];
    s32 unk4;
    u8 pad_8[0x8];
    s32 unk10;
};

struct M2c_temp_4_306 {
    u8 pad_0[0x4];
    s32 unk4;
};

struct M2c_temp_4_75 {
    u8 pad_0[0x78];
    struct M2c_temp_16_91 * unk78;
};

struct M2c_temp_7_182 {
    u8 pad_0[0x78];
    struct M2c_temp_3_229 * unk78;
};

#endif /* RNC_FUN_002196B8_TYPES_H */
