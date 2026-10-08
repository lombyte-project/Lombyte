#ifndef LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H
#define LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H

#include "types.h"

struct ModeEntry;
struct ModeTarget;

/*
 * State of the front-end menu system at D_001D5BF0, as far as the code in
 * src/ reads it. Fields are named once their use is known; 0x4 is the
 * active menu mode. Size 0x130 (only the fields used so far).
 */
struct MenuSystem {
    s32 unk0;
    struct ModeEntry *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad_14[0xB7];
    u8 unkCB;
    u8 pad_CC[0x4];
    struct ModeTarget *unkD0;
    s32 unkD4;
    u8 pad_D8[0x8];
    s32 unkE0;
    s32 unkE4;
    u8 pad_E8[0x8];
    s32 unkF0;
    s32 unkF4;
    u8 pad_F8[0x14];
    s32 unk10C;
    s32 unk110;
    u8 pad_114[0x10];
    s32 unk124;
    s32 unk128;
    s32 unk12C;
};

extern struct MenuSystem menu_system __asm__("D_001D5BF0");

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H */
