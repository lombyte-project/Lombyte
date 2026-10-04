#ifndef RNC_FUN_0022F778_TYPES_H
#define RNC_FUN_0022F778_TYPES_H

#include "types.h"

struct M2c_D_0013D4C0 {
    u8 pad_0[0x6];
    u8 unk6;
};

struct M2c_D_0013E030 {
    struct M2c_D_0013E030_unk0 * unk0;
    u8 pad_4[0x4];
    struct M2c_D_0013E030_unk8 * unk8;
    u8 pad_C[0x14];
    s32 unk20;
    u16 unk24;
    s16 unk26;
    s16 unk28;
    u8 pad_2A[2];
    s32 unk2C;
};

struct M2c_D_0013E030_unk0 {
    u8 pad_0[0x34];
    u16 unk34;
    u8 pad_36[2];
    u8 pad_38[0x10];
    f32 unk48;
    u8 pad_4C[0x66];
    s16 unkB2;
    u8 pad_B4[0x8];
    s8 unkBC;
};

struct M2c_D_0013E030_unk8 {
    u8 pad_0[0x24];
    struct M2c_D_0013E030_unk8_unk24 * unk24;
    u8 pad_28[0xA];
    u16 unk32;
    u16 unk34;
    s64 unk38;
    u8 pad_40[0x33];
    u8 unk73;
};

struct M2c_D_0013E030_unk8_unk24 {
    u8 pad_0[0x6];
    u8 unk6;
};

struct M2c_D_0013F350 {
    u8 pad_0[0x2080];
    struct M2c_D_0013F350_unk2080 * unk2080;
};

struct M2c_D_0013F350_unk2080 {
    u8 pad_0[0x38];
    s64 unk38;
};

#include "rnc/music_stream_state.h"

struct M2c_D_0018CB20 {
    u8 pad_0[0x34];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s16 unk40;
    u8 pad_42[2];
    s16 unk44;
    u8 pad_46[0xE];
    s32 unk54;
};

struct M2c_temp_16_1014 {
    u8 pad_0[0x10];
    f32 unk10;
    f32 unk14;
    u8 pad_18[0x4];
    f32 unk1C;
};

typedef struct { f32 x, y, z, w; } __attribute__((aligned(16))) Vec4;

struct M2c_temp_16_1110 {
    u8 pad_0[0x3C];
    s32 unk3C;
    s32 unk40;
    f32 unk44;
    u8 pad_48[0x8];
    s32 unk50;
    s32 unk54;
    u8 pad_58[0x28];
    Vec4 unk80;
    Vec4 unk90;
    Vec4 unkA0;
    u8 pad_A8[0x4];
    f32 unkB4;
    f32 unkB8;
};

struct M2c_temp_16_308 {
    s32 unk0;
    u8 pad_4[0x8];
    s32 unkC;
    f32 unk10;
};

struct M2c_temp_16_893 {
    u8 pad_0[0x24];
    s16 unk24;
    s16 unk26;
};

struct M2c_temp_17_1007 {
    s32 unk0;
    u8 pad_4[0xC];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
};

struct M2c_temp_17_1234 {
    u8 pad_0[0x24];
    s16 unk24;
    u8 pad_26[0xA];
    s32 unk30;
    u8 pad_34[0xC];
    f32 unk40;
    u8 pad_44[0x60];
    f32 unkA4;
    f32 unkA8;
    u8 pad_AC[0x8];
    f32 unkB4;
    f32 unkB8;
};

struct M2c_temp_17_310 {
    u8 pad_0[0x4];
    f32 unk4;
    f32 unk8;
    f32 unkC;
};

struct M2c_temp_18_399 {
    u8 pad_0[0x38];
    s64 unk38;
    u8 pad_40[0x8];
    s32 unk48;
    u8 pad_4C[0x4];
    u8 unk50;
    u8 unk51;
    u8 pad_52[0x2];
    f32 unk54;
    u8 pad_58[0x19];
    u8 unk71;
    u8 pad_72[0x6];
    s32 unk78;
    u8 pad_7C[0x3];
    u8 unk7F;
    u8 pad_80[0x10];
    s32 unk90;
    u8 pad_94[0x12];
    s16 unkA6;
    u8 pad_A8[0xA];
    s16 unkB2;
    u8 pad_B4[0x8];
    u8 unkBC;
};

