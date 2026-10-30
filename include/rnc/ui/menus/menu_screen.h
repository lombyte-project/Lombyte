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
    u8  pad_30[0x4];
    s32 unk34; /* 0x34: flags 0x40 / 0x80 (draw_map_screen) */
    s32 unk38; /* 0x38: cleared by enter_menu_help_text */
    s32 unk3C; /* 0x3C: stream buffer (leave_streamed_moby_screen) */
    u8  pad_40[0x10];
    s32 unk50; /* 0x50: 0, 1 or 3 (enter_menu_help_text) */
    s32 unk54; /* 0x54: cleared by enter_menu_help_text */
    u8  pad_58[0x8];
};

/* Option list screen (update_menu_option_list). */
struct MenuOption {
    s32 unk0;           /* 0x0: help text id of the option; 0 ends the list */
    u8 *flag;           /* 0x4: toggled by confirm (pad 0x40) */
    s32 first_message;  /* 0x8: text id */
    s32 second_message; /* 0xC: text id */
    u32 flags;          /* 0x10: bit 0 = fade out and toggle D_0016034C instead */
}; /* size 0x14 */

struct MenuOptionListData {
    s32 flags; /* 0x30: bit 0 draws a box while the list is empty (draw_cheats_menu) */
    struct MenuOption *list;       /* 0x34: ended by a null unk0 */
    s32                selection;  /* 0x38: pad 0x1000 up, 0x4000 down */
    s32                fade_timer; /* 0x3C: counts down after fade_to_black, drives D_0015F43C */
};

/* Choice list screen (update_menu_choice_list). */
struct MenuChoice {
    s32 unk0;      /* 0x0: 0 ends the list */
    u8 *value;     /* 0x4: chosen option; confirm advances it */
    s32 option[4]; /* 0x8: 0 ends them early */
}; /* size 0x18 */

struct MenuChoiceListData {
    u8                 pad_30[0x4];
    struct MenuChoice *list;      /* 0x34: ended by a zero unk0 */
    s32                selection; /* 0x38: pad 0x1000 up, 0x4000 down */
};

/* Mission list screen (update_menu_mission_list). */
struct MenuMissionListData {
    s32 choice[19]; /* 0x30: selected mission per level; -1 while unfocused */
    s32 count;      /* 0x7C: collect_mission_ids result */
};

/* Item grid screen (draw_menu_item_grid). Cell frames: +1 equipped,
   +2 locked, +4 D_0013E520. Locked: flags 4 / 8 with menu_system.unk138 /
   unk134 set. The up/down/left/right screens take the focus when the cursor
   leaves the grid on that side (update_menu_grid_selection). */
struct MenuGridCell {
    u16 icon;         /* 0x0: get_icon_frame icon */
    s16 frame;        /* 0x2: first frame */
    s16 kind;         /* 0x4: 0 = item (owned D_0013D4C0[id]), else D_0013D388[id] */
    s16 id;           /* 0x6: item id */
    s16 stream_entry; /* 0x8: stream entry shown for this cell (update_menu_resource_stream) */
}; /* size 0xA */

struct MenuItemGridData {
    s32                  flags; /* 0x30: 2 fixed row step, 4/8 lockable, 0x20 no equipped frame */
    f32                  margin_x;      /* 0x34: left margin */
    f32                  margin_y;      /* 0x38: top margin */
    s32                  selected_cell; /* 0x3C: highlighted cell */
    s32                  rows;          /* 0x40 */
    s32                  cols;          /* 0x44 */
    struct MenuGridCell *cells;         /* 0x48: rows * cols */
    struct MenuScreen   *up;            /* 0x4C */
    struct MenuScreen   *down;          /* 0x50 */
    struct MenuScreen   *left;          /* 0x54 */
    struct MenuScreen   *right;         /* 0x58 */
};

/* Icon list screen (draw_menu_icon_list). */
struct MenuIcon {
    u16 icon;  /* 0x0: get_icon_frame icon */
    s16 frame; /* 0x2: get_icon_frame frame */
}; /* 0xA bytes apart */

struct MenuIconListData {
    u8               pad_30[0xC];
    s32              cursor; /* 0x3C: highlighted icon */
    s32              count;  /* 0x40: icons drawn */
    u8               pad_44[0x4];
    struct MenuIcon *list; /* 0x48 */
    u8               pad_4C[0x10];
    s32              first_y; /* 0x5C: y of the first icon, 0x260 per icon */
};

