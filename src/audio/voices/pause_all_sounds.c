#include "types.h"
#include "sda.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_pages.h"

#include "rnc/camera/update_cam.h"
#include "rnc/globals.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/audio/voices/snd_pause_all_sounds_in_group.h"

extern s32 D_0015F674 MACRO_ADDR;
extern s32            D_00141660[];
extern u8 D_0014161B  NOT_SDA;
extern s32            D_0015EEA0_macro __asm__("D_0015EEA0") MACRO_ADDR;
extern s32            alternate_animation_sequence_macro __asm__("D_0015EE20") MACRO_ADDR;
extern s32            D_001D0398[]; /* help_history_list list.selected */

extern void music_pause(s32);
extern s32  snd_flush_sound_commands() __asm__("FUN_0012dc80");
extern s32  update_mission_list() __asm__("FUN_0020b950");
extern void update_skill_point_and_gold_weapon_menu_entries() __asm__("func_00226E58");

void pause_all_sounds(s32 mode) __asm__("FUN_00218d78");

extern s32 game_mode_macro __asm__("D_0015F604") MACRO_ADDR;

void pause_all_sounds(s32 mode) {
    struct MenuSystem *g;
    snd_pause_all_sounds_in_group(0x1D);
    music_pause(0);
    snd_flush_sound_commands();
    if (camera_position_publication_suppressed[0] != 0) {
        D_0015F674 = 1;
        return;
    }
    if (D_00141660[0] == 0x24) {
        D_00141660[0] = 0;
    }
    {
        struct MenuSystem *g = &menu_system;
        g->unk134 = *(volatile s32 *)&current_level_index == 0xD || D_0014161B != 0;
    }
    {
        struct MenuSystem *g = &menu_system;
        g->unk138 = current_level_index == 0 || current_level_index == 0xE;
    }
    {
        struct MenuSystem *g = &menu_system;
        g->unkD8 =
            D_0015EEA0_macro != 0 || alternate_animation_sequence_macro != 0 || g->unkF8 != 0;
    }
    {
        struct MenuSystem *g = &menu_system;
        char              *menu_entry_base = (char *)&pause_menu_chain_head;
        g->unkDC = mode == 0x23;
        *(char **)(menu_entry_base + 0x38) =
            g->unkD8 ? (char *)&pause_goodies_tab : (char *)&pause_menu_chain_tail;
    }
    {
        struct MenuSystem *g = &menu_system;
        char              *menu_record_base = (char *)&pause_menu_chain_tail;
        *(char **)(menu_record_base + 0x3C) =
            g->unkD8 ? (char *)&pause_goodies_tab : (char *)&pause_menu_chain_head;
    }
    {
        struct MenuSystem *g = &menu_system;
        game_mode_macro = 3;
        g->state = mode;
        g->close_request = 0;
        g->unk10 = 0;
        g->update_count = 0;
    }
    {
        s32 level_index_snapshot = *(volatile s32 *)&current_level_index;
        if (level_index_snapshot < 0x13) {
            selected_level_index[0] = level_index_snapshot;
        } else {
            selected_level_index[0] = 0;
        }
    }
    update_mission_list();
    D_001D0398[0] = 0;
    update_skill_point_and_gold_weapon_menu_entries();
    {
        struct MenuSystem *g = &menu_system;
        g->unk13C = 1;
        g->unk140 = 0;
    }
}

extern __typeof__(pause_all_sounds) func_00218D78 __attribute__((alias("FUN_00218d78")));

/* Defined below their only users, so retail reaches them with lui. */
s32 D_0015F674 MACRO_ADDR = 0;

/*
 * The pause pages of the menu page graph (rnc/ui/menus/menu_pages.h), from
 * pause_root_page to the help moves list moves_help_items. Types come from the
 * screens' draw functions; list lengths from their 0 terminators, grid cells from
 * rows * cols, label text tables from the list or grid they follow. Text ids are
 * help message ids; the English quoted next to some is from the port's reading of
 * the same graph in level01 (Lombyte-Tools docs/port/rerac/plan/menus.md).
 */
