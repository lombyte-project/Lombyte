#ifndef RNC_GAMEPLAY_ENTITIES_ATTACH_MANIPULATOR_TYPES_H
#define RNC_GAMEPLAY_ENTITIES_ATTACH_MANIPULATOR_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x24];
    struct M2c_arg0_unk24 * unk24;
    u8 pad_28[0x3C];
    s32 unk64;
};

struct M2c_arg0_unk24 {
    u8 pad_0[0x1C];
    s32 unk1C;
};

struct M2c_arg2 {
    u8 unk0;
    u8 unk1;
    u8 pad_2[0x2];
    s32 unk4;
    s32 unk8;
    u8 pad_C[0x10];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

#endif /* RNC_GAMEPLAY_ENTITIES_ATTACH_MANIPULATOR_TYPES_H */
