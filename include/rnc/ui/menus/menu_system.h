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
 * State of the front-end menu system at D_001D5BF0. Size 0x148.
 *
 * One struct, not adjacent globals: retail code materialises the base
 * 0x1D5BF0 (lui/addiu %lo(D_001D5BF0)) and then reaches 0x4..0x144 through
 * that one register in the same function: FUN_002192a8 (0x4..0x110),
 * FUN_00218f98 init (0x4..0x124), fun_0021e230 (0x4, 0x118..0x120,
 * 0x140, 0x144), load_hand_gadget/FUN_00224368 (0x1C..0x3C, 0xCC,
 * 0x118..0x144), fun_002240c8 (0x1C, 0xA0..0xCA, 0x11C..0x120),
 * fun_00225e70 (0x30..0x38, 0xBC..0xCC), FUN_00218d78 (0xC..0x13C).
 * gcc never folds two separate globals onto one base register, so these are
 * members of one object. The other D_001D5Cxx/D_001D5Dxx labels in the
 * listings (D_001D5BF4, D_001D5CBB, D_001D5CF8, D_001D5D14, ...) are only
 * direct %hi/%lo(base + member) accesses. D_001D5D90 (menu mobys) and
 * D_001D5DD0 (manipulators) are never reached from this base, so they stay
 * separate globals.
 */
