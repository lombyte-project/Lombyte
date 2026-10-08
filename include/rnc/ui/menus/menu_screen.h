#ifndef LOMBYTE_RNC_UI_MENUS_MENU_SCREEN_H
#define LOMBYTE_RNC_UI_MENUS_MENU_SCREEN_H

#include "types.h"

/*
 * Front-end menu screens. A MenuScreen (end of this file) is one screen of a
 * MenuPage; 0x0-0x2F are shared, and from 0x30 on each kind of screen keeps
 * its own data, read through the `data` union (one layout per kind).
 * Layout offsets are checked in menu_layout_check.c.
 */

/* MenuScreen 0x30-0x5F as plain words, for screens not understood yet. */
struct MenuScreenWords {
    u8 pad_30[0x4];
    s32 unk34;        /* 0x34: flags 0x40 / 0x80 (draw_map_screen) */
    s32 unk38;        /* 0x38: cleared by FUN_0021d1f8 */
    s32 unk3C;        /* 0x3C: stream buffer (FUN_00225660) */
    u8 pad_40[0x10];
    s32 unk50;        /* 0x50: 0, 1 or 3 (FUN_0021d1f8) */
    s32 unk54;        /* 0x54: cleared by FUN_0021d1f8 */
    u8 pad_58[0x8];
};

/* Option list screen (FUN_00220e28). */
struct MenuOption {
    void *unk0;               /* 0x0: 0 ends the list */
    u8 *flag;                 /* 0x4: toggled by confirm (pad 0x40) */
    u8 pad_8[0x8];
    u32 flags;                /* 0x10: bit 0 = fade out and toggle D_0016034C instead */
}; /* size 0x14 */

struct MenuOptionListData {
    u8 pad_30[0x4];
    struct MenuOption *list;  /* 0x34: ended by a null unk0 */
    s32 selection;            /* 0x38: pad 0x1000 up, 0x4000 down */
    s32 fade_timer;           /* 0x3C: counts down after fade_to_black, drives D_0015F43C */
};

/* Choice list screen (FUN_002212b8). */
struct MenuChoice {
    s32 unk0;                 /* 0x0: 0 ends the list */
    u8 *value;                /* 0x4: chosen option; confirm advances it */
    s32 option[4];            /* 0x8: 0 ends them early */
}; /* size 0x18 */

struct MenuChoiceListData {
    u8 pad_30[0x4];
    struct MenuChoice *list;  /* 0x34: ended by a zero unk0 */
    s32 selection;            /* 0x38: pad 0x1000 up, 0x4000 down */
};

/* Mission list screen (FUN_0021c4c0). */
struct MenuMissionListData {
    s32 choice[19];           /* 0x30: selected mission per level; -1 while unfocused */
    s32 count;                /* 0x7C: collect_mission_ids result */
};

/* Item grid screen (draw_menu_item_grid). Cell frames: +1 equipped,
   +2 locked, +4 D_0013E520. Locked: flags 4 / 8 with menu_system.unk138 /
   unk134 set. The up/down/left/right screens take the focus when the cursor
   leaves the grid on that side (fun_0021b858). */
struct MenuGridCell {
    u16 icon;                 /* 0x0: get_icon_frame icon */
    s16 frame;                /* 0x2: first frame */
    s16 kind;                 /* 0x4: 0 = item (owned D_0013D4C0[id]), else D_0013D388[id] */
    s16 id;                   /* 0x6: item id */
    s16 stream_entry;         /* 0x8: stream entry shown for this cell (fun_0021fdc8) */
}; /* size 0xA */

struct MenuItemGridData {
    s32 flags;                /* 0x30: 2 fixed row step, 4/8 lockable, 0x20 no equipped frame */
    f32 margin_x;             /* 0x34: left margin */
    f32 margin_y;             /* 0x38: top margin */
    s32 selected_cell;        /* 0x3C: highlighted cell */
    s32 rows;                 /* 0x40 */
    s32 cols;                 /* 0x44 */
    struct MenuGridCell *cells; /* 0x48: rows * cols */
    struct MenuScreen *up;    /* 0x4C */
    struct MenuScreen *down;  /* 0x50 */
    struct MenuScreen *left;  /* 0x54 */
    struct MenuScreen *right; /* 0x58 */
};

/* Icon list screen (FUN_00219fa0). */
struct MenuIcon {
    u16 icon;                 /* 0x0: get_icon_frame icon */
    s16 frame;                /* 0x2: get_icon_frame frame */
}; /* 0xA bytes apart */

