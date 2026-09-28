#ifndef RNC_FUN_00224E18_TYPES_H
#define RNC_FUN_00224E18_TYPES_H

#include "types.h"

struct M2c_D_001D5E10 {
    u8 pad_0[0x1];
    u8 unk1;
    u8 pad_2[0x1E];
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

struct M2c_D_001D5E50 {
    u8 pad_0[0x1];
    u8 unk1;
    u8 pad_2[0x1E];
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

struct M2c_arg0 {
    u8 pad_0[0x10];
    s64 unk10;
    u8 pad_18[0xC];
    s32 unk24;
    u8 pad_28[0x28];
    s32 unk50;
    s32 unk54;
    u8 pad_58[0x10];
    s32 unk68;
    s32 unk6C;
    u8 pad_70[0x8];
    s32 unk78;
};

#endif /* RNC_FUN_00224E18_TYPES_H */
