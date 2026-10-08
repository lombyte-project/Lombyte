#ifndef LOMBYTE_RNC_UI_MENUS_MENU_SCREEN_H
#define LOMBYTE_RNC_UI_MENUS_MENU_SCREEN_H

#include "types.h"

/* MenuScreen 0x30-0x5F seen as plain words (stream buffer screens and others). */
struct MenuScreenWords {
    s32 unk30;        /* 0x30: bit 0 picks the pad repeat mask (saving_data_menu) */
    s32 unk34;        /* 0x34: bit 0x200 passed to select_next_stream_buffer (FUN_0021fce0) */
    s32 unk38;
    s32 unk3C;        /* 0x3C: stream buffer (FUN_00225660) */
    s32 unk40;        /* 0x40: list index, wraps at 30 (FUN_00221d68) */
    s32 unk44;
    s32 unk48;        /* 0x48: stream buffer (FUN_00222f18/FUN_0021fce0) */
    s32 unk4C;        /* 0x4C: stream buffer (FUN_0021fce0) */
    s32 unk50;
    s32 unk54;        /* 0x54: stream buffer (FUN_00221a48) */
    s32 unk58;
    s32 unk5C;
};

/* One toggle of an option list screen (FUN_00220e28), 0x14 bytes. */
struct MenuOption {
    void *name;               /* 0x0: 0 ends the list (the cursor stops before it) */
    u8 *flag;                 /* 0x4: toggled by the confirm button (pad bit 0x40) unless type bit 0 is set */
    u8 pad_8[0x8];
    u32 type;                 /* 0x10: bit 0 = fade out (fade_to_black) and toggle D_0016034C instead of the flag */
};

/* Option list screen (FUN_00220e28): MenuScreen 0x30-0x3F. */
struct MenuOptionListData {
    s32 unk30;
    struct MenuOption *list;  /* 0x34: options, ended by a null name */
    s32 selection;            /* 0x38: moved by pad bits 0x1000 (up) and 0x4000 (down) */
    s32 fade_timer;           /* 0x3C: scale_game_frames(0x10) after fade_to_black(4); counted down, drives D_0015F43C */
};

/* One entry of a choice list screen (FUN_002212b8), 0x18 bytes. */
struct MenuChoice {
    s32 name;                 /* 0x0: 0 ends the list (the cursor stops before it) */
    u8 *value;                /* 0x4: index of the chosen option; confirm (pad bit 0x40) advances it modulo the option count */
    s32 option[4];            /* 0x8: options, 0 ends them early */
};

/* Choice list screen (FUN_002212b8): MenuScreen 0x30-0x3B. */
struct MenuChoiceListData {
    s32 unk30;
    struct MenuChoice *list;  /* 0x34: entries, ended by a zero name */
    s32 selection;            /* 0x38: moved by pad bits 0x1000 (up) and 0x4000 (down) */
};

/* Mission list screen (FUN_0021c4c0): MenuScreen 0x30-0x7F. */
struct MenuMissionListData {
    s32 choice[19];           /* 0x30: selected mission per level (D_001A00F0 level index); -1 while unfocused */
    s32 count;                /* 0x7C: collect_mission_ids result; choice wraps modulo it */
};

/* One cell of an item grid screen, 0xA bytes. */
struct MenuGridCell {
    u16 icon;                 /* 0x0: get_icon_frame icon */
    s16 frame;                /* 0x2: get_icon_frame first frame (+1 equipped, +2 locked, +4 D_0013E520) */
    s16 kind;                 /* 0x4: 0 = item (owned flag D_0013D4C0[id]), else D_0013D388[id] */
    s16 id;                   /* 0x6: item id */
    u16 pad_8;
};

/* Item grid screen (draw_menu_item_grid): MenuScreen 0x30-0x4B. */
struct MenuItemGridData {
    s32 flags;                /* 0x30: 2 = fixed row step, 4/8 = locked by menu_system.unk138/unk134, 0x20 = no equipped frame */
    f32 margin_x;             /* 0x34: left margin of the first column */
    f32 margin_y;             /* 0x38: top margin of the first row */
    s32 selected_cell;        /* 0x3C: index into cells drawn with the highlight */
    s32 rows;                 /* 0x40 */
    s32 cols;                 /* 0x44 */
    struct MenuGridCell *cells; /* 0x48: rows * cols cells */
};

/* One icon of an icon list screen, 0xA bytes apart. */
struct MenuIcon {
    u16 icon;                 /* 0x0: get_icon_frame icon */
    s16 frame;                /* 0x2: get_icon_frame frame */
};

