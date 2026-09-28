#ifndef RNC_UI_HUD_DRAW_HUD_SPRITE_TYPES_H
#define RNC_UI_HUD_DRAW_HUD_SPRITE_TYPES_H

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

struct M2c_D_0019A3E8 {
    u8 pad_0[0xC];
    s32 unkC;
    u8 pad_10[0x10];
    s32 unk20;
    s32 unk24;
};

struct M2c_temp_10_41 {
    u8 pad_0[0x6];
    u8 unk6;
    s32 unk7;
};

struct M2c_temp_18_58 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x4];
    s32 unk10;
    u8 pad_14[0x4];
    s32 unk18;
    u8 pad_1C[0x4];
    s64 unk20;
    s32 unk28;
    u8 pad_2C[0x4];
    s64 unk30;
    s64 unk38;
    s64 unk40;
    s32 unk48;
};

struct M2c_temp_2_57 {
    u8 pad_0[0x10];
    s64 unk10;
};

#endif /* RNC_UI_HUD_DRAW_HUD_SPRITE_TYPES_H */