struct MenuIconListData {
    u8 pad_30[0xC];
    s32 cursor;               /* 0x3C: highlighted icon */
    s32 count;                /* 0x40: icons drawn */
    u8 pad_44[0x4];
    struct MenuIcon *list;    /* 0x48 */
    u8 pad_4C[0x10];
    s32 first_y;              /* 0x5C: y of the first icon, 0x260 per icon */
};

/* Save slot screen (saving_data_menu, saving_data_menu2, loading_data_menu,
   draw_save_slot_list). Enter fun_00222f18 takes a stream buffer into
   save_data and sets step 0; leave fun_00222f58 gives the buffer back.
   flags 0x2000 is the second variant (FUN_00226b08, page D_001D4F98). */
struct MenuSaveSlotData {
    s32 flags;                /* 0x30: bit 0 pad repeat mask, 0x2000 second variant */
    u8 pad_34[0xC];
    s32 slot;                 /* 0x40: memory card slot 0..4, mirrored in D_0015EE34 */
    u8 pad_44[0x4];
    s32 save_data;            /* 0x48: buffer for prepare_save_game / FUN_00226a70 */
    s32 step;                 /* 0x4C: 0 idle, 1 start saving, 2 started */
};

/* Confirm prompt screen (draw_prompt_box). Enter fun_00221a48 clears
   menu_system.current->confirmed and takes the buffer; leave fun_00221a88
   gives it back (complete_stream_buffer_transfer). */
struct MenuPromptData {
    u8 pad_30[0x24];
    s32 buffer;               /* 0x54: stream buffer */
};

/* Item slot screen (FUN_0021c7a0). Confirm stores the grid's selected item
   at cursor, clears its old slot and advances the cursor. */
struct MenuItemSlotsData {
    s32 items[8];             /* 0x30: item id per slot */
    s32 cursor;               /* 0x50: slot 0..7, pad 8/4 step it */
};

/* Cycle screen (update_menu_cycle_selection). */
struct MenuCycleData {
    u8 pad_30[0x24];
    s32 selection;            /* 0x54: one of twelve, wraps */
};

/*
 * Text list screen (draw_menu_text_list, render_localized_ui_entry_list,
 * fun_0021abf8). Entry actions on confirm (fun_0021abf8): 0 none (dimmed,
 * skipped), 1/3 open page param, 4/5 mode_freeze_init(3, param), 6 message
 * close, 7/8/10 close with param, 9 requested level, 11 close; 2 draws text
 * 0x4F54. FUN_00221d68 steps `selected` modulo 30 with pad 0x40/0x20;
 * FUN_002220f0 reads items[selected].param.value of the focus list.
 */
struct MenuTextItem {
    s16 text;                 /* 0x0: help text id; 0 ends the list */
    s16 action;               /* 0x2: confirm action, see above */
    union {
        s32 value;            /* page, close value, level or language */
        struct {
            u16 lo;           /* action 6: menu_system.unkE4 */
            s16 hi;           /* action 6: menu_action_messages index, 0 = none */
        } half;
    } param;                  /* 0x4 */
    s16 subtext;              /* 0x8: second line text id, 0 = none */
    s16 fade_timer;           /* 0xA: up while selected, down otherwise */
}; /* size 0xC */

struct MenuTextListData {
    s32 flags;                /* 0x30: 1 pad repeat, 2 no highlight, 4/8 font, 0x10 fixed step, 0x20 D_001A0314, 0x1000 wrap, 0x8000 scroll once */
    struct MenuTextItem *items; /* 0x34: ended by a zero text */
    struct MenuScreen *up;    /* 0x38: focus above the first entry */
    struct MenuScreen *down;  /* 0x3C: focus below the last entry */
    s32 selected;             /* 0x40: highlighted entry */
    s32 scroll;               /* 0x44: text offset, keeps the selection visible */
};

/*
 * Resource stream screen (fun_0021fdc8, fun_0021f990, enter fun_0021fce0,
 * leave fun_0021fd78, draw_checking_memory_card_data_menu): streams the
 * entry picked by `flags` into one of two buffers.
 * flags: entry source 1 fixed_entry, 2 D_001A0314[0], 4 focus grid cell,
 * 0x100 focus save slot; 0x20 read to the buffer end and decompress_wad;
 * 0x200 select_next_stream_buffer argument; 0x400 cycle 19 entries by
 * elapsed_frames; 0x1000 add menu_language_resource_offsets[language].
 * state: 0/2 read the entry, 1/3 wait then unpack, -1 stopped.
 */
