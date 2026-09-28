#ifndef RNC_UI_MENUS_SAVE_DATA_LOADING_DATA_MENU_TYPES_H
#define RNC_UI_MENUS_SAVE_DATA_LOADING_DATA_MENU_TYPES_H

#include "types.h"

struct M2c_D_0013C940 {
    u8 pad_0[0x1B4];
    s32 unk1B4;
    u8 pad_1B8[0xC];
    s32 unk1C4;
};

struct M2c_D_0013D290 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x8];
    s32 unk14;
    u8 pad_18[0xA8];
    s32 unkC0;
    u8 pad_C4[0x10];
    s32 unkD4;
    u8 pad_D8[0x4];
    s32 unkDC;
    s32 unkE0;
    s32 unkE4;
    u8 pad_E8[0xC];
    s32 unkF4;
};

struct M2c_D_0013E550 {
    u8 pad_0[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
};

struct M2c_D_001D5BF0 {
    u8 pad_0[0x4];
    struct M2c_D_001D5BF0_unk4 * unk4;
    s32 unk8;
    u8 pad_C[0x118];
    s32 unk124;
    s32 unk128;
    s32 unk12C;
};

struct M2c_D_001D5BF0_unk4 {
    u8 pad_0[0x38];
    s32 unk38;
};

struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x18];
    s32 unk30;
    u8 pad_34[0xC];
    s32 unk40;
};

#endif /* RNC_UI_MENUS_SAVE_DATA_LOADING_DATA_MENU_TYPES_H */