#define SCREEN(screen) ((struct MenuScreen *)&(screen))

extern s32  FUN_00219d80(struct MenuScreen *);
extern void FUN_0021a318(struct MenuScreen *, s32); /* clear_menu_selected_index */
extern s32  FUN_0021a328(struct MenuScreen *);      /* render_configured_text_label */
extern s32  FUN_0021abf8(struct MenuScreen *);      /* update_menu_entry_actions */
extern s32  FUN_0021b1c8(struct MenuScreen *);      /* render_localized_ui_entry_list */
extern void FUN_0021b7a8(struct MenuScreen *, s32); /* find_next_pause_page */
extern s32  FUN_0021b858(struct MenuScreen *);      /* update_menu_grid_selection */
extern s32  FUN_0021bd98(struct MenuScreen *);      /* menu_update_noop */
extern void FUN_0021bda0(struct MenuScreen *, s32); /* leave_map_screen */
extern s32  FUN_0021be60(struct MenuScreen *);      /* draw_map_screen */
extern void FUN_0021c420(struct MenuScreen *, s32); /* enter_menu_mission_list */
extern s32  FUN_0021c4c0(struct MenuScreen *);      /* update_menu_mission_list */
extern s32  FUN_0021c7a0(struct MenuScreen *);      /* update_menu_item_slots */
extern void FUN_0021c9c8(struct MenuScreen *, s32); /* load_selection_list */
extern void FUN_0021ca60(struct MenuScreen *, s32); /* pause_stage_sprite_lists */
extern void FUN_0021cac8(struct MenuScreen *, s32); /* reset_pause_memory_card_state */
extern void FUN_0021cae0(struct MenuScreen *, s32); /* reset_menu_graphics_buffers */
extern void FUN_0021d168(struct MenuScreen *, s32);
extern void FUN_0021d1f0(struct MenuScreen *, s32); /* menu_leave_noop */
extern void FUN_0021d1f8(struct MenuScreen *, s32); /* enter_menu_help_text */
extern void FUN_0021d2c8(struct MenuScreen *, s32); /* restore_menu_help_text */
extern s32  FUN_0021d338(struct MenuScreen *);      /* update_menu_help_text_load */
extern s32  FUN_0021d4a8(struct MenuScreen *);      /* draw_menu_text_list */
extern s32  FUN_0021d948(struct MenuScreen *);      /* draw_menu_item_grid */
extern void FUN_0021ddd0(struct MenuScreen *, s32); /* set_menu_pause_flag_state */
extern s32  FUN_0021ddf8(struct MenuScreen *);      /* draw_menu_preview_objects */
extern s32  FUN_0021df28(struct MenuScreen *);      /* menu_draw_noop */
extern void FUN_0021df30(struct MenuScreen *, s32); /* enter_item_preview */
extern void FUN_0021df58(struct MenuScreen *, s32); /* clear_menu_preview_pair */
extern s32  FUN_0021df98(struct MenuScreen *);      /* update_item_preview_moby */
extern s32  FUN_0021e110(struct MenuScreen *);      /* draw_item_preview_screen */
extern s32  FUN_0021f158(struct MenuScreen *);      /* draw_selected_item_ammo */
extern s32  FUN_0021f330(struct MenuScreen *);      /* draw_map_screen_overlay */
extern s32  FUN_0021f368(struct MenuScreen *);      /* draw_missions_menu */
extern s32  FUN_0021f688(struct MenuScreen *);      /* draw_missions_menu2 */
extern s32  FUN_0021f990(struct MenuScreen *);      /* update_menu_stream_image */
extern s32  FUN_0021fc68(struct MenuScreen *);      /* draw_transition_overlay */
extern void FUN_0021fce0(struct MenuScreen *, s32); /* enter_menu_stream_image */
extern void FUN_0021fd78(struct MenuScreen *, s32); /* leave_menu_stream_image */
extern s32  FUN_0021fdc8(struct MenuScreen *);      /* update_menu_resource_stream */
extern s32  FUN_00220348(struct MenuScreen *);      /* draw_checking_memory_card_data_menu */
extern s32  FUN_00220648(struct MenuScreen *);      /* advance_audio_stream_state */
extern s32  FUN_00220790(struct MenuScreen *);      /* draw_menu_stream_image */
extern s32  FUN_00220850(struct MenuScreen *);      /* draw_map_screen_layers */
extern s32  FUN_00220b20(struct MenuScreen *);      /* draw_pause_ring */
extern void FUN_002240c8(struct MenuScreen *, s32); /* initialize_menu_preview_objects */
extern void FUN_002242b8(struct MenuScreen *, s32); /* release_menu_preview_objects */
extern s32  FUN_00224368(struct MenuScreen *);      /* load_hand_gadget */

