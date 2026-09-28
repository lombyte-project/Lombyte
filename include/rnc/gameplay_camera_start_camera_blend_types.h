#ifndef RNC_GAMEPLAY_CAMERA_START_CAMERA_BLEND_TYPES_H
#define RNC_GAMEPLAY_CAMERA_START_CAMERA_BLEND_TYPES_H

#include "types.h"

struct M2c_D_0013F350 {
    u8 pad_0[0x2080];
    s32 unk2080;
};

struct M2c_D_001871B0 {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u8 pad_4[0xC];
    s32 unk10;
    u8 pad_14[0x1C];
    s64 unk30;
    u8 pad_38[0x8];
    s64 unk40;
    u8 pad_48[0x8];
    s64 unk50;
    u8 pad_58[0x8];
    s32 unk60;
    u8 pad_64[0x4C];
    s64 unkB0;
    u8 pad_B8[0x8];
    s64 unkC0;
    u8 pad_C8[0x8];
    s64 unkD0;
};

struct M2c_arg0 {
    u8 pad_0[0x30];
    s32 unk30;
};

struct M2c_temp_16_135 {
    u8 pad_0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
};

struct M2c_temp_3_117 {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
};

#endif /* RNC_GAMEPLAY_CAMERA_START_CAMERA_BLEND_TYPES_H */
