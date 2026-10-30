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
extern s32            D_001D0398[]; /* D_001D0358 list.selected */

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
            g->unkD8 ? (char *)&D_001CE798 : (char *)&pause_menu_chain_tail;
    }
    {
        struct MenuSystem *g = &menu_system;
        char              *menu_record_base = (char *)&pause_menu_chain_tail;
        *(char **)(menu_record_base + 0x3C) =
            g->unkD8 ? (char *)&D_001CE798 : (char *)&pause_menu_chain_head;
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
 * The pause pages of the menu page graph (rnc/ui/menus/menu_pages.h), from the
 * pause root page D_001CE488 to the help list help_history_menu_items. Types come
 * from the screens' draw functions; list lengths from their 0 terminators, grid
 * cells from rows * cols, label text tables from the list or grid they follow.
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

struct MenuScreen D_001CE2F8 = {
    0,
    0,
    0,
    0,
    4,
    0,
    0,
    0,
    0,
    0,
    {0},
    {.missions = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10, 0x11, 0x12}}}};

struct MenuTextItem D_001CE378[3] = {
    {0x4EDF},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen D_001CE3A0 = {
    {     0, FUN_0021b1c8},
    {0x4002,   D_001CE378}
};

struct MenuTextItem D_001CE3F0[2] = {
    {0x4EE0},
    {0},
};

struct MenuTextItem D_001CE408[4] = {
    {0x4EE0},
    {0x4EEF},
    {0x4F71},
    {0},
};

struct MenuTextListScreen D_001CE438 = {
    {     0, FUN_0021b1c8},
    {0x4002,   D_001CE3F0}
};

struct MenuPage D_001CE488 = {
    {0xE, 0xF, 0x10, 0x11, 0x12, 0x13, 0x14, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    2,
    SCREEN(pause_menu_chain_head),
    {SCREEN(pause_menu_chain_head),
     SCREEN(D_001CE608),
     SCREEN(D_001CE658),
     SCREEN(D_001CE6A8),
     SCREEN(D_001CE6F8),
     SCREEN(pause_menu_chain_tail),
     SCREEN(D_001CE798)}
};

struct MenuTextItem D_001CE510[2] = {
    {0x4EE2, 3, {(s32)&D_001CECB8}},
    {0},
};

struct MenuTextItem D_001CE528[2] = {
    {0x4EE3, 3, {(s32)&D_001CE7E8}},
    {0},
};

struct MenuTextItem D_001CE540[2] = {
    {0x4EE4, 3, {(s32)&D_001CF1B8}},
    {0},
};

struct MenuTextItem D_001CE558[2] = {
    {0x4EE5, 3, {(s32)&D_001D1BB8}},
    {0},
};

struct MenuTextItem D_001CE570[2] = {
    {0x4EE6, 3, {(s32)&D_001CF898}},
    {0},
};

struct MenuTextItem D_001CE588[2] = {
    {0x4EE7, 3, {(s32)&D_001D0850}},
    {0},
};

struct MenuTextItem D_001CE5A0[2] = {
    {0x4FC2, 3, {(s32)&D_001D2720}},
    {0},
};

struct MenuTextListScreen pause_menu_chain_head = {
    {FUN_0021abf8, FUN_0021b1c8, FUN_0021ddd0},
    {4, D_001CE510, 0, SCREEN(D_001CE608)}
};

struct MenuTextListScreen D_001CE608 = {
    {FUN_0021abf8, FUN_0021b1c8, FUN_0021ddd0},
    {4, D_001CE528, SCREEN(pause_menu_chain_head), SCREEN(D_001CE658)}
};

struct MenuTextListScreen D_001CE658 = {
    {FUN_0021abf8, FUN_0021b1c8, FUN_0021ddd0},
    {4, D_001CE540, SCREEN(D_001CE608), SCREEN(D_001CE6A8)}
};

struct MenuTextListScreen D_001CE6A8 = {
    {FUN_0021abf8, FUN_0021b1c8},
    {4, D_001CE558, SCREEN(D_001CE658), SCREEN(D_001CE6F8)}
};

struct MenuTextListScreen D_001CE6F8 = {
    {FUN_0021abf8, FUN_0021b1c8},
    {4, D_001CE570, SCREEN(D_001CE6A8), SCREEN(pause_menu_chain_tail)}
};

struct MenuTextListScreen pause_menu_chain_tail = {
    {FUN_0021abf8, FUN_0021b1c8},
    {4, D_001CE588, SCREEN(D_001CE6F8)}
};

struct MenuTextListScreen D_001CE798 = {
    {FUN_0021abf8, FUN_0021b1c8},
    {4, D_001CE5A0, SCREEN(pause_menu_chain_tail), SCREEN(pause_menu_chain_head)}
};

struct MenuPage D_001CE7E8 = {
    {              0x1A,0x1B,       0x22,0x23,       0x1E,0x1F,0x14,0x24,0x21,0x25,0x26,0x27,0x28,0x29           },
    &D_001CE488,
    4,
    SCREEN(D_001CEAE8),
    {SCREEN(D_001CE870),
     SCREEN(D_001CED98),
     SCREEN(D_001CEA80),
     SCREEN(D_001CEAE8),
     SCREEN(D_001CE8C8),
     SCREEN(D_001D0E28),
     &D_001CE2F8,
     SCREEN(D_001CEA28),
     SCREEN(D_001CF0D0),
     SCREEN(D_001CEB50),
     SCREEN(D_001CEBB8),
     SCREEN(D_001CE9D0),
     SCREEN(D_001CE978),
     SCREEN(D_001CE920)}
};

struct MenuLabelScreen D_001CE870 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4EE3}
};