/* Save slot screen (saving_data_menu, saving_data_menu2, loading_data_menu,
   draw_save_slot_list). Enter enter_save_slot_screen takes a stream buffer into
   save_data and sets step 0; leave leave_save_slot_screen gives the buffer back.
   flags 0x2000 is the second variant (FUN_00226b08, page challenge_confirm_page). */
struct MenuSaveSlotData {
    s32 flags; /* 0x30: bit 0 pad repeat mask, 0x2000 second variant */
    u8  pad_34[0xC];
    s32 slot; /* 0x40: memory card slot 0..4, mirrored in D_0015EE34 */
    u8  pad_44[0x4];
    s32 save_data; /* 0x48: buffer for prepare_save_game / start_memory_card_save */
    s32 step;      /* 0x4C: 0 idle, 1 start saving, 2 started */
};

/* Confirm prompt screen (draw_prompt_box). Enter enter_menu_prompt clears
   menu_system.current->confirmed and takes the buffer; leave leave_menu_prompt
   gives it back (release_menu_stream_buffer). */
struct MenuPromptData {
    u8  pad_30[0x24];
    s32 buffer; /* 0x54: stream buffer */
};

/* Item slot screen (update_menu_item_slots). Confirm stores the grid's selected item
   at cursor, clears its old slot and advances the cursor. */
struct MenuItemSlotsData {
    s32 items[8]; /* 0x30: item id per slot */
    s32 cursor;   /* 0x50: slot 0..7, pad 8/4 step it */
};

/* Cycle screen (update_menu_cycle_selection). */
struct MenuCycleData {
    u8  pad_30[0x8];
    s32 side; /* 0x38: draw_menu_side_indicator draws glyph 0x10 while set, else 0x11 */
    u8  pad_3C[0x18];
    s32 selection; /* 0x54: one of twelve, wraps */
};

/*
 * Text list screen (draw_menu_text_list, render_localized_ui_entry_list,
 * update_menu_entry_actions). Entry actions on confirm (update_menu_entry_actions): 0 none (dimmed,
 * skipped), 1/3 open page param, 4/5 mode_freeze_init(3, param), 6 message
 * close, 7/8/10 close with param, 9 requested level, 11 close; 2 draws text
 * 0x4F54. FUN_00221d68 steps `selected` modulo 30 with pad 0x40/0x20;
 * FUN_002220f0 reads items[selected].param.value of the focus list.
 */
/* MenuTextItem.action: what confirm does (update_menu_entry_actions). The
   REQUEST_CLOSE actions set menu_system.close_request: 6 -> 5, 7 -> 3,
   8 -> 4, 10 -> 6, 11 -> 7, with param in menu_system.unkE4. */
enum MenuTextAction {
    MENU_ACTION_LOCKED = -1,  /* goodies entry not unlocked yet: confirm does nothing */
    MENU_ACTION_DISABLED = 0, /* dimmed and skipped */
    MENU_ACTION_NO_OP = 1,
    MENU_ACTION_ALLOCATE_VOICE = 2,                           /* denied sound */
    MENU_ACTION_SET_NEXT_PAGE = 3,                            /* param: the page */
    MENU_ACTION_SET_NEXT_PAGE_OR_INITIALIZE_FREEZE = 4,       /* param: the page (save) */
    MENU_ACTION_SET_NEXT_PAGE_OR_INITIALIZE_TIMED_FREEZE = 5, /* param: the page (load) */
    MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE = 6,      /* param.half */
    MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD = 7,
    MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD = 8,
    MENU_ACTION_SET_LANGUAGE = 9, /* param: game_language */
    MENU_ACTION_REQUEST_CLOSE_6_WITH_WORD_PAYLOAD = 10,
    MENU_ACTION_REQUEST_CLOSE_PRESERVING_PAYLOAD = 11
};