struct M2c_temp_18_940 {
    u8 pad_0[0x10];
    struct M2c_temp_18_940_unk10 * unk10;
    struct M2c_temp_18_940_unk14 * unk14;
    u8 pad_18[0x18];
    s32 unk30;
    u8 pad_34[0x14];
    s32 unk48;
    s32 unk4C;
};

struct M2c_temp_18_940_unk10 {
    u8 pad_0[0x72];
    u8 unk72;
    u8 pad_73[0x21];
    s32 unk94;
};

struct M2c_temp_18_940_unk14 {
    u8 pad_0[0x38];
    s64 unk38;
    u8 pad_3C[0x36];
    u8 unk72;
    u8 pad_73[0x21];
    s32 unk94;
};

struct M2c_temp_19_1379 {
    u8 pad_0[0x26];
    s16 unk26;
    u8 pad_2A[0x12];
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    u8 pad_48[0x8];
    s32 unk50;
    s32 unk54;
};

struct M2c_temp_2_1026 {
    u8 pad_0[0x10];
    f32 unk10;
    f32 unk14;
};

struct M2c_temp_2_1061 {
    u8 pad_0[0x10];
    f32 unk10;
    f32 unk14;
};

struct M2c_temp_2_1075 {
    u8 pad_0[0x10];
    f32 unk10;
    f32 unk14;
};

struct M2c_temp_2_349 {
    u8 pad_0[0x350];
    f32 unk350;
    f32 unk354;
    f32 unk358;
    u8 pad_35C[0x4];
    f32 unk360;
    f32 unk364;
    f32 unk368;
    u8 pad_36C[0x4];
    f32 unk370;
    f32 unk374;
    f32 unk378;
};

struct M2c_temp_2_570 {
    u8 pad_0[0x32];
    u16 unk32;
};

struct M2c_temp_2_955 {
    u8 pad_0[0x32];
    u16 unk32;
};

struct M2c_temp_2_973 {
    u8 pad_0[0x32];
    u16 unk32;
};

struct M2c_temp_3_1015 {
    u8 pad_0[0x10];
    f32 unk10;
    f32 unk14;
};

struct M2c_temp_3_1060 {
    u8 pad_0[0x10];
    f32 unk10;
    f32 unk14;
};

struct M2c_temp_3_198 {
    u8 pad_0[0x24];
    struct M2c_temp_3_205 * unk24;
};

struct M2c_temp_3_205 {
    u8 pad_0[0xC];
    u8 unkC;
};

struct M2c_temp_4_1517 {
    u8 pad_0[0x40];
    Vec4 unk40;
};

struct M2c_temp_4_1538 {
    u8 pad_0[0x40];
    Vec4 unk40;
};

struct M2c_temp_4_981 {
    u8 pad_0[0x34];
    u16 unk34;
};

struct M2c_temp_5_201 {
    u8 pad_0[0xC];
    u8 unkC;
};

struct M2c_temp_5_966 {
    u8 pad_0[0x34];
    u16 unk34;
};

struct M2c_temp_5_971 {
    u8 pad_0[0x38];
    s64 unk38;
};

struct M2c_unk24 {
    u8 pad_0[0x6];
    u8 unk6;
};

struct M2c_var_16_1502 {
    u8 pad_0[0x10];
    struct M2c_temp_4_1517 * unk10;
    struct M2c_temp_4_1538 * unk14;
    u8 pad_18[0xE];
    s16 unk26;
};

struct M2c_var_17_904 {
    u8 pad_0[0x34];
    s32 unk34;
    s32 unk38;
    u8 pad_3C[0xC];
    f32 unk48;
};

struct M2c_var_3_1096 {
    u8 pad_0[0x1C];
    f32 unk1C;
};

struct M2c_var_5_227 {
    u8 pad_0[0x20];
    u8 unk20;
    u8 pad_21[0x13];
    u16 unk34;
    u8 pad_36[0x70];
    s16 unkA6;
};

#endif /* RNC_FUN_0022F778_TYPES_H */
