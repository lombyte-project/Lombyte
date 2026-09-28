#ifndef RNC_UI_MENUS_FUN_0021DDF8_TYPES_H
#define RNC_UI_MENUS_FUN_0021DDF8_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x44];
    struct M2c_temp_5_52 * unk44;
    u8 pad_48[0x18];
    struct M2c_arg0_unk60 * unk60;
};

struct M2c_arg0_unk60 {
    u8 pad_0[0xA6];
    s16 unkA6;
    u8 pad_A8[0x2];
};

struct M2c_temp_2_35 {
    u8 pad_0[0x52];
    u8 unk52;
    s32 unk53;
};

struct M2c_temp_5_52 {
    u8 pad_0[0x50];
    s32 unk50;
};

#endif /* RNC_UI_MENUS_FUN_0021DDF8_TYPES_H */
