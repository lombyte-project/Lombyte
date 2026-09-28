#ifndef RNC_AUDIO_VOICES_SOUND_UPDATE_TYPES_H
#define RNC_AUDIO_VOICES_SOUND_UPDATE_TYPES_H

#include "types.h"

struct M2c_D_0013E550 {
    u8 pad_0[0x40];
    s32 unk40;
    u8 pad_44[0x4];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad_60[0x4];
    s32 unk64;
    u8 unk68;
    u8 unk69;
    u8 unk6A;
    u8 unk6B;
    s32 unk6C;
    u8 pad_70[0xD28];
    s32 unkD98;
};

struct M2c_temp_16_261 {
    u8 pad_0[0x20];
    s32 unk20;
};

struct M2c_temp_16_383 {
    u8 pad_0[0x75];
    u8 unk75;
    u8 pad_76[0x2];
    struct M2c_temp_16_383_unk78 * unk78;
    u8 pad_7C[0x4];
    s32 unk80;
};

struct M2c_temp_16_383_unk78 {
    u8 pad_0[0x19];
    u8 unk19;
    u8 pad_1A[0x3];
};

struct M2c_temp_16_504 {
    u8 pad_0[0x74];
    u8 unk74;
    u8 unk75;
    u8 pad_76[0x3A];
    s32 unkB0;
};

struct M2c_temp_16_722 {
    u8 pad_0[0x70];
    s32 unk70;
    u8 unk74;
    u8 pad_75[0x3];
    struct M2c_temp_16_722_unk78 * unk78;
    u8 pad_7C[0x8];
    s32 unk84;
    s32 unk88;
    s32 unk8C;
};

struct M2c_temp_16_722_unk78 {
    u8 pad_0[0x19];
    u8 unk19;
    u8 pad_1A[0x3];
};

struct M2c_temp_17_306 {
    u8 pad_0[0x75];
    u8 unk75;
    u8 pad_76[0x12];
    struct M2c_temp_17_306_unk88 * unk88;
    u8 pad_8C[0xC];
    s32 unk98;
};

struct M2c_temp_17_306_unk88 {
    u8 pad_0[0x10];
    s32 unk10;
};

struct M2c_temp_3_826 {
    u8 pad_0[0x1A];
    s16 unk1A;
    s32 unk1C;
};

struct M2c_temp_4_809 {
    u8 pad_0[0x74];
    u8 unk74;
    u8 pad_75[0x3];
    struct M2c_temp_3_826 * unk78;
};

struct M2c_var_19_223 {
    u8 pad_0[0x70];
    s32 unk70;
    u8 unk74;
    u8 unk75;
    u8 pad_76[0x2];
    struct M2c_var_19_223_unk78 * unk78;
    u8 pad_7C[0xC];
    struct M2c_temp_16_261 * unk88;
    u8 pad_8C[0xC];
    s32 unk98;
};

struct M2c_var_19_223_unk78 {
    u8 pad_0[0x18];
    u8 unk18;
    u8 unk19;
    u8 pad_1A[0x3];
};

#endif /* RNC_AUDIO_VOICES_SOUND_UPDATE_TYPES_H */
