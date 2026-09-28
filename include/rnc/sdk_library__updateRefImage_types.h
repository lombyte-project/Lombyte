#ifndef RNC_SDK_LIBRARY__UPDATEREFIMAGE_TYPES_H
#define RNC_SDK_LIBRARY__UPDATEREFIMAGE_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0xA0];
    s32 unkA0;
    s32 unkA4;
    u8 pad_A8[0x40];
    s32 unkE8;
    u8 pad_EC[0x50];
    s32 unk13C;
    u8 pad_140[0x8];
    s32 unk148;
    s32 unk14C;
    s32 unk150;
    u8 pad_154[0x20];
    s32 unk174;
    s32 unk178;
    u8 pad_17C[0x8];
    s32 unk184;
    s32 unk188;
    s32 unk18C;
    s32 unk190;
    s32 unk194;
    s32 unk198;
    s32 unk19C;
    s32 unk1A0;
    s32 unk1A4;
    s32 unk1A8;
    u8 pad_1AC[0xC];
    struct M2c_arg0_unk1B8 * unk1B8;
    u8 * unk1BC;
    struct M2c_var_8_8 * unk1C0;
    s32 unk1C4;
    struct M2c_arg0_unk1C8 * unk1C8;
    struct M2c_arg0_unk1CC * unk1CC;
    struct M2c_var_8_8 * unk1D0;
    s32 unk1D4;
    struct M2c_arg0_unk1D8 * unk1D8;
    u8 * unk1DC;
    struct M2c_var_8_8 * unk1E0;
    s32 unk1E4;
    u8 pad_1E8[0x640];
    s32 unk828;
    u8 pad_82C[0x4];
    s32 unk830;
};

struct M2c_arg0_unk1B8 {
    u8 pad_0[0x28];
    s32 unk28;
};

struct M2c_arg0_unk1C8 {
    u8 pad_0[0x28];
    s32 unk28;
};

struct M2c_arg0_unk1CC {
    u8 pad_0[0x28];
    s32 unk28;
};

struct M2c_arg0_unk1D8 {
    u8 pad_0[0x28];
    s32 unk28;
};

struct M2c_var_2_113 {
    u8 pad_0[0x28];
    s32 unk28;
};

struct M2c_var_3_55 {
    u8 pad_0[0x28];
    s32 unk28;
};

struct M2c_var_8_8 {
    u8 pad_0[0x18];
    s64 unk18;
    s64 unk20;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
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

#endif /* RNC_SDK_LIBRARY__UPDATEREFIMAGE_TYPES_H */