struct MenuTextItem {
    s16 text;   /* 0x0: help text id; 0 ends the list */
    s16 action; /* 0x2: enum MenuTextAction */
    union {
        s32 value; /* page, close value, level or language */
        struct {
            u16 lo; /* action 6: menu_system.unkE4 */
            s16 hi; /* action 6: menu_action_messages index, 0 = none */
        } half;
    } param;        /* 0x4 */
    s16 subtext;    /* 0x8: second line text id, 0 = none */
    s16 fade_timer; /* 0xA: up while selected, down otherwise */
}; /* size 0xC */

struct MenuTextListData {
    s32 flags; /* 0x30: 1 pad repeat, 2 no highlight, 4/8 font, 0x10 fixed step, 0x20 D_001A0314, 0x1000 wrap, 0x8000 scroll once */
    struct MenuTextItem *items;    /* 0x34: ended by a zero text */
    struct MenuScreen   *up;       /* 0x38: focus above the first entry */
    struct MenuScreen   *down;     /* 0x3C: focus below the last entry */
    s32                  selected; /* 0x40: highlighted entry */
    s32                  scroll;   /* 0x44: text offset, keeps the selection visible */
};

/*
 * Resource stream screen (update_menu_resource_stream, update_menu_stream_image, enter enter_menu_stream_image,
 * leave leave_menu_stream_image, draw_checking_memory_card_data_menu): streams the
 * entry picked by `flags` into one of two buffers.
 * flags: entry source 1 fixed_entry, 2 D_001A0314[0], 4 focus grid cell,
 * 0x100 focus save slot; 0x20 read to the buffer end and decompress_wad;
 * 0x200 acquire_menu_stream_buffer argument; 0x400 cycle 19 entries by
 * elapsed_frames; 0x1000 add menu_language_resource_offsets[language].
 * state: 0/2 read the entry, 1/3 wait then unpack, -1 stopped.
 */
struct MenuStreamEntry {
    s32 sector;       /* 0x0: start_audio_stream_read sector */
    s32 sector_count; /* 0x4: 0 = nothing to load */
};

struct MenuStreamData {
    struct MenuStreamEntry *entries;         /* 0x30 */
    s32                     flags;           /* 0x34: see above */
    s32                     texture_width;   /* 0x38: draw size of the streamed image */
    s32                     texture_height;  /* 0x3C */
    s32                    *language_base;   /* 0x40: base entry for flags 0x1000 */
    s32                     state;           /* 0x44: see above */
    s32                     buffer[2];       /* 0x48: stream buffers */
    s32                     loaded_entry[2]; /* 0x50: entry held by each buffer, -1 none */
    s32                     fixed_entry;     /* 0x58: entry for flags 1, -1 none */
    s32                     elapsed_frames;  /* 0x5C: updates since enter */
    s32                     read_offset;     /* 0x60: data offset of a flags 0x20 read */
};

/* Item preview screen (draw_item_preview_screen, draw_available_item_preview_mobys,
   update_item_preview_moby). */
struct MenuItemPreviewData {
    s32   flags;          /* 0x30: update_item_preview_binding */
    s32   state;          /* 0x34 */
    f32   rotation_angle; /* 0x38 */
    u8    pad_3C[0x8];
    char *moby;        /* 0x44: preview moby (create_menu_preview_moby) */
    char *second_moby; /* 0x48: optional, drawn after moby */
};

/* Label screen (render_configured_text_label). flags: 8/0x10 font; value source 0x20
   current level, 0x40 D_001A0314, 0x80 focus list selection, 0x100 focus
   grid cell, 0x1000 help entry of the focus list item. */
struct MenuLabelData {
    s32 flags;         /* 0x30: see above */
    s32 text_id;       /* 0x34: help text id, or table of text ids per value */
    u32 text_stride;   /* 0x38: byte stride of the text_id table */
    s32 scroll_offset; /* 0x3C: text scroll, 1/16 px */
    u8  pad_40[0x4];
    s32 fade_timer;    /* 0x44: reset to scale_game_frames(menu_fade_duration) */
    s32 cached_value;  /* 0x48: value shown while fading */
    s32 value_variant; /* 0x4C */
};

/* MenuScreen.unk10: how the page draws the screen (port docs/port/rerac/plan/menus.md). */
#define MENU_SCREEN_DRAW_DIRECT 0x1
#define MENU_SCREEN_DRAW_3D     0x2 /* drawn in the first, 3D pass */
#define MENU_SCREEN_HIDDEN      0x4

