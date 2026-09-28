#ifndef RNC_UI_MAP_DRAW_MAP_OVERLAY_TYPES_H
#define RNC_UI_MAP_DRAW_MAP_OVERLAY_TYPES_H

#include "types.h"

struct M2c_D_0013E500 {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
};

struct M2c_D_0013F350 {
    u8 pad_0[0x80];
    f32 unk80;
    f32 unk84;
    u8 pad_88[0x10];
    f32 unk98;
    u8 pad_9C[0x1FF0];
    s32 unk208C;
};

struct M2c_D_00160F00 {
    s32 unk0;
    u16 unk4;
    u8 pad_6[0x2];
    s32 unk8;
    s32 unkC;
};

struct M2c_D_0019A3E8 {
    u8 pad_0[0x20];
    s32 unk20;
    s32 unk24;
};

struct M2c_D_001A00F0 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0xC];
    s32 unk18;
    s32 unk1C;
    struct M2c_D_001A00F0_unk20 * unk20;
    s32 unk24;
    u8 pad_28[0x4];
    s32 unk2C;
    u8 pad_30[0x1F8];
    s32 unk228;
    u8 pad_22C[0x14];
    s32 unk240;
};

struct M2c_D_001A00F0_unk20 {
    u8 pad_0[0x4];
    u16 unk4;
    u8 pad_6[0x26];
    u16 unk2C;
    u8 pad_2E[0x2];
};

struct M2c_temp_10_562 {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct M2c_temp_3_259 {
    u8 pad_0[0x10];
    s64 unk10;
};

struct M2c_packet {
    s64 unk0;
    s64 unk8;
    s64 unk10;
    s64 unk18;
    s64 unk20;
    s64 unk28;
    s64 unk30;
    s64 unk38;
    s64 unk40;
    s64 unk48;
};

struct M2c_temp_3_543 {
    u8 pad_0[0x4];
    u16 unk4;
    s32 unk6;
    u8 pad_A[0x1A];
    s32 unk24;
};

struct M2c_temp_3_62 {
    u8 pad_0[0xB4];
    s32 unkB4;
    u8 pad_B8[0x4C];
    s32 unk104;
    u8 pad_108[0x4C];
    s32 unk154;
};

struct M2c_temp_3_865 {
    u8 pad_0[0x4];
    s32 unk4;
    u8 pad_8[0x6];
    u16 unkE;
    u16 unk10;
    u16 unk12;
    s32 unk14;
};

struct M2c_temp_4_460 {
    u8 pad_0[0x6];
    u8 unk6;
    s32 unk7;
};

struct M2c_temp_4_590 {
    u8 pad_0[0x4];
    u16 unk4;
    s32 unk6;
    u8 pad_A[0x1A];
    s32 unk24;
};

struct M2c_temp_4_696 {
    u8 pad_0[0x4];
    u16 unk4;
    u16 unk6;
    s32 unk8;
    u8 pad_C[0x18];
    s32 unk24;
};

struct M2c_temp_5_407 {
    u8 pad_0[0x4];
    u16 unk4;
    u16 unk6;
    s32 unk8;
    u8 pad_C[0x18];
    s32 unk24;
};

struct M2c_temp_5_438 {
    u8 pad_0[0x4];
    s32 unk4;
    u8 pad_8[0x14];
    s32 unk1C;
};

struct M2c_temp_6_171 {
    s64 unk0;
    s64 unk8;
    s64 unk10;
    s64 unk18;
    s64 unk20;
    s64 unk28;
    s64 unk30;
    s64 unk38;
    s64 unk40;
    s64 unk48;
};

struct M2c_temp_7_261 {
    s64 unk0;
    s64 unk8;
    s64 unk10;
    s64 unk18;
    s64 unk20;
    s64 unk28;
    s64 unk30;
    s64 unk38;
    s64 unk40;
    s64 unk48;
};

struct M2c_var_16_402 {
    s32 unk0;
    u16 unk4;
    u8 pad_6[0x2];
    s32 unk8;
    s32 unkC;
};

struct M2c_var_19_691 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct M2c_var_4_740 {
    s16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    s16 unk8;
    u8 pad_A[0x16];
    f32 unk20;
};

struct M2c_var_6_566 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

union M2c_slot {
    u8 b[0x80];
    f32 f[2];
};

#endif /* RNC_UI_MAP_DRAW_MAP_OVERLAY_TYPES_H */
