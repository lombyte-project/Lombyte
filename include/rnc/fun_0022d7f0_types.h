#ifndef RNC_FUN_0022D7F0_TYPES_H
#define RNC_FUN_0022D7F0_TYPES_H

#include "types.h"

struct M2c_D_0013E550 {
    u8 pad_0[0x74];
    u8 unk74;
    u8 pad_75[0x3];
};

struct M2c_D_0013F350 {
    u8 pad_0[0x1090];
    s32 unk1090;
    u8 pad_1094[0xFEC];
    s32 unk2080;
};

struct M2c_arg0 {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
    u16 unk18;
    s32 unk1A;
};

struct M2c_arg2 {
    u8 pad_0[0x10];
    s32 unk10;
    u8 pad_14[0x92];
    s16 unkA6;
};

struct M2c_temp_2_142 {
    u8 pad_0[0x74];
    u8 unk74;
    u8 unk75;
    u8 pad_76[0x3A];
    s32 unkB0;
};

struct M2c_temp_2_69 {
    u8 pad_0[0x78];
    s32 unk78;
    u16 unk7C;
    u16 unk7E;
    s32 unk80;
    u8 pad_84[0x4];
    s32 unk88;
    s32 unk8C;
};

struct M2c_temp_3_160 {
    u8 pad_0[0x70];
    s32 unk70;
    u8 pad_74[0x10];
    s32 unk84;
};

struct M2c_temp_4_97 {
    u8 pad_0[0x98];
    s32 unk98;
};

#endif /* RNC_FUN_0022D7F0_TYPES_H */