extern struct MenuStreamEntry D_00138008[];
extern struct MenuStreamEntry D_00138080[];
extern u8                     D_001DDD4C[];
extern u8                     D_001DDD50[];

struct MenuScreen menu_placeholder_screen = {
    0,
    0,
    0,
    0,
    MENU_SCREEN_HIDDEN,
    0,
    0,
    0,
    0,
    0,
    {0},
    {.missions = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10, 0x11, 0x12}}}};

struct MenuTextItem toggle_exit_hint_items[3] = {
    {0x4EDF}, /* "Cross: Toggle" */
    {0x4EE0}, /* "Triangle: Exit" */
    {0},
};

struct MenuTextListScreen toggle_exit_hint_list = {
    {                                              0,           FUN_0021b1c8},
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST, toggle_exit_hint_items}
};

struct MenuTextItem exit_hint_items[2] = {
    {0x4EE0}, /* "Triangle: Exit" */
    {0},
};

struct MenuTextItem level_map_hint_items[4] = {
    {0x4EE0}, /* "Triangle: Exit" */
    {0x4EEF},
    {0x4F71},
    {0},
};

struct MenuTextListScreen exit_hint_list = {
    {                                              0,    FUN_0021b1c8},
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST, exit_hint_items}
};

struct MenuPage pause_root_page = {
    {0xE, 0xF, 0x10, 0x11, 0x12, 0x13, 0x14, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    2,
    SCREEN(pause_menu_chain_head),
    {SCREEN(pause_menu_chain_head),
     SCREEN(pause_gadgets_tab),
     SCREEN(pause_quick_select_tab),
     SCREEN(pause_items_tab),
     SCREEN(pause_help_tab),
     SCREEN(pause_menu_chain_tail),
     SCREEN(pause_goodies_tab)}
};

struct MenuTextItem pause_weapons_tab_items[2] = {
    {0x4EE2, MENU_ACTION_SET_NEXT_PAGE, {(s32)&pause_weapons_page}}, /* "Weapons" */
    {0},
};

struct MenuTextItem pause_gadgets_tab_items[2] = {
    {0x4EE3, MENU_ACTION_SET_NEXT_PAGE, {(s32)&pause_gadgets_page}}, /* "Gadgets" */
    {0},
};

struct MenuTextItem pause_quick_select_tab_items[2] = {
    {0x4EE4, MENU_ACTION_SET_NEXT_PAGE, {(s32)&pause_quick_select_page}}, /* "Quick Select" */
    {0},
};

struct MenuTextItem pause_items_tab_items[2] = {
    {0x4EE5, MENU_ACTION_SET_NEXT_PAGE, {(s32)&pause_items_page}}, /* "Items" */
    {0},
};

struct MenuTextItem pause_help_tab_items[2] = {
    {0x4EE6, MENU_ACTION_SET_NEXT_PAGE, {(s32)&pause_help_page}}, /* "Help" */
    {0},
};

struct MenuTextItem pause_options_tab_items[2] = {
    {0x4EE7, MENU_ACTION_SET_NEXT_PAGE, {(s32)&pause_options_page}}, /* "Options" */
    {0},
};

struct MenuTextItem pause_goodies_tab_items[2] = {
    {0x4FC2, MENU_ACTION_SET_NEXT_PAGE, {(s32)&goodies_page}}, /* "Goodies" */
    {0},
};

struct MenuTextListScreen pause_menu_chain_head = {
    {FUN_0021abf8, FUN_0021b1c8, FUN_0021ddd0},
    {MENU_LIST_LARGE_FONT, pause_weapons_tab_items, 0, SCREEN(pause_gadgets_tab)}
};

struct MenuTextListScreen pause_gadgets_tab = {
    {FUN_0021abf8, FUN_0021b1c8, FUN_0021ddd0},
    {MENU_LIST_LARGE_FONT,
     pause_gadgets_tab_items, SCREEN(pause_menu_chain_head),
     SCREEN(pause_quick_select_tab)}
};

struct MenuTextListScreen pause_quick_select_tab = {
    {FUN_0021abf8, FUN_0021b1c8, FUN_0021ddd0},
    {MENU_LIST_LARGE_FONT,
     pause_quick_select_tab_items, SCREEN(pause_gadgets_tab),
     SCREEN(pause_items_tab)}
};

struct MenuTextListScreen pause_items_tab = {
    {FUN_0021abf8, FUN_0021b1c8},
    {MENU_LIST_LARGE_FONT,
     pause_items_tab_items, SCREEN(pause_quick_select_tab),
     SCREEN(pause_help_tab)}
};

struct MenuTextListScreen pause_help_tab = {
    {FUN_0021abf8, FUN_0021b1c8},
    {MENU_LIST_LARGE_FONT,
     pause_help_tab_items, SCREEN(pause_items_tab),
     SCREEN(pause_menu_chain_tail)}
};

struct MenuTextListScreen pause_menu_chain_tail = {
    {FUN_0021abf8, FUN_0021b1c8},
    {MENU_LIST_LARGE_FONT, pause_options_tab_items, SCREEN(pause_help_tab)}
};

struct MenuTextListScreen pause_goodies_tab = {
    {FUN_0021abf8, FUN_0021b1c8},
    {MENU_LIST_LARGE_FONT,
     pause_goodies_tab_items, SCREEN(pause_menu_chain_tail),
     SCREEN(pause_menu_chain_head)}
};

struct MenuPage pause_gadgets_page = {
    {                       0x1A,0x1B,                   0x22,0x23,                  0x1E,0x1F,             0x14,0x24,0x21,0x25,0x26,0x27,0x28,0x29                  },
    &pause_root_page,
    4,
    SCREEN(pause_pack_gadget_grid),
    {SCREEN(pause_gadgets_title),
     SCREEN(pause_hand_model_screen),
     SCREEN(pause_hand_gadget_grid),
     SCREEN(pause_pack_gadget_grid),
     SCREEN(pause_item_info_label),
     SCREEN(item_preview_screen),
     &menu_placeholder_screen,
     SCREEN(pause_foot_gadgets_heading),
     SCREEN(equip_exit_hint_list),
     SCREEN(pause_head_gadget_grid),
     SCREEN(pause_foot_gadget_grid),
     SCREEN(pause_head_gadgets_heading),
     SCREEN(pause_pack_gadgets_heading),
     SCREEN(pause_hand_gadgets_heading)}
};

struct MenuLabelScreen pause_gadgets_title = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4EE3}
};

