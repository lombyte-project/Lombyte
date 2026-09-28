#ifndef RNC_UI_MENUS_FUN_002220F0_TYPES_H
#define RNC_UI_MENUS_FUN_002220F0_TYPES_H

#include "types.h"

struct M2c_D_001D5BF4 {
    u8 pad_0[0x40];
    struct M2c_temp_3_18 * unk40;
};

struct M2c_arg0 {
    u8 pad_0[0x20];
    s32 unk20;
    s32 unk24;
};

struct M2c_temp_3_18 {
    u8 pad_0[0x34];
    s32 unk34;
    u8 pad_38[0x8];
    s32 unk40;
};

#endif /* RNC_UI_MENUS_FUN_002220F0_TYPES_H */