struct MenuLabelScreen D_001CE8C8 = {
    {FUN_00219d80,        FUN_0021a328, FUN_0021a318},
    {           3, (s32)menu_item_info,         0x4C}
};

struct MenuLabelScreen D_001CE920 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0x12015, 0x4EE8}
};

struct MenuLabelScreen D_001CE978 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0x12015, 0x4EE9}
};

struct MenuLabelScreen D_001CE9D0 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0x12015, 0x4EEA}
};

struct MenuLabelScreen D_001CEA28 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0x12015, 0x4EEB}
};

struct MenuGridScreen D_001CEA80 = {
    {FUN_0021b858, FUN_0021d948},
    {0x20, 0.1f, 0.1f, 0, 2, 3, D_001CEC20, SCREEN(D_001CEBB8), SCREEN(D_001CEAE8)}
};

struct MenuGridScreen D_001CEAE8 = {
    {FUN_0021b858, FUN_0021d948, FUN_0021b7a8},
    {4, 0.1f, 0.1f, 0, 1, 3, D_001CEC60, SCREEN(D_001CEA80), SCREEN(D_001CEB50)}
};

struct MenuGridScreen D_001CEB50 = {
    {FUN_0021b858, FUN_0021d948, FUN_0021b7a8},
    {8, 0.1f, 0.1f, 0, 1, 3, D_001CEC80, SCREEN(D_001CEAE8), SCREEN(D_001CEBB8)}
};

struct MenuGridScreen D_001CEBB8 = {
    {FUN_0021b858, FUN_0021d948, FUN_0021b7a8},
    {0x10, 0.1f, 0.1f, 0, 1, 2, D_001CECA0, SCREEN(D_001CEB50), SCREEN(D_001CEA80)}
};

struct MenuGridCell D_001CEC20[6] = {
    {0xEA7A, 0, 0, 0x1A},
    {0xEA76, 0, 0, 0x16},
    {0xEA6C, 0, 0,  0xC},
    {0xEA80, 0, 0, 0x20},
    {0xEA7B, 0, 0, 0x1B},
    {0xEA7F, 0, 0, 0x1F},
};

struct MenuGridCell D_001CEC60[3] = {
    {0xEA62, 0, 0, 2},
    {0xEA63, 0, 0, 3},
    {0xEA64, 0, 0, 4},
};

struct MenuGridCell D_001CEC80[3] = {
    {0xEA66, 0, 0, 6},
    {0xEA65, 0, 0, 5},
    {0xEA67, 0, 0, 7},
};

struct MenuGridCell D_001CECA0[2] = {
    {0xEA7D, 0, 0, 0x1D},
    {0xEA7C, 0, 0, 0x1C},
};

struct MenuPage D_001CECB8 = {
    {0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x14, 0x20, 0x21, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001CE488,
    3,
    SCREEN(D_001CEE58),
    {SCREEN(D_001CED40),
     SCREEN(D_001CED98),
     SCREEN(D_001CEE58),
     SCREEN(D_001D0E28),
     SCREEN(D_001CE8C8),
     SCREEN(D_001CEF80),
     &D_001CE2F8,
     SCREEN(D_001CF040),
     SCREEN(D_001CF0D0)}
};

struct MenuLabelScreen D_001CED40 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4EE2}
};

