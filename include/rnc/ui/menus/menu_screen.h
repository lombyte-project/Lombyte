#ifndef LOMBYTE_RNC_UI_MENUS_MENU_SCREEN_H
#define LOMBYTE_RNC_UI_MENUS_MENU_SCREEN_H

#include "types.h"

/*
 * One screen of a menu page (struct MenuPage.screens), passed to its own
 * handlers (menu_system.current->focus is the one with input focus). 0x10-0x27
 * are shared by every screen; from 0x30 on each screen keeps its own data
 * (stream buffers from select_next_stream_buffer, the selected save slot,
 * list cursors), so those words keep offset names.
 */
struct MenuScreen {
    s32 (*update)(struct MenuScreen *);              /* 0x0: called every frame; nonzero sets menu_system.close_request (FUN_002192a8) */
    s32 unk4;
    void (*enter)(struct MenuScreen *, s32);         /* 0x8: called when its page becomes current (arg 0) or it gains focus (arg 1) */
    void (*leave)(struct MenuScreen *, s32);         /* 0xC: called when its page is left (arg 0) or it loses focus (arg 1); FUN_002191b8 */
    s32 unk10;                /* 0x10: bit 2 set by FUN_0021d1f8 */
    s32 moby;                 /* 0x14: its menu moby D_001D5D90[i], set on a page switch (FUN_002192a8); target of allocate_voice_for_target_entry for menu sounds */
    s32 x;                    /* 0x18: left edge (draw_level_selection_map(x, x + width, ...)) */
    s32 y;                    /* 0x1C: top edge (draw_level_selection_map(..., y, y + height)) */
    s32 width;                /* 0x20: text centred at width / 2 (FUN_00222d98) */
    s32 height;               /* 0x24: text centred at height / 2 (FUN_00222d98) */
    u8 pad_28[0x8];
    s32 unk30;                /* 0x30: bit 0 picks the pad repeat mask (saving_data_menu) */
    s32 unk34;                /* 0x34: bit 0x200 passed to select_next_stream_buffer (FUN_0021fce0) */
    s32 unk38;
    s32 unk3C;                /* 0x3C: stream buffer (FUN_00225660) or icon cursor (FUN_00219fa0) */
    s32 unk40;                /* 0x40: save slot (saving_data_menu), list index (FUN_00221d68), icon count (FUN_00219fa0) */
    s32 unk44;
    s32 unk48;                /* 0x48: stream buffer (FUN_00222f18/FUN_0021fce0) or icon list (FUN_00219fa0) */
    s32 unk4C;                /* 0x4C: stream buffer (FUN_0021fce0) or save step (saving_data_menu) */
    s32 unk50;
    s32 unk54;                /* 0x54: stream buffer (FUN_00221a48) */
    u8 pad_58[0x4];
    s32 unk5C;                /* 0x5C: first icon y (FUN_00219fa0) */
};

/* gcc 2.95 has no _Static_assert: a negative array size fails the build. */
#define MENU_SCREEN_OFFSET_CHECK(field, off) \
    typedef char menu_screen_offset_check_##field[ \
        ((unsigned long)&((struct MenuScreen *)0)->field == (off)) ? 1 : -1]
MENU_SCREEN_OFFSET_CHECK(update, 0x0);
MENU_SCREEN_OFFSET_CHECK(enter, 0x8);
MENU_SCREEN_OFFSET_CHECK(leave, 0xC);
MENU_SCREEN_OFFSET_CHECK(moby, 0x14);
MENU_SCREEN_OFFSET_CHECK(x, 0x18);
MENU_SCREEN_OFFSET_CHECK(y, 0x1C);
MENU_SCREEN_OFFSET_CHECK(width, 0x20);
MENU_SCREEN_OFFSET_CHECK(height, 0x24);
MENU_SCREEN_OFFSET_CHECK(unk5C, 0x5C);
#undef MENU_SCREEN_OFFSET_CHECK

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_SCREEN_H */
