#ifndef RNC_VIDEO_DISPLAY_SET_PAL_MODE_TYPES_H
#define RNC_VIDEO_DISPLAY_SET_PAL_MODE_TYPES_H

#include "types.h"

struct M2c_D_0013CF10 {
    u8 pad_0[0x10];
    s32 unk10;
    u8 pad_14[0xC];
    s32 unk20;
    u8 pad_24[0xC];
    s32 unk30;
    u8 pad_34[0xC];
    s32 unk40;
    u8 pad_44[0xC];
    s64 unk50;
    u8 pad_58[0x8];
    s32 unk60;
    u8 pad_64[0xC];
    s32 unk70;
    u8 pad_74[0xC];
    s32 unk80;
};

struct M2c_D_0013E500 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
};

struct M2c_D_00151780 {
    u8 pad_0[0x150];
    s16 unk150;
    s32 unk152;
    u8 pad_156[0x2];
    s16 unk158;
    s16 unk15A;
    u8 pad_15C[0x2];
};

#endif /* RNC_VIDEO_DISPLAY_SET_PAL_MODE_TYPES_H */
