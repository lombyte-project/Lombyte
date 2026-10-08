#ifndef LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H
#define LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H

#include "types.h"

struct MenuScreen;

/*
 * One page of the front-end menu: up to 14 screens, each paired with one of
 * the 14 menu mobys (D_001D5D90). Size 0x88 (only the fields used so far).
 *
 * FUN_002192a8 switches pages: it plays moby_anims[i] on D_001D5D90[i], copies
 * state into menu_system.state, runs the enter/leave/update handlers of
 * screens[], and moves pending_focus into focus on the next update.
 * confirmed is the answer of the confirm page: FUN_00221af0 sets 1 for pad
 * bit 0x20 and 0 for 0x10, FUN_00221a48 clears it on enter; the save screens
 * start saving once menu_system.previous->confirmed != 0.
 */
struct MenuPage {
    s32 moby_anims[14];       /* 0x00: animation per menu moby */
    struct MenuPage *back;    /* 0x38: page for the back button (pad 0x10); 0 = none */
    s32 state;                /* 0x3C: menu_system.state while this page is current */
    struct MenuScreen *focus; /* 0x40: screen with input focus */
    struct MenuScreen *screens[14]; /* 0x44 */
    u8 pad_7C[0x4];
    struct MenuScreen *pending_focus; /* 0x80: becomes focus on the next update */
    s32 confirmed;            /* 0x84: confirm page answer, 1 = yes */
}; /* size 0x88 */

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
 * members of one object. D_001D5D90 (menu mobys) and D_001D5DD0
 * (manipulators) are never reached from this base and stay separate.
 *
 * Evidence for the less obvious fields:
 * - close_request: nonzero makes FUN_002192a8 call FUN_002191b8 (release the
 *   page, state 20); 1 from a screen update, 3 with unkE4..unkF4 set
 *   (fun_00221968, draw_map_screen), 3/4/5 from fun_0021abf8.
 * - equipped: D_001863D8[id] +0 names the slot; draw_menu_item_grid draws the
 *   held item with frame +1; load_hand_gadget builds the preview mobys from
 *   slots 0..3; fun_0021b858 never clears slots 0 and 3.
 * - preview stream (0xA0-0xCF): buffers from select_next_stream_buffer(1) at
 *   preview init; fun_00225e70 reads animations at or above
 *   streamed_animation_base from the stream archive (id - base);
 *   pending_buffer is cleared by complete_stream_buffer_transfer and
 *   clear_preview_animation_queue.
 * - unkD4: 0 from saving_data_menu, 1/2 from saving_data_menu2 (flags
 *   0x2000); draw_prompt_box shows text 0x4FB3 for 0..2, 0x4FB5 for 3.
 * - unkD8: fun_002196b8 draws panel slot 6 only while set; pause_all_sounds
 *   sets it and picks page tables D_001CE798 by it.
 * - resource_table_toggle: select_world_object_resource_tables(class,
 *   toggle == 0), then stored negated (load_hand_gadget, fun_0021e230);
 *   load_hand_gadget builds the preview moby once unk120 equals the
 *   selected class.
 * - unk134 / unk138: while set, pages with flags 8 / 4 are skipped
 *   (find_next_pause_page) and grid items with those flags draw frame +2.
 * - The buffer addresses (unk10, unkFC..unk10C) are carved from
 *   D_001940C0[1] / [2] by FUN_00218f98.
 */
struct MenuSystem {
    /* Page switching (FUN_002192a8) */
    s32 state;                /* 0x000: menu state, from page->state; 0x2D at init */
    struct MenuPage *current; /* 0x004: active page */
    struct MenuPage *next;    /* 0x008: requested page */
    s32 close_request;        /* 0x00C: nonzero closes the menu */
    s32 unk10;                /* 0x010: buffer address D_001940C0[2] + 0xA0000 */
    s32 timer;                /* 0x014: 12 on a switch, 2 on close; counts down */
    s32 saved_texture_start;  /* 0x018: gs_texture_allocation_start to restore on close */
    s32 current_gadget;       /* 0x01C: gadget of the queued preview animation, -1 none */
    u8 pad_20[0x10];
    s32 equipped[4];          /* 0x030: gadget id held by each slot */
    u8 pad_40[0x60];