struct MenuLabelScreen pause_item_info_label = {
    {                             FUN_00219d80,        FUN_0021a328,         FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y, (s32)menu_item_info, sizeof(MenuItemInfo)}
};

struct MenuLabelScreen pause_hand_gadgets_heading = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_FULL_HEIGHT | MENU_LABEL_NO_SCROLL | MENU_LABEL_SMALL_FONT | MENU_LABEL_FIXED_TEXT |
         MENU_LABEL_CENTER_X, 0x4EE8}  /* "Hand" */
};

struct MenuLabelScreen pause_pack_gadgets_heading = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_FULL_HEIGHT | MENU_LABEL_NO_SCROLL | MENU_LABEL_SMALL_FONT | MENU_LABEL_FIXED_TEXT |
         MENU_LABEL_CENTER_X, 0x4EE9}  /* "Back Packs" */
};

struct MenuLabelScreen pause_head_gadgets_heading = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_FULL_HEIGHT | MENU_LABEL_NO_SCROLL | MENU_LABEL_SMALL_FONT | MENU_LABEL_FIXED_TEXT |
         MENU_LABEL_CENTER_X, 0x4EEA}  /* "Head" */
};

struct MenuLabelScreen pause_foot_gadgets_heading = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_FULL_HEIGHT | MENU_LABEL_NO_SCROLL | MENU_LABEL_SMALL_FONT | MENU_LABEL_FIXED_TEXT |
         MENU_LABEL_CENTER_X, 0x4EEB}  /* "Foot" */
};