struct MenuItemPreviewScreen D_001CED98 = {
    {FUN_00224368, FUN_0021ddf8, FUN_002240c8, FUN_002242b8, 2}
};

struct MenuGridScreen D_001CEE58 = {
    {FUN_0021b858, FUN_0021d948},
    {0x20, 0.08f, 0.08f, 0, 3, 5, pause_item_candidate_entries}
};

struct MenuPlainScreenC0 D_001CEEC0 = {
    {FUN_0021bd98, FUN_0021df28}
};

struct MenuItemPreviewScreen D_001CEF80 = {
    {FUN_0021df98, FUN_0021e110, FUN_0021df30, FUN_0021df58, 2}
};

struct MenuPlainScreen68 D_001CF040 = {
    {FUN_0021bd98, FUN_0021f158}
};

struct MenuTextItem D_001CF0A8[3] = {
    {0x4EEC},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen D_001CF0D0 = {
    {     0, FUN_0021b1c8},
    {0x4002,   D_001CF0A8}
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

struct MenuPage D_001CF1B8 = {
    {0x1A, 0x1B, 0x2A, 0x2B, 0x1E, 0x1F, 0x14, 0x2C, 0x21, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001CE488,
    5,
    SCREEN(D_001CF298),
    {SCREEN(D_001CF240),
     SCREEN(D_001CF368),
     SCREEN(D_001CF298),
     SCREEN(D_001CF300),
     SCREEN(D_001CE8C8),
     SCREEN(D_001D0E28),
     &D_001CE2F8,
     SCREEN(D_001CF040),
     SCREEN(D_001CF0D0)}
};

struct MenuLabelScreen D_001CF240 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4EE4}
};

struct MenuGridScreen D_001CF298 = {
    {FUN_0021b858, FUN_0021d948},
    {0x20,
     0.08f, 0.08f,
     0, 3,
     5, pause_item_candidate_entries,
     SCREEN(D_001CF300),
     SCREEN(D_001CF300)}
};

struct MenuGridScreen D_001CF300 = {
    {FUN_0021b858, FUN_0021d948},
    {0, 0.1f, 0.1f, 0, 2, 3, D_001CEC20, SCREEN(D_001CF298), SCREEN(D_001CF298)}
};

struct MenuItemSlotsScreen D_001CF368 = {
    {FUN_0021c7a0, FUN_00220b20, FUN_0021c9c8, FUN_0021ca60}
};

struct MenuLabelScreen D_001CF3C0 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0x17, 0x4EED}
};

struct MenuPage map_circle_destination_page = {
    {0x15, 0x16, 0x17, 0x18, 0x19, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    0xB,
    SCREEN(D_001CF550),
    {SCREEN(D_001CF4A0),
     SCREEN(D_001CF4F8),
     SCREEN(D_001CF550),
     SCREEN(D_001CF5B8),
     SCREEN(D_001CF620)}
};

struct MenuLabelScreen D_001CF4A0 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {        0x4B, (s32)D_001DDD4C,          0xC}
};

struct MenuLabelScreen D_001CF4F8 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {       0x24B, (s32)D_001DDD50,          0xC}
};

struct MenuMapScreen D_001CF550 = {
    {FUN_0021be60, FUN_0021f330, 0, FUN_0021bda0}
};

struct MenuMapScreen D_001CF5B8 = {
    {0, FUN_00220850, FUN_0021cac8}
};

struct MenuPlainScreen58 D_001CF620 = {
    {FUN_00219d80, FUN_0021f368}
};

struct MenuPage map_cross_destination_page = {
    {0x33, 0x34, 0x35, 0x36, 0x37, 5, 6, 0x38, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &map_circle_destination_page,
    0xC,
    &D_001CF7C0,
    {SCREEN(D_001CF768),
     SCREEN(D_001CF4F8),
     SCREEN(D_001CF700),
     &D_001CF7C0,
     SCREEN(D_001CE438),
     &D_001CE2F8,
     &D_001CE2F8,
     SCREEN(D_001CF840)}
};

struct MenuStreamScreen D_001CF700 = {
    {FUN_0021f990, FUN_0021fc68, FUN_0021fce0, FUN_0021fd78},
    {  D_00138080,         0x21,         0xDB,         0xAF}
};

struct MenuLabelScreen D_001CF768 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {        0x4B, (s32)D_001DDD4C,          0xC}
};