/* Icon list screen (FUN_00219fa0): MenuScreen 0x30-0x5F. */
struct MenuIconListData {
    u8 pad_30[0xC];
    s32 cursor;               /* 0x3C: icon drawn with the highlight */
    s32 count;                /* 0x40: icons drawn */
    s32 unk44;
    struct MenuIcon *list;    /* 0x48: icons, 0xA bytes apart */
    u8 pad_4C[0x10];
    s32 first_y;              /* 0x5C: y of the first icon; 0x260 per icon */
};

/* Save slot screen (saving_data_menu, saving_data_menu2): MenuScreen 0x30-0x4F. */
struct MenuSaveSlotData {
    s32 flags;                /* 0x30: bit 0 picks the pad repeat mask; bit 0x2000 = second variant (FUN_00226b08, page D_001D4F98) */
    u8 pad_34[0xC];
    s32 slot;                 /* 0x40: memory card slot 0..4, mirrored in D_0015EE34 */
    s32 unk44;
    s32 save_data;            /* 0x48: save_data argument of prepare_save_game / FUN_00226a70 */
    s32 step;                 /* 0x4C: 0 idle, 1 = start saving this frame, 2 = started */
};

/* Item slot screen (FUN_0021c7a0): MenuScreen 0x30-0x53. */
struct MenuItemSlotsData {
    s32 items[8];             /* 0x30: item id per slot; confirm stores the grid's selected item at cursor and clears its old slot */
    s32 cursor;               /* 0x50: slot 0..7, pad bits 8/4 step it; advances after a store */
};

/* Cycle screen (update_menu_cycle_selection): MenuScreen 0x30-0x57. */
struct MenuCycleData {
    u8 pad_30[0x24];
    s32 selection;            /* 0x54: one of twelve, wraps both ways */
};

/*
 * One screen of a menu page (struct MenuPage.screens), passed to its own
 * handlers (menu_system.current->focus is the one with input focus). 0x0-0x2F
 * are shared by every screen; from 0x30 on each screen keeps its own data,
 * read through the `data` union: one named layout per kind of screen, `raw`
 * for screens whose words are not understood yet.
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
    /* 0x30: per-screen data; each handler reads one of these layouts. */
    union {
        struct MenuScreenWords raw;          /* screens whose words are not understood yet */
        struct MenuOptionListData options;  /* FUN_00220e28 */
        struct MenuChoiceListData choices;  /* FUN_002212b8 */
        struct MenuMissionListData missions; /* FUN_0021c4c0 */
        struct MenuItemGridData grid;       /* draw_menu_item_grid */
        struct MenuIconListData icons;      /* FUN_00219fa0 */
        struct MenuSaveSlotData save;       /* saving_data_menu, saving_data_menu2 */
        struct MenuItemSlotsData slots;     /* FUN_0021c7a0 */
        struct MenuCycleData cycle;         /* update_menu_cycle_selection */
    } data;
};

/* gcc 2.95 has no _Static_assert: a negative array size fails the build. */
#define MENU_SCREEN_OFFSET_CHECK(field, off) \
    typedef char menu_screen_offset_check_##field[ \
        ((unsigned long)&((struct MenuScreen *)0)->field == (off)) ? 1 : -1]
#define MENU_SCREEN_DATA_CHECK(layout, field, off) \
    typedef char menu_screen_offset_check_##layout##_##field[ \
        ((unsigned long)&((struct MenuScreen *)0)->data.layout.field == (off)) ? 1 : -1]
MENU_SCREEN_OFFSET_CHECK(update, 0x0);
MENU_SCREEN_OFFSET_CHECK(enter, 0x8);
MENU_SCREEN_OFFSET_CHECK(leave, 0xC);
MENU_SCREEN_OFFSET_CHECK(moby, 0x14);
MENU_SCREEN_OFFSET_CHECK(x, 0x18);
MENU_SCREEN_OFFSET_CHECK(y, 0x1C);
MENU_SCREEN_OFFSET_CHECK(width, 0x20);
MENU_SCREEN_OFFSET_CHECK(height, 0x24);
MENU_SCREEN_DATA_CHECK(raw, unk5C, 0x5C);
MENU_SCREEN_DATA_CHECK(options, selection, 0x38);
MENU_SCREEN_DATA_CHECK(choices, selection, 0x38);
MENU_SCREEN_DATA_CHECK(missions, count, 0x7C);
MENU_SCREEN_DATA_CHECK(grid, cells, 0x48);
MENU_SCREEN_DATA_CHECK(icons, first_y, 0x5C);
MENU_SCREEN_DATA_CHECK(save, step, 0x4C);
MENU_SCREEN_DATA_CHECK(slots, cursor, 0x50);
MENU_SCREEN_DATA_CHECK(cycle, selection, 0x54);
#undef MENU_SCREEN_OFFSET_CHECK
#undef MENU_SCREEN_DATA_CHECK

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_SCREEN_H */