/* MenuLabelData.flags (render_configured_text_label). Without a BY_ flag the
   value is the item id of the focus grid's selected cell. */
#define MENU_LABEL_CENTER_X          0x1
#define MENU_LABEL_CENTER_Y          0x2
#define MENU_LABEL_FIXED_TEXT        0x4 /* text_id is the text itself */
#define MENU_LABEL_LARGE_FONT        0x8
#define MENU_LABEL_SMALL_FONT        0x10
#define MENU_LABEL_BY_CURRENT_LEVEL  0x20 /* value current_level_index - 1 */
#define MENU_LABEL_BY_SELECTED_LEVEL 0x40 /* value selected_level_index[0] - 1 */
#define MENU_LABEL_BY_LIST_SELECTION 0x80 /* value: the focus list's selected entry */
#define MENU_LABEL_BY_GRID_CELL      0x100
#define MENU_LABEL_PLANET_PREFIX     0x200 /* "Planet %s" (text 0x4ECC) around the text */
#define MENU_LABEL_SCROLLING         0x400 /* set while the text overflows */
#define MENU_LABEL_LOCKED_TEXT       0x800 /* text 0x4F54 while the item is locked */
#define MENU_LABEL_BY_HELP_ENTRY     0x1000
#define MENU_LABEL_NO_SCROLL         0x2000
#define MENU_LABEL_CENTER_TEXT_523E  0x4000 /* with FIXED_TEXT: text 0x523E centred, y 12 */
#define MENU_LABEL_VIDEO_MODE_VARIANT \
    0x8000 /* with BY_LIST_SELECTION: second table column when pal_mode is set */
#define MENU_LABEL_FULL_HEIGHT 0x10000

/* MenuTextListData.flags (update_menu_entry_actions, render_localized_ui_entry_list,
   draw_menu_text_list). */
#define MENU_LIST_BUTTONS_ONLY      0x1 /* cursor keys from the buttons, not the stick */
#define MENU_LIST_FIXED_COLOR       0x2 /* no highlight */
#define MENU_LIST_LARGE_FONT        0x4
#define MENU_LIST_SMALL_FONT        0x8
#define MENU_LIST_FIXED_STEP        0x10 /* rows font size + 3 apart, not spread over the height */
#define MENU_LIST_SETS_LEVEL        0x20 /* moving sets selected_level_index */
#define MENU_LIST_LEFT_ALIGNED      0x40
#define MENU_LIST_NO_COLOR_CODES    0x80
#define MENU_LIST_SHOULDER_KEYS     0x100 /* L1 / R1 move the cursor too */
#define MENU_LIST_SKILL_POINT_MARKS 0x200 /* check mark per entry: item_unlocked[i] */
#define MENU_LIST_CENTERED          0x400 /* draw_menu_text_list */
#define MENU_LIST_LANGUAGE_MARKS    0x800 /* check mark on the entry of game_language */
#define MENU_LIST_WRAP              0x1000
#define MENU_LIST_CENTER_WIDEST     0x4000 /* left edge of the widest entry centred */
#define MENU_LIST_SCROLL_ONCE       0x8000
#define MENU_LIST_NO_SCROLL         0x10000
#define MENU_LIST_SHRINK_TO_FIT     0x20000 /* switches to the small font when too wide */

/* MenuItemGridData.flags (update_menu_grid_selection, draw_menu_item_grid). */
#define MENU_GRID_NO_CONFIRM         0x1
#define MENU_GRID_FIXED_ROW_STEP     0x2
#define MENU_GRID_HIDDEN_WITH_UNK138 0x4 /* skipped, cells locked while menu_system.unk138 */
#define MENU_GRID_HIDDEN_WITH_UNK134 0x8 /* skipped, cells locked while menu_system.unk134 */
#define MENU_GRID_NO_EQUIPPED_FRAME  0x20
#define MENU_GRID_NO_WRAP            0x8000

