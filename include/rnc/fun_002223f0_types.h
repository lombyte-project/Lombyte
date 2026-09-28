#ifndef RNC_FUN_002223F0_TYPES_H
#define RNC_FUN_002223F0_TYPES_H

#include "types.h"

struct M2c_D_0013C940 {
    u8 pad_0[0x1A0];
    s32 unk1A0;
    u8 pad_1A4[0x20];
    s32 unk1C4;
};

struct M2c_D_001D5BF0 {
    u8 pad_0[0x4];
    struct M2c_D_001D5BF0_unk4 * unk4;
    s32 unk8;
    u8 pad_C[0xD0];
    s32 unkDC;
    u8 pad_E0[0x44];
    s32 unk124;
};

struct M2c_D_001D5BF0_unk4 {
    u8 pad_0[0x38];
    s32 unk38;
};

struct M2c_arg0 {
    u8 pad_0[0x34];
    s32 unk34;
    u8 pad_38[0x4];
    s32 unk3C;
    s32 unk40;
    u8 pad_44[0xC];
    u32 unk50;
    s32 unk54;
};

#endif /* RNC_FUN_002223F0_TYPES_H */
