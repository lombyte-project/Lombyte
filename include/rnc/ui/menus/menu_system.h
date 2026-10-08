#ifndef LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H
#define LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H

#include "types.h"

struct MenuScreen;

/*
 * One page of the front-end menu: up to 14 screens, each paired with one of
 * the 14 menu mobys (D_001D5D90). Size 0x88 (only the fields used so far).
 */
struct MenuPage {
    s32 moby_anims[14];       /* 0x0: animation per menu moby, set_moby_animation(D_001D5D90[i], moby_anims[i], ...) on a page switch (FUN_002192a8) */
    struct MenuPage *back;    /* 0x38: page the back button (pad bit 0x10) switches to; 0 = none */
    s32 state;                /* 0x3C: copied into menu_system.state when the page becomes current (FUN_002192a8) */
    struct MenuScreen *focus; /* 0x40: screen with input focus; its leave/enter handlers run when pending_focus replaces it */
    struct MenuScreen *screens[14]; /* 0x44: enter/leave/update handlers called for each on a page switch (FUN_002192a8) */
    u8 pad_7C[0x4];
    struct MenuScreen *pending_focus; /* 0x80: becomes focus on the next update, then cleared (FUN_002192a8) */
    s32 confirmed;            /* 0x84: confirm page answer: 1 when left with pad bit 0x20, 0 with 0x10 (FUN_00221af0), cleared on enter (FUN_00221a48); the save screens start saving once previous->confirmed != 0 */
};

#define MENU_PAGE_OFFSET_CHECK(field, off) \
    typedef char menu_page_offset_check_##field[ \
        ((unsigned long)&((struct MenuPage *)0)->field == (off)) ? 1 : -1]
MENU_PAGE_OFFSET_CHECK(back, 0x38);
MENU_PAGE_OFFSET_CHECK(state, 0x3C);
MENU_PAGE_OFFSET_CHECK(focus, 0x40);
MENU_PAGE_OFFSET_CHECK(screens, 0x44);
MENU_PAGE_OFFSET_CHECK(pending_focus, 0x80);
MENU_PAGE_OFFSET_CHECK(confirmed, 0x84);
#undef MENU_PAGE_OFFSET_CHECK

/*
 * State of the front-end menu system at D_001D5BF0, as far as the code in
 * src/ reads it. Fields are named once their use is known. Size 0x13C (only the fields used so far).
 */
struct MenuSystem {
    s32 state;                /* 0x0: menu state; loaded from the page (+0x3C) on a switch, 0x2D at init */
    struct MenuPage *current;  /* 0x4: active page; passed to mode_freeze_init */
    struct MenuPage *next;    /* 0x8: requested page (back = current->back); becomes current after the switch (FUN_002192a8) */
    s32 close_request;        /* 0xC: nonzero makes FUN_002192a8 call FUN_002191b8 (release the page, state 20) */
    s32 unk10;                /* 0x10: buffer address D_001940C0[2] + 0xA0000, set at init (FUN_00218f98) */
    s32 timer;                /* 0x14: 12 on a page switch, 2 on close; counted down in states 1 and 20 */
    s32 saved_texture_start;  /* 0x18: restored to gs_texture_allocation_start on close (FUN_002191b8) */
    u8 pad_1C[0x14];
    s32 equipped[4];          /* 0x30: gadget id held by each slot (D_001863D8[id] +0 names the slot); draw_menu_item_grid draws the item that its slot holds with frame +1; load_hand_gadget reads 0x30-0x3C as its selected/attachment/animation/pose gadget */
    u8 pad_40[0x8B];
    u8 unkCB;
    u8 pad_CC[0x4];
    struct MenuPage *previous; /* 0xD0: the page left by the last switch (FUN_002192a8) */
    s32 confirm_kind;         /* 0xD4: who opened the confirm page: 0 = saving_data_menu (overwrite), 1/2 = saving_data_menu2 (flags bit 0x2000); draw_prompt_box shows text 0x4FB3 for 0..2, 0x4FB5 for 3 */
    u8 pad_D8[0x4];
    s32 unkDC;
    s32 unkE0;
    s32 unkE4;
    u8 pad_E8[0x8];
    s32 unkF0;
    s32 unkF4;
    u8 pad_F8[0x4];
    s32 unkFC;                /* 0xFC: buffer address carved from D_001940C0[1] at init (FUN_00218f98) */
    s32 unk100;               /* 0x100: buffer address carved from D_001940C0[2] at init (FUN_00218f98) */
    s32 unk104;               /* 0x104: buffer address D_001940C0[1] + 0xA0000, set at init (FUN_00218f98) */
    s32 help_text_buffer;     /* 0x108: start_audio_stream_read destination for disc_table.help_text (update_menu_help_text_load, FUN_0021d1f8); D_001940C0[1] + 0xDC000 at init */
    s32 unk10C;               /* 0x10C: buffer address D_001940C0[2] + 0x180000, set at init (FUN_00218f98) */
    s32 update_count;         /* 0x110: incremented on every FUN_002192a8 update */
    u8 pad_114[0x10];
    s32 close_blocked;        /* 0x124: nonzero stops screens from closing the menu (confirm pad 0xD00 returns 1, back without a back page returns -1); 0 at init (FUN_00218f98) */
    s32 save_pending;         /* 0x128: set after prepare_save_game, cleared once the memory card is idle (saving_data_menu) */
    s32 message_id;           /* 0x12C: get_help_message_text id (0x4FB5 while saving) */
    s32 unk130;
    s32 unk134;               /* 0x134: while set, pages with flags bit 8 are skipped (find_next_pause_page) and grid items with flags 8 draw frame +2 */
    s32 unk138;               /* 0x138: while set, pages with flags bit 4 are skipped (find_next_pause_page) and grid items with flags 4 draw frame +2 */
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
MENU_SYSTEM_OFFSET_CHECK(equipped, 0x30);
MENU_SYSTEM_OFFSET_CHECK(unkCB, 0xCB);
MENU_SYSTEM_OFFSET_CHECK(previous, 0xD0);
MENU_SYSTEM_OFFSET_CHECK(confirm_kind, 0xD4);
MENU_SYSTEM_OFFSET_CHECK(unkFC, 0xFC);
MENU_SYSTEM_OFFSET_CHECK(help_text_buffer, 0x108);
MENU_SYSTEM_OFFSET_CHECK(update_count, 0x110);
MENU_SYSTEM_OFFSET_CHECK(close_blocked, 0x124);
MENU_SYSTEM_OFFSET_CHECK(save_pending, 0x128);
MENU_SYSTEM_OFFSET_CHECK(message_id, 0x12C);
MENU_SYSTEM_OFFSET_CHECK(unk138, 0x138);
#undef MENU_SYSTEM_OFFSET_CHECK

extern struct MenuSystem menu_system __asm__("D_001D5BF0");

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H */
