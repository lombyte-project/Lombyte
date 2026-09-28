#ifndef RNC_FUN_00219E10_TYPES_H
#define RNC_FUN_00219E10_TYPES_H

#include "types.h"

struct M2c_D_0013C940 {
    u8 pad_0[0x1C4];
    s32 unk1C4;
};

struct M2c_D_001D5BF0 {
    u8 pad_0[0x4];
    struct M2c_D_001D5BF0_unk4 * unk4;
    s32 unk8;
    u8 pad_C[0x118];
    s32 unk124;
};

struct M2c_D_001D5BF0_unk4 {
    u8 pad_0[0x38];
    s32 unk38;
};

struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0xC];
    s32 unk24;
    u8 pad_28[0x14];
    s32 unk3C;
    s32 unk40;
    u8 pad_44[0x18];
    s32 unk5C;
    s32 unk60;
};

#endif /* RNC_FUN_00219E10_TYPES_H */