struct MenuSystem {
    s32 state;                /* 0x0: menu state; loaded from the page (+0x3C) on a switch, 0x2D at init */
    struct MenuPage *current;  /* 0x4: active page; passed to mode_freeze_init */
    struct MenuPage *next;    /* 0x8: requested page (back = current->back); becomes current after the switch (FUN_002192a8) */
    s32 close_request;        /* 0xC: nonzero makes FUN_002192a8 call FUN_002191b8 (release the page, state 20); 1 from a screen update, 3 with action_* set (fun_00221968, draw_map_screen), 3/4/5 from fun_0021abf8 */
    s32 unk10;                /* 0x10: buffer address D_001940C0[2] + 0xA0000, set at init (FUN_00218f98) */
    s32 timer;                /* 0x14: 12 on a page switch, 2 on close; counted down in states 1 and 20 */
    s32 saved_texture_start;  /* 0x18: restored to gs_texture_allocation_start on close (FUN_002191b8) */
    s32 current_gadget;       /* 0x1C: gadget whose preview animation is queued; load_hand_gadget queues gadget_animations[id] when the loaded gadget differs; -1 at preview init */
    u8 pad_20[0x10];
    s32 equipped[4];          /* 0x30: gadget id held by each slot (D_001863D8[id] +0 names the slot); draw_menu_item_grid draws the item that its slot holds with frame +1; load_hand_gadget builds the class-pose/attachment/animation/pose preview mobys from slots 0..3; fun_0021b858 never clears slots 0 and 3 */
    u8 pad_40[0x60];
    s32 stream_buffer[2];     /* 0xA0: preview animation stream buffers, select_next_stream_buffer(1) at preview init */
    s32 resource_first;       /* 0xA8: first preview resource of the queued animation (fun_00226848, clear_preview_resource_bindings) */
    s32 resource_count;       /* 0xAC: number of preview resources; 0 after clear_preview_resource_bindings */
    s32 resource_buffer_address[3]; /* 0xB0: per-resource buffer addresses (fun_00226848) */
    s32 read_offset;          /* 0xBC: stream read offset (fun_00225e70) */
    s32 unkC0;                /* 0xC0: -1 at preview init */
    u8 pad_C4[0x4];
    u8 loaded_animation[2];   /* 0xC8: animation held by each stream buffer, 0xFF at preview init */
    u8 read_buffer_index;     /* 0xCA: stream buffer being read, 0 at preview init */
    u8 pending_buffer;        /* 0xCB: set while a stream read is in flight; cleared by complete_stream_buffer_transfer / clear_preview_animation_queue */
    u32 streamed_animation_base; /* 0xCC: animation ids at or above it are read from the stream archive at id - base (fun_00225e70); load_hand_gadget adds it to gadget_animations[id] ids before queue_preview_animation */
    struct MenuPage *previous; /* 0xD0: the page left by the last switch (FUN_002192a8) */
    s32 confirm_kind;         /* 0xD4: who opened the confirm page: 0 = saving_data_menu (overwrite), 1/2 = saving_data_menu2 (flags bit 0x2000); draw_prompt_box shows text 0x4FB3 for 0..2, 0x4FB5 for 3 */
    s32 special_slot_enabled; /* 0xD8: panel slot 6 is drawn only while set (fun_002196b8); pause_all_sounds sets it from D_0015EEA0/D_0015EE20/unkF8 and picks page tables D_001CE798 by it */
    s32 unkDC;                /* 0xDC: pause_all_sounds sets it to (mode == 0x23) */
    s32 unkE0;                /* 0xE0: memcard_make_whole_save / memcard_restore_game argument (fun_00226b08, process_global_state_flags) */
    s32 action_value;         /* 0xE4: value for the close action: menu item param (fun_0021abf8), D_001A0314[0] (fun_00221968), D_001A00F0.level (draw_map_screen) */
    u8 pad_E8[0x4];
    s32 action_message;       /* 0xEC: menu_action_messages[i] picked when an action closes the menu (fun_0021abf8) */
    struct MenuPage *return_page; /* 0xF0: page current when the close action was requested (fun_0021abf8, fun_00221968; D_001CF418 from draw_map_screen) */
    s32 action_mode;          /* 0xF4: kind of close action: 0/2 (fun_0021abf8), 0xB (draw_map_screen), 0xF (fun_00221968) */
    s32 unkF8;
    s32 unkFC;                /* 0xFC: buffer address carved from D_001940C0[1] at init (FUN_00218f98) */
    s32 unk100;               /* 0x100: buffer address carved from D_001940C0[2] at init (FUN_00218f98) */
    s32 unk104;               /* 0x104: buffer address D_001940C0[1] + 0xA0000, set at init (FUN_00218f98) */
    s32 help_text_buffer;     /* 0x108: start_audio_stream_read destination for disc_table.help_text (update_menu_help_text_load, FUN_0021d1f8); D_001940C0[1] + 0xDC000 at init */
    s32 unk10C;               /* 0x10C: buffer address D_001940C0[2] + 0x180000, set at init (FUN_00218f98) */
    s32 update_count;         /* 0x110: incremented on every FUN_002192a8 update */
    u8 pad_114[0x4];
    s32 resource_table_toggle; /* 0x118: select_world_object_resource_tables(class, toggle == 0), then stored negated (load_hand_gadget, fun_0021e230) */
    s32 active_resource_class; /* 0x11C: moby class whose resources are selected; -1 at preview init */
    s32 requested_resource_class; /* 0x120: class asked for; load_hand_gadget builds the preview moby once it equals the selected class; -1 at preview init */
    s32 close_blocked;        /* 0x124: nonzero stops screens from closing the menu (confirm pad 0xD00 returns 1, back without a back page returns -1); 0 at init (FUN_00218f98) */
    s32 save_pending;         /* 0x128: set after prepare_save_game, cleared once the memory card is idle (saving_data_menu, loading_data_menu) */
    s32 message_id;           /* 0x12C: get_help_message_text id (0x4FB5 while saving) */
    s32 unk130;
    s32 unk134;               /* 0x134: while set, pages with flags bit 8 are skipped (find_next_pause_page) and grid items with flags 8 draw frame +2 */
    s32 unk138;               /* 0x138: while set, pages with flags bit 4 are skipped (find_next_pause_page) and grid items with flags 4 draw frame +2 */
    s32 unk13C;               /* 0x13C: written by FUN_00218d78 */
    s32 last_requested_resource_class; /* 0x140: copy of active_resource_class at the last select (load_hand_gadget, fun_0021e230) */
    s32 last_resource_table_toggle; /* 0x144: copy of resource_table_toggle at the last select */
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
MENU_SYSTEM_OFFSET_CHECK(current_gadget, 0x1C);
MENU_SYSTEM_OFFSET_CHECK(equipped, 0x30);
MENU_SYSTEM_OFFSET_CHECK(stream_buffer, 0xA0);
MENU_SYSTEM_OFFSET_CHECK(resource_buffer_address, 0xB0);
MENU_SYSTEM_OFFSET_CHECK(read_offset, 0xBC);
MENU_SYSTEM_OFFSET_CHECK(loaded_animation, 0xC8);
MENU_SYSTEM_OFFSET_CHECK(pending_buffer, 0xCB);
MENU_SYSTEM_OFFSET_CHECK(streamed_animation_base, 0xCC);
MENU_SYSTEM_OFFSET_CHECK(previous, 0xD0);
MENU_SYSTEM_OFFSET_CHECK(confirm_kind, 0xD4);
MENU_SYSTEM_OFFSET_CHECK(special_slot_enabled, 0xD8);
MENU_SYSTEM_OFFSET_CHECK(action_value, 0xE4);
MENU_SYSTEM_OFFSET_CHECK(action_message, 0xEC);
MENU_SYSTEM_OFFSET_CHECK(action_mode, 0xF4);
MENU_SYSTEM_OFFSET_CHECK(unkFC, 0xFC);
MENU_SYSTEM_OFFSET_CHECK(help_text_buffer, 0x108);
MENU_SYSTEM_OFFSET_CHECK(update_count, 0x110);
MENU_SYSTEM_OFFSET_CHECK(resource_table_toggle, 0x118);
MENU_SYSTEM_OFFSET_CHECK(close_blocked, 0x124);
MENU_SYSTEM_OFFSET_CHECK(save_pending, 0x128);
MENU_SYSTEM_OFFSET_CHECK(message_id, 0x12C);
MENU_SYSTEM_OFFSET_CHECK(unk138, 0x138);
MENU_SYSTEM_OFFSET_CHECK(last_resource_table_toggle, 0x144);
#undef MENU_SYSTEM_OFFSET_CHECK
typedef char menu_system_size_check[(sizeof(struct MenuSystem) == 0x148) ? 1 : -1];

extern struct MenuSystem menu_system __asm__("D_001D5BF0");

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H */