/* MenuStreamData.flags (update_menu_resource_stream, update_menu_stream_image). */
#define MENU_STREAM_FIXED_ENTRY       0x1
#define MENU_STREAM_BY_SELECTED_LEVEL 0x2
#define MENU_STREAM_BY_GRID_CELL      0x4
#define MENU_STREAM_BY_CELL_ENTRY     0x8  /* the focus grid cell's stream_entry */
#define MENU_STREAM_UNPACK            0x20 /* read to the buffer end, then decompress_wad */
#define MENU_STREAM_BY_SAVE_SLOT      0x100
#define MENU_STREAM_BUFFER_ARG        0x200 /* acquire_menu_stream_buffer argument */
#define MENU_STREAM_CYCLE             0x400 /* 19 entries by elapsed_frames */
#define MENU_STREAM_BY_LANGUAGE       0x1000
#define MENU_STREAM_SKILL_POINT_LOCK  0x2000 /* entry 0x1E while item_unlocked[entry] is 0 */
#define MENU_STREAM_DENIED_ENTRY      0x4000 /* entry 9 for a list entry with action 2 */

/* MenuOption.flags (update_menu_option_list). */
#define MENU_OPTION_FADE_TOGGLE 0x1 /* fade out and toggle D_0016034C instead */

/*
 * One screen of a menu page (MenuPage.screens). update_menu_system calls update
 * every frame (nonzero sets menu_system.close_request), enter when the page
 * becomes current (arg 0) or the screen gains focus (arg 1), and leave the
 * other way round (also close_menu_system).
 */
struct MenuScreen {
    s32 (*update)(struct MenuScreen *);      /* 0x00 */
    s32 (*draw)(struct MenuScreen *);        /* 0x04 */
    void (*enter)(struct MenuScreen *, s32); /* 0x08 */
    void (*leave)(struct MenuScreen *, s32); /* 0x0C */
    s32 unk10;                               /* 0x10: bit 2 set by enter_menu_help_text */
    s32 moby;   /* 0x14: its menu moby D_001D5D90[i]; menu sounds play on it */
    s32 x;      /* 0x18: left edge */
    s32 y;      /* 0x1C: top edge */
    s32 width;  /* 0x20 */
    s32 height; /* 0x24 */
    u8  pad_28[0x8];
    union {
        struct MenuScreenWords     raw;      /* not understood yet */
        struct MenuOptionListData  options;  /* update_menu_option_list */
        struct MenuChoiceListData  choices;  /* update_menu_choice_list */
        struct MenuMissionListData missions; /* update_menu_mission_list */
        struct MenuItemGridData    grid;     /* draw_menu_item_grid */
        struct MenuIconListData    icons;    /* draw_menu_icon_list */
        struct MenuSaveSlotData    save;     /* saving_data_menu, saving_data_menu2 */
        struct MenuItemSlotsData   slots;    /* update_menu_item_slots */
        struct MenuCycleData       cycle;    /* update_menu_cycle_selection */
        struct MenuTextListData    list;     /* draw_menu_text_list, update_menu_entry_actions */
        struct MenuStreamData stream; /* update_menu_resource_stream, update_menu_stream_image */
        struct MenuItemPreviewData preview; /* draw_item_preview_screen, update_item_preview_moby */
        struct MenuLabelData       label;   /* render_configured_text_label */
        struct MenuPromptData      prompt;  /* draw_prompt_box, enter_menu_prompt */
    } data;                                 /* 0x30 */
};

/*
 * Screen objects of the menu page graph (D_001CE2F8-D_001D5020, defined in
 * audio/voices/pause_all_sounds.c and ui/menus/initialize_menu_preview_scene.c).
 * Every screen starts with MenuScreen's shared head (0x00-0x2F); the data after
 * it depends on the kind of screen, and so does its size: every instance with
 * the same draw function has the same size, so each kind is its own struct.
 * MenuPlainScreenNN are kinds whose own data starts zeroed in every instance.
 * Pages hold them as struct MenuScreen pointers.
 */
struct MenuScreenHead {
    s32 (*update)(struct MenuScreen *);      /* 0x00 */
    s32 (*draw)(struct MenuScreen *);        /* 0x04 */
    void (*enter)(struct MenuScreen *, s32); /* 0x08 */
    void (*leave)(struct MenuScreen *, s32); /* 0x0C */
    s32 unk10;                               /* 0x10 */
    s32 moby;                                /* 0x14 */
    s32 x;                                   /* 0x18 */
    s32 y;                                   /* 0x1C */
    s32 width;                               /* 0x20 */
    s32 height;                              /* 0x24 */
    u8  pad_28[0x8];
}; /* size 0x30 */