struct MenuScreen D_001CF7C0 = {FUN_0021c4c0, FUN_0021f688, FUN_0021c420};

struct MenuLabelScreen D_001CF840 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0x4014}
};

struct MenuPage D_001CF898 = {
    {0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x14, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001CE488,
    7,
    SCREEN(D_001CFA98),
    {SCREEN(D_001CF988),
     SCREEN(D_001CFA40),
     SCREEN(D_001CF920),
     SCREEN(D_001CFA98),
     SCREEN(D_001CFB10)}
};

struct MenuStreamScreen D_001CF920 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_00138008,         0x20,         0x93,         0x85}
};

struct MenuLabelScreen D_001CF988 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {0xF, 0x4F5E}
};

struct MenuTextItem D_001CF9E0[6] = {
    {0x4FD5, 3, {(s32)&D_001CFB60}},
    {0x4EF1, 3, {(s32)&D_001D0400}},
    {0x4EF2, 3, {(s32)&D_001D0548}},
    {0x4EE2, 3, {(s32)&D_001D0C20}},
    {0x4EE3, 3, {(s32)&D_001D0FA8}},
    {0},
};

s32 D_001CFA28[5] = {0x4FD6, 0x4EFD, 0x4EFE, 0x4EFF, 0x4F00};

struct MenuLabelScreen D_001CFA40 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {        0x90, (s32)D_001CFA28,            4}
};

struct MenuTextListScreen D_001CFA98 = {
    {FUN_0021abf8, FUN_0021b1c8},
    {      0x1000,   D_001CF9E0}
};

struct MenuTextItem D_001CFAE8[3] = {
    {0x4F05},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen D_001CFB10 = {
    {     0, FUN_0021b1c8},
    {0x4002,   D_001CFAE8}
};

struct MenuPage D_001CFB60 = {
    {0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001CF898,
    0x15,
    SCREEN(D_001D0358),
    {SCREEN(D_001CFBE8), SCREEN(D_001D0358), SCREEN(D_001D03A8), SCREEN(D_001CE438)}
};

struct MenuLabelScreen D_001CFBE8 = {
    {FUN_00219d80, FUN_0021a328},
    {         0xF,       0x4FD5}
};

struct MenuTextItem help_history_menu_items[151] = {
    {0x5277, 1},
    {0x5278, 1},
    {0x52B9, 1},
    {0x52C0, 1},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
};

struct MenuTextListScreen D_001D0358 = {
    {FUN_0021abf8, FUN_0021d4a8, FUN_0021d168, FUN_0021d1f0},
    {0x410, help_history_menu_items}
};

struct MenuLabelScreen D_001D03A8 = {
    {FUN_0021d338, FUN_0021a328, FUN_0021d1f8, FUN_0021d2c8},
    {0x1010}
};

struct MenuPage D_001D0400 = {
    {0x39, 0x3A, 0x3B, 0x3C, 0x3D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001CF898,
    0x16,
    SCREEN(D_001D0488),
    {SCREEN(D_001D0488), &D_001CE2F8, &D_001CE2F8, SCREEN(D_001D04E0), SCREEN(D_001CE438)}
};

struct MenuLabelScreen D_001D0488 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {0xF, 0x4EF1}
};

struct MenuStreamScreen D_001D04E0 = {
    {FUN_00220648, FUN_00220790, FUN_0021fce0, FUN_0021fd78}
};

struct MenuPage D_001D0548 = {
    {0x3E, 0x3F, 0x40, 0x41, 0x42, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001CF898,
    0x17,
    SCREEN(D_001D0740),
    {SCREEN(D_001D05D0),
     SCREEN(D_001D0740),
     SCREEN(D_001D07F8),
     SCREEN(D_001D0790),
     SCREEN(D_001CE438)}
};

struct MenuLabelScreen D_001D05D0 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4EF2}
};

struct MenuTextItem D_001D0628[14] = {
    {0x4E8C, 1},
    {0x4E85, 1},
    {0x4E88, 1},
    {0x4F06, 1},
    {0x4F07, 1},
    {0x4F08, 1},
    {0x4F09, 1},
    {0x4F0A, 1},
    {0x4F0B, 1},
    {0x4F0C, 1},
    {0x4E87, 1},
    {0x4E86, 1},
    {0x4E8D, 1},
    {0},
};
