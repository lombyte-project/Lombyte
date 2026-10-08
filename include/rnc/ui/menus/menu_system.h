#ifndef LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H
#define LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H

#include "types.h"

struct ModeEntry;
struct ModeTarget;

/*
 * State of the front-end menu system at D_001D5BF0, as far as the code in
 * src/ reads it. Fields are named once their use is known. Size 0x130 (only the fields used so far).
 */
struct MenuSystem {
    s32 state;                /* 0x0: menu state; loaded from the page (+0x3C) on a switch, 0x2D at init */
    struct ModeEntry *current; /* 0x4: active page; passed to mode_freeze_init */
    s32 next;                 /* 0x8: requested page (back = current->unk38); becomes current after the switch (FUN_002192a8) */
    s32 close_request;        /* 0xC: nonzero makes FUN_002192a8 call FUN_002191b8 (release the page, state 20) */
    s32 unk10;
    s32 timer;                /* 0x14: 12 on a page switch, 2 on close; counted down in states 1 and 20 */
    s32 saved_texture_start;  /* 0x18: restored to gs_texture_allocation_start on close (FUN_002191b8) */
    u8 pad_1C[0xAF];
    u8 unkCB;
    u8 pad_CC[0x4];
    struct ModeTarget *previous; /* 0xD0: the page left by the last switch (FUN_002192a8) */
    s32 unkD4;
    u8 pad_D8[0x4];
    s32 unkDC;
    s32 unkE0;
    s32 unkE4;
    u8 pad_E8[0x8];
    s32 unkF0;
    s32 unkF4;
    u8 pad_F8[0x14];
    s32 unk10C;
    s32 update_count;         /* 0x110: incremented on every FUN_002192a8 update */
    u8 pad_114[0x10];
    s32 unk124;
    s32 save_pending;         /* 0x128: set after prepare_save_game, cleared once the memory card is idle (saving_data_menu) */
    s32 message_id;           /* 0x12C: get_help_message_text id (0x4FB5 while saving) */
};

/* gcc 2.95 has no _Static_assert: a negative array size fails the build. */
#define MENU_SYSTEM_OFFSET_CHECK(field, off) \
    typedef char menu_system_offset_check_##field[ \
        ((unsigned long)&((struct MenuSystem *)0)->field == (off)) ? 1 : -1]
MENU_SYSTEM_OFFSET_CHECK(current, 0x4);
MENU_SYSTEM_OFFSET_CHECK(next, 0x8);
MENU_SYSTEM_OFFSET_CHECK(close_request, 0xC);
MENU_SYSTEM_OFFSET_CHECK(timer, 0x14);
MENU_SYSTEM_OFFSET_CHECK(saved_texture_start, 0x18);
MENU_SYSTEM_OFFSET_CHECK(unkCB, 0xCB);
MENU_SYSTEM_OFFSET_CHECK(previous, 0xD0);
MENU_SYSTEM_OFFSET_CHECK(update_count, 0x110);
MENU_SYSTEM_OFFSET_CHECK(save_pending, 0x128);
MENU_SYSTEM_OFFSET_CHECK(message_id, 0x12C);
#undef MENU_SYSTEM_OFFSET_CHECK

extern struct MenuSystem menu_system __asm__("D_001D5BF0");

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H */