/* draw_level_selection_map_screen: the head only. */
struct MenuLevelMapScreen {
    struct MenuScreenHead head;
}; /* size 0x30 */

/* update_menu_option_list / draw_cheats_menu. */
struct MenuOptionListScreen {
    struct MenuScreenHead     head;
    struct MenuOptionListData options;
}; /* size 0x40 */

/* update_menu_choice_list / draw_menu_choice_list. */
struct MenuChoiceListScreen {
    struct MenuScreenHead     head;
    struct MenuChoiceListData choices;
    u8                        pad_3C[0x4];
}; /* size 0x40 */

/* Text lists: render_localized_ui_entry_list, draw_menu_text_list. */
struct MenuTextListScreen {
    struct MenuScreenHead   head;
    struct MenuTextListData list;
    u8                      pad_48[0x8];
}; /* size 0x50 */

/* Save slot lists: draw_save_slot_list. */
struct MenuSaveSlotScreen {
    struct MenuScreenHead   head;
    struct MenuSaveSlotData save;
}; /* size 0x50 */

/* draw_quit_game_menu, draw_sound_menu. */
struct MenuPlainScreen50 {
    struct MenuScreenHead head;
    u8                    pad_30[0x20];
}; /* size 0x50 */

/* Labels: render_configured_text_label. */
struct MenuLabelScreen {
    struct MenuScreenHead head;
    struct MenuLabelData  label;
    u8                    pad_50[0x8];
}; /* size 0x58 */

/* draw_prompt_box. */
struct MenuPromptScreen {
    struct MenuScreenHead head;
    struct MenuPromptData prompt;
}; /* size 0x58 */

/* draw_menu_side_indicator (update_menu_cycle_selection). */
struct MenuCycleScreen {
    struct MenuScreenHead head;
    struct MenuCycleData  cycle;
}; /* size 0x58 */

/* draw_pause_ring (update_menu_item_slots). */
struct MenuItemSlotsScreen {
    struct MenuScreenHead    head;
    struct MenuItemSlotsData slots;
    u8                       pad_54[0x4];
}; /* size 0x58 */

/* draw_missions_menu, draw_selected_menu_item_text, draw_menu_text_box,
   draw_localized_three_option_menu, draw_menu_selection_text and the draws
   at 0x21F5F8 and 0x222948. */
struct MenuPlainScreen58 {
    struct MenuScreenHead head;
    u8                    pad_30[0x28];
}; /* size 0x58 */

/* draw_menu_item_grid. */
struct MenuGridScreen {
    struct MenuScreenHead   head;
    struct MenuItemGridData grid;
    u8                      pad_5C[0xC];
}; /* size 0x68 */

/* Streamed images: draw_transition_overlay, draw_checking_memory_card_data_menu,
   draw_menu_stream_image. */
struct MenuStreamScreen {
    struct MenuScreenHead head;
    struct MenuStreamData stream;
    u8                    pad_64[0x4];
}; /* size 0x68 */

/* draw_menu_icon_list. */
struct MenuIconListScreen {
    struct MenuScreenHead   head;
    struct MenuIconListData icons;
    u8                      pad_60[0x8];
}; /* size 0x68 */

/* Map screens: draw_map_screen, draw_map_screen_overlay, draw_map_screen_layers. */
struct MenuMapScreen {
    struct MenuScreenHead  head;
    struct MenuScreenWords raw;
    u8                     pad_60[0x8];
}; /* size 0x68 */

/* draw_selected_item_ammo. */
struct MenuPlainScreen68 {
    struct MenuScreenHead head;
    u8                    pad_30[0x38];
}; /* size 0x68 */

/* Item previews: draw_menu_preview_objects, draw_item_preview_screen,
   draw_available_item_preview_mobys, draw_items_menu, setup_menu_display_text. */
struct MenuItemPreviewScreen {
    struct MenuScreenHead      head;
    struct MenuItemPreviewData preview;
    u8                         pad_4C[0x74];
}; /* size 0xC0 */

/* draw_moby_entries_from_object and the draw at 0x21DF28. */
struct MenuPlainScreenC0 {
    struct MenuScreenHead head;
    u8                    pad_30[0x90];
}; /* size 0xC0 */

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_SCREEN_H */
