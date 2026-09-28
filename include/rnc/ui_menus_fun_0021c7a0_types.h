#ifndef RNC_UI_MENUS_FUN_0021C7A0_TYPES_H
#define RNC_UI_MENUS_FUN_0021C7A0_TYPES_H

#include "types.h"

struct M2c_D_0013C940 {
    u8 pad_0[0x1C4];
    s32 unk1C4;
};

struct M2c_D_00141848 {
    u8 pad_0[0xA8];
    u16 unkA8;
    u16 unkAA;
    s32 unkAC;
};

struct M2c_D_001D5BF4 {
    u8 pad_0[0x40];
    struct M2c_temp_3_16 * unk40;
};

struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x18];
    s32 unk30;
    u8 pad_34[0x1C];
    s32 unk50;
};

struct M2c_temp_3_16 {
    u8 pad_0[0x3C];
    s32 unk3C;
    u8 pad_40[0x8];
    s32 unk48;
};

#endif /* RNC_UI_MENUS_FUN_0021C7A0_TYPES_H */
