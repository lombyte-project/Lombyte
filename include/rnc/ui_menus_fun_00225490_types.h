#ifndef RNC_UI_MENUS_FUN_00225490_TYPES_H
#define RNC_UI_MENUS_FUN_00225490_TYPES_H

#include "types.h"

struct M2c_temp_2_16 {
    u8 pad_0[0x20];
    s32 unk20;
    struct M2c_temp_2_16_unk24 * unk24;
    u8 pad_28[0x8];
    u8 unk30;
    u8 unk31;
    s32 unk32;
    u8 pad_36[0x3D];
    s32 unk73;
};

struct M2c_temp_2_16_unk24 {
    u8 pad_0[0x6];
    s32 unk6;
};

#endif /* RNC_UI_MENUS_FUN_00225490_TYPES_H */