struct MenuGridScreen pause_hand_gadget_grid = {
    {FUN_0021b858, FUN_0021d948},
    {MENU_GRID_NO_EQUIPPED_FRAME,
     0.1f, 0.1f,
     0, 2,
     3, pause_hand_gadget_cells,
     SCREEN(pause_foot_gadget_grid),
     SCREEN(pause_pack_gadget_grid)}
};

struct MenuGridScreen pause_pack_gadget_grid = {
    {FUN_0021b858, FUN_0021d948, FUN_0021b7a8},
    {MENU_GRID_HIDDEN_WITH_UNK138,
     0.1f, 0.1f,
     0, 1,
     3, pause_pack_gadget_cells,
     SCREEN(pause_hand_gadget_grid),
     SCREEN(pause_head_gadget_grid)}
};

struct MenuGridScreen pause_head_gadget_grid = {
    {FUN_0021b858, FUN_0021d948, FUN_0021b7a8},
    {MENU_GRID_HIDDEN_WITH_UNK134,
     0.1f, 0.1f,
     0, 1,
     3, pause_head_gadget_cells,
     SCREEN(pause_pack_gadget_grid),
     SCREEN(pause_foot_gadget_grid)}
};

struct MenuGridScreen pause_foot_gadget_grid = {
    {FUN_0021b858, FUN_0021d948, FUN_0021b7a8},
    {0x10 /* not read by the grid code */,
     0.1f, 0.1f,
     0, 1,
     2, pause_foot_gadget_cells,
     SCREEN(pause_head_gadget_grid),
     SCREEN(pause_hand_gadget_grid)}
};

struct MenuGridCell pause_hand_gadget_cells[6] = {
    {0xEA7A, 0, 0, 0x1A},
    {0xEA76, 0, 0, 0x16},
    {0xEA6C, 0, 0,  0xC},
    {0xEA80, 0, 0, 0x20},
    {0xEA7B, 0, 0, 0x1B},
    {0xEA7F, 0, 0, 0x1F},
};

struct MenuGridCell pause_pack_gadget_cells[3] = {
    {0xEA62, 0, 0, 2},
    {0xEA63, 0, 0, 3},
    {0xEA64, 0, 0, 4},
};

struct MenuGridCell pause_head_gadget_cells[3] = {
    {0xEA66, 0, 0, 6},
    {0xEA65, 0, 0, 5},
    {0xEA67, 0, 0, 7},
};

struct MenuGridCell pause_foot_gadget_cells[2] = {
    {0xEA7D, 0, 0, 0x1D},
    {0xEA7C, 0, 0, 0x1C},
};

struct MenuPage pause_weapons_page = {
    {0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x14, 0x20, 0x21, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_root_page,
    3,
    SCREEN(pause_weapon_grid),
    {SCREEN(pause_weapons_title),
     SCREEN(pause_hand_model_screen),
     SCREEN(pause_weapon_grid),
     SCREEN(item_preview_screen),
     SCREEN(pause_item_info_label),
     SCREEN(pause_item_preview_screen),
     &menu_placeholder_screen,
     SCREEN(pause_weapon_ammo_screen),
     SCREEN(equip_exit_hint_list)}
};

struct MenuLabelScreen pause_weapons_title = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4EE2}
};

