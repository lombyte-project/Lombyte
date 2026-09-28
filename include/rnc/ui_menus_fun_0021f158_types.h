#ifndef RNC_UI_MENUS_FUN_0021F158_TYPES_H
#define RNC_UI_MENUS_FUN_0021F158_TYPES_H

#include "types.h"

struct M2c_D_001D5BF4 {
    u8 pad_0[0x40];
    struct M2c_temp_3_15 * unk40;
};

struct M2c_temp_16_37 {
    u8 pad_0[0x8];
    u16 unk8;
    u8 pad_A[0x4];
    s32 unkE;
};

struct M2c_temp_3_15 {
    u8 pad_0[0x3C];
    s32 unk3C;
    u8 pad_40[0x8];
    s32 unk48;
};

#endif /* RNC_UI_MENUS_FUN_0021F158_TYPES_H */
