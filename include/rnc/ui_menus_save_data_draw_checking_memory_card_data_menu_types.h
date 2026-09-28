#ifndef RNC_UI_MENUS_SAVE_DATA_DRAW_CHECKING_MEMORY_CARD_DATA_MENU_TYPES_H
#define RNC_UI_MENUS_SAVE_DATA_DRAW_CHECKING_MEMORY_CARD_DATA_MENU_TYPES_H

#include "types.h"

struct M2c_D_0013D290 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0xC8];
    s32 unkD4;
    u8 pad_D8[0x4];
    s32 unkDC;
};

struct M2c_D_00151780 {
    u8 pad_0[0x160];
    s16 unk160;
    s16 unk162;
    u8 pad_164[0x2];
};

struct M2c_D_001D5BF0 {
    u8 pad_0[0x128];
    s32 unk128;
    s32 unk12C;
};

struct M2c_arg0 {
    u8 pad_0[0x20];
    s32 unk20;
    s32 unk24;
    u8 pad_28[0xC];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    u8 pad_40[0x4];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
};

struct M2c_var_5_0 {
    u8 pad_0[0x4];
    s16 unk4;
    s16 unk6;
    u8 pad_8[0x2];
};

struct M2c_var_6_0 {
    u8 pad_0[0x40];
    struct M2c_var_6_0_unk40 * unk40;
};

struct M2c_var_6_0_unk40 {
    u8 pad_0[0x48];
    s32 unk48;
};

#endif /* RNC_UI_MENUS_SAVE_DATA_DRAW_CHECKING_MEMORY_CARD_DATA_MENU_TYPES_H */
