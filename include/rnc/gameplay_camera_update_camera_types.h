#ifndef RNC_GAMEPLAY_CAMERA_UPDATE_CAMERA_TYPES_H
#define RNC_GAMEPLAY_CAMERA_UPDATE_CAMERA_TYPES_H

#include "types.h"

struct M2c_D_00186F40 {
    u8 pad_0[0x140];
    f32 unk140;
    u8 pad_144[0x3C];
    s32 unk180;
    s32 unk184;
    u8 pad_188[0xE8];
    s16 unk270;
    u8 pad_272[0xDE];
    f32 unk350;
    u8 pad_354[0xC];
    f32 unk360;
    u8 pad_364[0xC];
    f32 unk370;
    u8 pad_374[0x24];
    s32 unk398;
};

struct M2c_D_001871B0 {
    s16 unk0;
    u8 unk2;
    u8 pad_3[0x3];
};

struct M2c_D_0018C318 {
    u8 pad_0[0x14];
    s32 unk14;
};

struct M2c_temp_17_36 {
    s32 unk0;
    u8 pad_4[0xC];
    s32 unk10;
    u8 pad_14[0xC];
    s32 unk20;
    u8 pad_24[0xC];
    s32 unk30;
};

#endif /* RNC_GAMEPLAY_CAMERA_UPDATE_CAMERA_TYPES_H */
