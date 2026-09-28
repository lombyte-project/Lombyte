#ifndef RNC_SDK_LIBRARY__DISPREFIMAGE_TYPES_H
#define RNC_SDK_LIBRARY__DISPREFIMAGE_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x80];
    s32 unk80;
    u8 pad_84[0x4];
    s64 unk88;
    u8 pad_90[0x20];
    s32 unkB0;
    s32 unkB4;
    s32 unkB8;
    s32 unkBC;
    s32 unkC0;
    s32 unkC4;
    s32 unkC8;
    s32 unkCC;
    s32 unkD0;
    u8 pad_D4[0x784];
    struct M2c_temp_7_14 * unk858;
};

struct M2c_arg1 {
    u8 pad_0[0x28];
    s32 unk28;
    u8 pad_2C[0x18];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
};

struct M2c_temp_7_14 {
    u8 pad_0[0x10];
    s32 unk10;
    u8 pad_14[0xC];
    s32 unk20;
};

#endif /* RNC_SDK_LIBRARY__DISPREFIMAGE_TYPES_H */