struct MenuItemPreviewScreen pause_hand_model_screen = {
    {FUN_00224368, FUN_0021ddf8, FUN_002240c8, FUN_002242b8, 2}
};

struct MenuGridScreen pause_weapon_grid = {
    {FUN_0021b858, FUN_0021d948},
    {MENU_GRID_NO_EQUIPPED_FRAME, 0.08f, 0.08f, 0, 3, 5, pause_item_candidate_entries}
};

struct MenuPlainScreenC0 menu_noop_screen = {
    {FUN_0021bd98, FUN_0021df28}
};

struct MenuItemPreviewScreen pause_item_preview_screen = {
    {FUN_0021df98, FUN_0021e110, FUN_0021df30, FUN_0021df58, 2}
};

struct MenuPlainScreen68 pause_weapon_ammo_screen = {
    {FUN_0021bd98, FUN_0021f158}
};

struct MenuTextItem equip_exit_hint_items[3] = {
    {0x4EEC}, /* "Cross: Equip" */
    {0x4EE0}, /* "Triangle: Exit" */
    {0},
};

struct MenuTextListScreen equip_exit_hint_list = {
    {                                              0,          FUN_0021b1c8},
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST, equip_exit_hint_items}
};

struct MenuGridCell pause_item_candidate_entries[15] = {
    {0xEA6A, 0, 0,  0xA},
    {0xEA70, 0, 0, 0x10},
    {0xEA6F, 0, 0,  0xF},
    {0xEA74, 0, 0, 0x14},
    {0xEA71, 0, 0, 0x11},
    {0xEA6E, 0, 0,  0xE},
    {0xEA69, 0, 0,    9},
    {0xEA6B, 0, 0,  0xB},
    {0xEA72, 0, 0, 0x12},
    {0xEA6D, 0, 0,  0xD},
    {0xEA79, 0, 0, 0x19},
    {0xEA78, 0, 0, 0x18},
    {0xEA73, 0, 0, 0x13},
    {0xEA75, 0, 0, 0x15},
    {0xEA77, 0, 0, 0x17},
};

struct MenuPage pause_quick_select_page = {
    {0x1A, 0x1B, 0x2A, 0x2B, 0x1E, 0x1F, 0x14, 0x2C, 0x21, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_root_page,
    5,
    SCREEN(pause_quick_select_weapon_grid),
    {SCREEN(pause_quick_select_title),
     SCREEN(pause_quick_select_ring),
     SCREEN(pause_quick_select_weapon_grid),
     SCREEN(pause_quick_select_gadget_grid),
     SCREEN(pause_item_info_label),
     SCREEN(item_preview_screen),
     &menu_placeholder_screen,
     SCREEN(pause_weapon_ammo_screen),
     SCREEN(equip_exit_hint_list)}
};

struct MenuLabelScreen pause_quick_select_title = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4EE4}
};

struct MenuGridScreen pause_quick_select_weapon_grid = {
    {FUN_0021b858, FUN_0021d948},
    {0x20,
     0.08f, 0.08f,
     0, 3,
     5, pause_item_candidate_entries,
     SCREEN(pause_quick_select_gadget_grid),
     SCREEN(pause_quick_select_gadget_grid)}
};

struct MenuGridScreen pause_quick_select_gadget_grid = {
    {FUN_0021b858, FUN_0021d948},
    {0,
     0.1f, 0.1f,
     0, 2,
     3, pause_hand_gadget_cells,
     SCREEN(pause_quick_select_weapon_grid),
     SCREEN(pause_quick_select_weapon_grid)}
};

struct MenuItemSlotsScreen pause_quick_select_ring = {
    {FUN_0021c7a0, FUN_00220b20, FUN_0021c9c8, FUN_0021ca60}
};

struct MenuLabelScreen D_001CF3C0 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_SMALL_FONT | MENU_LABEL_FIXED_TEXT | MENU_LABEL_CENTER_Y | MENU_LABEL_CENTER_X,
     0x4EED}
};