struct MenuStreamEntry {
    s32 sector;               /* 0x0: start_audio_stream_read sector */
    s32 sector_count;         /* 0x4: 0 = nothing to load */
};

struct MenuStreamData {
    struct MenuStreamEntry *entries; /* 0x30 */
    s32 flags;                /* 0x34: see above */
    s32 texture_width;        /* 0x38: draw size of the streamed image */
    s32 texture_height;       /* 0x3C */
    s32 *language_base;       /* 0x40: base entry for flags 0x1000 */
    s32 state;                /* 0x44: see above */
    s32 buffer[2];            /* 0x48: stream buffers */
    s32 loaded_entry[2];      /* 0x50: entry held by each buffer, -1 none */
    s32 fixed_entry;          /* 0x58: entry for flags 1, -1 none */
    s32 elapsed_frames;       /* 0x5C: updates since enter */
    s32 read_offset;          /* 0x60: data offset of a flags 0x20 read */
};

/* Item preview screen (FUN_0021e110, draw_available_item_preview_mobys,
   update_item_preview_moby). */
struct MenuItemPreviewData {
    u8 pad_30[0x14];
    char *moby;               /* 0x44: preview moby (create_menu_preview_moby) */
    char *second_moby;        /* 0x48: optional, drawn after moby */
};

/* Label screen (fun_0021a328). flags: 8/0x10 font; value source 0x20
   current level, 0x40 D_001A0314, 0x80 focus list selection, 0x100 focus
   grid cell, 0x1000 help entry of the focus list item. */
struct MenuLabelData {
    s32 flags;                /* 0x30: see above */
    s32 text_id;              /* 0x34: help text id, or table of text ids per value */
    u32 text_stride;          /* 0x38: byte stride of the text_id table */
    s32 scroll_offset;        /* 0x3C: text scroll, 1/16 px */
    u8 pad_40[0x4];
    s32 fade_timer;           /* 0x44: reset to scale_game_frames(menu_fade_duration) */
    s32 cached_value;         /* 0x48: value shown while fading */
    s32 value_variant;        /* 0x4C */
};

/*
 * One screen of a menu page (MenuPage.screens). FUN_002192a8 calls update
 * every frame (nonzero sets menu_system.close_request), enter when the page
 * becomes current (arg 0) or the screen gains focus (arg 1), and leave the
 * other way round (also FUN_002191b8).
 */
struct MenuScreen {
    s32 (*update)(struct MenuScreen *);      /* 0x00 */
    u8 pad_4[0x4];
    void (*enter)(struct MenuScreen *, s32); /* 0x08 */
    void (*leave)(struct MenuScreen *, s32); /* 0x0C */
    s32 unk10;                /* 0x10: bit 2 set by FUN_0021d1f8 */
    s32 moby;                 /* 0x14: its menu moby D_001D5D90[i]; menu sounds play on it */
    s32 x;                    /* 0x18: left edge */
    s32 y;                    /* 0x1C: top edge */
    s32 width;                /* 0x20 */
    s32 height;               /* 0x24 */
    u8 pad_28[0x8];
    union {
        struct MenuScreenWords raw;          /* not understood yet */
        struct MenuOptionListData options;   /* FUN_00220e28 */
        struct MenuChoiceListData choices;   /* FUN_002212b8 */
        struct MenuMissionListData missions; /* FUN_0021c4c0 */
        struct MenuItemGridData grid;        /* draw_menu_item_grid */
        struct MenuIconListData icons;       /* FUN_00219fa0 */
        struct MenuSaveSlotData save;        /* saving_data_menu, saving_data_menu2 */
        struct MenuItemSlotsData slots;      /* FUN_0021c7a0 */
        struct MenuCycleData cycle;          /* update_menu_cycle_selection */
        struct MenuTextListData list;        /* draw_menu_text_list, fun_0021abf8 */
        struct MenuStreamData stream;        /* fun_0021fdc8, fun_0021f990 */
        struct MenuItemPreviewData preview;  /* FUN_0021e110, update_item_preview_moby */
        struct MenuLabelData label;          /* fun_0021a328 */
        struct MenuPromptData prompt;        /* draw_prompt_box, fun_00221a48 */
    } data;                   /* 0x30 */
};

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_SCREEN_H */