    /* Preview animation stream */
    s32 stream_buffer[2];     /* 0x0A0: double buffer */
    s32 unkA8;                /* 0x0A8: fun_00226848 first-resource argument */
    s32 unkAC;                /* 0x0AC: fun_00226848 count argument */
    s32 unkB0[3];             /* 0x0B0: buffer addresses read by fun_00226848 */
    s32 read_offset;          /* 0x0BC: offset of the data in the read buffer */
    s32 unkC0;                /* 0x0C0: -1 at preview init */
    u8 pad_C4[0x4];
    u8 loaded_animation[2];   /* 0x0C8: animation held by each buffer, 0xFF none */
    u8 read_buffer_index;     /* 0x0CA: buffer being read */
    u8 pending_buffer;        /* 0x0CB: buffer index + 1 while a read is in flight */
    u32 streamed_animation_base; /* 0x0CC: first animation id read from the stream archive */

    struct MenuPage *previous; /* 0x0D0: page left by the last switch */
    s32 unkD4;                /* 0x0D4: 0 saving_data_menu, 1/2 saving_data_menu2; picks draw_prompt_box text */
    s32 unkD8;                /* 0x0D8: fun_002196b8 draws slot 6 only while set */
    s32 unkDC;                /* 0x0DC: pause_all_sounds: (mode == 0x23) */
    s32 unkE0;                /* 0x0E0: memcard_make_whole_save / memcard_restore_game argument */
    s32 unkE4;                /* 0x0E4: written with unkF0/unkF4 before close_request 3 */
    u8 pad_E8[0x4];
    s32 unkEC;                /* 0x0EC: menu_action_messages[] entry (fun_0021abf8) */
    struct MenuPage *unkF0;   /* 0x0F0: menu_system.current, or D_001CF418 (draw_map_screen) */
    s32 unkF4;                /* 0x0F4: 0, 2, 0xB or 0xF */
    s32 unkF8;                /* 0x0F8: read by pause_all_sounds */

    /* Buffers carved at init (FUN_00218f98) */
    s32 unkFC;                /* 0x0FC: from D_001940C0[1] */
    s32 unk100;               /* 0x100: from D_001940C0[2] */
    s32 unk104;               /* 0x104: D_001940C0[1] + 0xA0000 */
    s32 help_text_buffer;     /* 0x108: help text read target, D_001940C0[1] + 0xDC000 */
    s32 unk10C;               /* 0x10C: D_001940C0[2] + 0x180000 */

    s32 update_count;         /* 0x110: FUN_002192a8 updates */
    u8 pad_114[0x4];

    /* Preview moby resources */
    s32 resource_table_toggle;    /* 0x118: alternates per resource table select */
    s32 unk11C;                   /* 0x11C: an oclass, -1 none */
    s32 unk120;                   /* 0x120: an oclass, -1 none */

    s32 close_locked;         /* 0x124: nonzero blocks start/back closing the menu */
    s32 card_op_pending;      /* 0x128: card save/load started, waiting for the card */
    s32 card_op_text;         /* 0x12C: text shown while pending (0x4FB5 save, 0x4FB6 load) */
    u8 pad_130[0x4];
    s32 unk134;               /* 0x134: hides pages / items with flags 8 */
    s32 unk138;               /* 0x138: hides pages / items with flags 4 */
    s32 unk13C;               /* 0x13C: written by FUN_00218d78 */
    s32 unk140;                        /* 0x140: oclass at the last resource table select */
    s32 last_resource_table_toggle;    /* 0x144: resource_table_toggle at the last select */
}; /* size 0x148 */

extern struct MenuSystem menu_system __asm__("D_001D5BF0");

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_SYSTEM_H */