struct MenuPage map_circle_destination_page = {
    {0x15, 0x16, 0x17, 0x18, 0x19, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    0xB,
    SCREEN(map_screen),
    {SCREEN(map_location_label),
     SCREEN(map_planet_label),
     SCREEN(map_screen),
     SCREEN(map_globe_screen),
     SCREEN(map_legend_screen)}
};

struct MenuLabelScreen map_location_label = {
    {                FUN_00219d80,    FUN_0021a328,FUN_0021a318},
    {MENU_LABEL_BY_SELECTED_LEVEL | MENU_LABEL_LARGE_FONT | MENU_LABEL_CENTER_Y |
MENU_LABEL_CENTER_X, (s32)D_001DDD4C,
     0xC     }
};

struct MenuLabelScreen map_planet_label = {
    {                                      FUN_00219d80,FUN_0021a328,FUN_0021a318},
    {MENU_LABEL_PLANET_PREFIX | MENU_LABEL_BY_SELECTED_LEVEL | MENU_LABEL_LARGE_FONT |
MENU_LABEL_CENTER_Y | MENU_LABEL_CENTER_X,
     (s32)D_001DDD50,
     0xC     }
};

struct MenuMapScreen map_screen = {
    {FUN_0021be60, FUN_0021f330, 0, FUN_0021bda0}
};

struct MenuMapScreen map_globe_screen = {
    {0, FUN_00220850, FUN_0021cac8}
};

struct MenuPlainScreen58 map_legend_screen = {
    {FUN_00219d80, FUN_0021f368}
};

struct MenuPage map_cross_destination_page = {
    {0x33, 0x34, 0x35, 0x36, 0x37, 5, 6, 0x38, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &map_circle_destination_page,
    0xC,
    &missions_list_screen,
    {SCREEN(missions_location_label),
     SCREEN(map_planet_label),
     SCREEN(missions_picture_screen),
     &missions_list_screen,
     SCREEN(exit_hint_list),
     &menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(missions_description_label)}
};

struct MenuStreamScreen missions_picture_screen = {
    {FUN_0021f990,                                 FUN_0021fc68, FUN_0021fce0, FUN_0021fd78},
    {  D_00138080, MENU_STREAM_UNPACK | MENU_STREAM_FIXED_ENTRY,          219,          175}
};

struct MenuLabelScreen missions_location_label = {
    {                FUN_00219d80,    FUN_0021a328,FUN_0021a318},
    {MENU_LABEL_BY_SELECTED_LEVEL | MENU_LABEL_LARGE_FONT | MENU_LABEL_CENTER_Y |
MENU_LABEL_CENTER_X, (s32)D_001DDD4C,
     0xC     }
};

struct MenuScreen missions_list_screen = {FUN_0021c4c0, FUN_0021f688, FUN_0021c420};

struct MenuLabelScreen missions_description_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_TEXT_523E | MENU_LABEL_SMALL_FONT | MENU_LABEL_FIXED_TEXT}
};

struct MenuPage pause_help_page = {
    {0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x14, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_root_page,
    7,
    SCREEN(pause_help_list),
    {SCREEN(pause_help_title),
     SCREEN(pause_help_description_label),
     SCREEN(pause_help_image),
     SCREEN(pause_help_list),
     SCREEN(select_exit_hint_list)}
};

struct MenuStreamScreen pause_help_image = {
    {FUN_0021fdc8,       FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_00138008, MENU_STREAM_UNPACK,          147,          133}
};

struct MenuLabelScreen pause_help_title = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F5E}
};

struct MenuTextItem pause_help_items[6] = {
    {0x4FD5, MENU_ACTION_SET_NEXT_PAGE, {(s32)&help_history_page}},
    {0x4EF1, MENU_ACTION_SET_NEXT_PAGE, {(s32)&controls_help_page}},
    {0x4EF2, MENU_ACTION_SET_NEXT_PAGE, {(s32)&moves_help_page}},
    {0x4EE2, MENU_ACTION_SET_NEXT_PAGE, {(s32)&weapon_help_page}},
    {0x4EE3, MENU_ACTION_SET_NEXT_PAGE, {(s32)&gadget_help_page}},
    {0},
};

