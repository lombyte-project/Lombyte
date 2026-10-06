#ifndef LOMBYTE_RNC_UI_MENUS_FUN_0021F158_H
#define LOMBYTE_RNC_UI_MENUS_FUN_0021F158_H

#include "types.h"

struct ModeRef {
    u8 pad_0[0x40];
    struct MenuItem *unk40;
};

struct MenuLabel {
    u8 pad_0[0x8];
    u16 unk8;
    u8 pad_A[0x4];
    s32 unkE;
};

struct MenuItem {
    u8 pad_0[0x3C];
    s32 unk3C;
    u8 pad_40[0x8];
    s32 unk48;
};

#endif /* LOMBYTE_RNC_UI_MENUS_FUN_0021F158_H */
