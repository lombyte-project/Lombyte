#ifndef RNC_UI_MENUS_FUN_00222D98_TYPES_H
#define RNC_UI_MENUS_FUN_00222D98_TYPES_H

#include "types.h"

struct M2c_D_0013D290 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0xC8];
    s32 unkD4;
    u8 pad_D8[0x4];
    s32 unkDC;
};

struct M2c_D_001D5BF4 {
    u8 pad_0[0x40];
    struct M2c_D_001D5BF4_unk40 * unk40;
};

struct M2c_D_001D5BF4_unk40 {
    u8 pad_0[0x40];
    s32 unk40;
};

struct M2c_arg0 {
    u8 pad_0[0x20];
    s32 unk20;
    s32 unk24;
};

#endif /* RNC_UI_MENUS_FUN_00222D98_TYPES_H */