s32 pause_help_descriptions[5] = {0x4FD6, 0x4EFD, 0x4EFE, 0x4EFF, 0x4F00};

struct MenuLabelScreen pause_help_description_label = {
    {                                        FUN_00219d80,                 FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_BY_LIST_SELECTION | MENU_LABEL_SMALL_FONT, (s32)pause_help_descriptions,            4}
};

struct MenuTextListScreen pause_help_list = {
    {  FUN_0021abf8,     FUN_0021b1c8},
    {MENU_LIST_WRAP, pause_help_items}
};

struct MenuTextItem select_exit_hint_items[3] = {
    {0x4F05}, /* "Cross: Select" */
    {0x4EE0}, /* "Triangle: Exit" */
    {0},
};

struct MenuTextListScreen select_exit_hint_list = {
    {                                              0,           FUN_0021b1c8},
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST, select_exit_hint_items}
};

struct MenuPage help_history_page = {
    {0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_help_page,
    0x15,
    SCREEN(help_history_list),
    {SCREEN(help_history_title),
     SCREEN(help_history_list),
     SCREEN(help_history_text_label),
     SCREEN(exit_hint_list)}
};

struct MenuLabelScreen help_history_title = {
    {                                                                             FUN_00219d80,FUN_0021a328},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4FD5  }
};

struct MenuTextItem help_history_menu_items[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52B9, MENU_ACTION_NO_OP},
    {0x52C0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextListScreen help_history_list = {
    {FUN_0021abf8, FUN_0021d4a8, FUN_0021d168, FUN_0021d1f0},
    {MENU_LIST_CENTERED | MENU_LIST_FIXED_STEP, help_history_menu_items}
};

struct MenuLabelScreen help_history_text_label = {
    {FUN_0021d338, FUN_0021a328, FUN_0021d1f8, FUN_0021d2c8},
    {MENU_LABEL_BY_HELP_ENTRY | MENU_LABEL_SMALL_FONT}
};

struct MenuPage controls_help_page = {
    {0x39, 0x3A, 0x3B, 0x3C, 0x3D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_help_page,
    0x16,
    SCREEN(controls_help_title),
    {SCREEN(controls_help_title),
     &menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(controls_help_pictures),
     SCREEN(exit_hint_list)}
};

struct MenuLabelScreen controls_help_title = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4EF1}
};

struct MenuStreamScreen controls_help_pictures = {
    {FUN_00220648, FUN_00220790, FUN_0021fce0, FUN_0021fd78}
};

struct MenuPage moves_help_page = {
    {0x3E, 0x3F, 0x40, 0x41, 0x42, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_help_page,
    0x17,
    SCREEN(moves_help_list),
    {SCREEN(moves_help_title),
     SCREEN(moves_help_list),
     SCREEN(moves_help_label),
     SCREEN(moves_help_image),
     SCREEN(exit_hint_list)}
};

struct MenuLabelScreen moves_help_title = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4EF2}
};

struct MenuTextItem moves_help_items[14] = {
    {0x4E8C, MENU_ACTION_NO_OP},
    {0x4E85, MENU_ACTION_NO_OP},
    {0x4E88, MENU_ACTION_NO_OP},
    {0x4F06, MENU_ACTION_NO_OP},
    {0x4F07, MENU_ACTION_NO_OP},
    {0x4F08, MENU_ACTION_NO_OP},
    {0x4F09, MENU_ACTION_NO_OP},
    {0x4F0A, MENU_ACTION_NO_OP},
    {0x4F0B, MENU_ACTION_NO_OP},
    {0x4F0C, MENU_ACTION_NO_OP},
    {0x4E87, MENU_ACTION_NO_OP},
    {0x4E86, MENU_ACTION_NO_OP},
    {0x4E8D, MENU_ACTION_NO_OP},
    {0},
};
