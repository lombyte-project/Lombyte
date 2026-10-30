#include "types.h"
#include "rnc/ui/menus/panel_slots.h"
#include "sda.h"
#include "qcopy.h"
#include "qzero.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/rendering/render_globals.h"

struct P {
    u8 pad_0[0x10];
    u8 f10;
};

struct T {
    u8        pad_0[0x48];
    struct P *tbl[1];
};

struct O2 {
    u8        pad_0[0x10];
    float     f10, f14, f18;
    u8        pad_1C[8];
    struct T *f24;
    u8        pad_28[0xC];
    u16       f34;
    u8        pad_36[0xA];
    s32       f40, f44, f48;
    u8        pad_4C[0x28];
    s32      *f74;
};

#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_pages.h"

extern u8   D_0019C150[];
extern u8   D_0019C160[];
extern void quad_moby_update() __asm__("FUN_0023a2c0");

#include "rnc/ui/menus/menu_globals.h"
#include "rnc/ui/menus/initialize_menu_preview_scene_globals.h"
#include "rnc/math/fast_math.h"

extern u8         menu_preview_lighting_scale_source_gp __asm__("D_001601C0") __attribute__((sda));
extern u8         menu_preview_lighting_copy_source_gp __asm__("D_001601D0") __attribute__((sda));
extern void       vu1_sync_chain(s32) __asm__("func_002337B0");
extern s32        sceGsSyncV(s32);
extern s32        initialize_menu_stream_buffers() __asm__("FUN_00225ac0");
extern void       vu1_init_chain(void) __asm__("func_002335D0");
extern struct O2 *create_menu_preview_moby() __asm__("func_00225490");
extern s32        set_moby_animation() __asm__("func_00212ED8");

void func_00225AC0__void() __asm__("FUN_00225ac0");
void initialize_menu_preview_scene(void) __asm__("FUN_00218f98");

void initialize_menu_preview_scene(void) {
    s32 i;
    s32 preview_workspace_start, graphics_workspace_base, help_text_buffer_start,
        graphics_buffer_start;
    menu_system.state = 2;
    menu_system.current = &menu_preview_initial_page;
    menu_system.next = &menu_preview_initial_page;
    menu_system.close_locked = 0;
    fast_vec_scale((s32)D_0019C150, (s32)&menu_preview_lighting_scale_source_gp, 1.0f);
    qcopy(D_0019C150 - 0x10, &menu_preview_lighting_copy_source_gp);
    qzero(D_0019C150 + 0x20);
    qzero(D_0019C160);
    vu1_sync_chain(1);
    sceGsSyncV(0);
    game_frame_counter++;
    preview_workspace_start = D_001940C0[1] + 0xA0000;
    graphics_workspace_base = D_001940C0[2] + 0xA0000;
    help_text_buffer_start = preview_workspace_start + 0x3C000;
    graphics_buffer_start = graphics_workspace_base + 0xE0000;
    menu_system.unkFC = help_text_buffer_start + 0xC1000;
    menu_system.unk100 = graphics_buffer_start + 0x11800;
    current_vu_chain = 0xA0000;
    menu_system.unk104 = preview_workspace_start;
    menu_system.unk10 = graphics_workspace_base;
    menu_system.help_text_buffer = help_text_buffer_start;
    menu_system.unk10C = graphics_buffer_start;
    func_00225AC0__void(1);
    vu1_init_chain();
    menu_system.saved_texture_start = gs_texture_allocation_start;
    if (menu_system.current != 0) {
        for (i = 0; i < 14; i++) {
            struct O2 *preview_moby = (struct O2 *)create_menu_preview_moby(0x472);
            panel_slots[i] = preview_moby;
            if (preview_moby != 0) {
                s32 k;
                preview_moby->f34 &= 0xFFFD;
                ((struct O2 *)panel_slots[i])->f74 = quad_moby_update;
                ((struct O2 *)panel_slots[i])->f10 = preview_camera.position.f[0];
                ((struct O2 *)panel_slots[i])->f14 = preview_camera.position.f[1];
                ((struct O2 *)panel_slots[i])->f18 = preview_camera.position.f[2];
                ((struct O2 *)panel_slots[i])->f40 = 0;
                ((struct O2 *)panel_slots[i])->f44 = 0;
                ((struct O2 *)panel_slots[i])->f48 = 0;
                k = menu_system.current->moby_anims[i];
                set_moby_animation(
                    panel_slots[i], k, ((struct O2 *)panel_slots[i])->f24->tbl[k]->f10 - 1);
            }
        }
    }
}

extern __typeof__(initialize_menu_preview_scene) func_00218F98
    __attribute__((alias("FUN_00218f98")));

/*
 * The menu page graph from 0x1D0740 to 0x1D5020 (rnc/ui/menus/menu_pages.h):
 * every page reachable from menu_preview_initial_page, the page this function
 * makes current and next, with their screens and lists. Types come from the
 * screens' draw functions; list lengths from their 0 terminators, grid cells
 * from rows * cols, label text tables from the list or grid they follow.
 */
#define SCREEN(screen) ((struct MenuScreen *)&(screen))

extern s32  FUN_00219d80(struct MenuScreen *);
extern s32  FUN_00219e10(struct MenuScreen *);      /* update_menu_icon_list */
extern s32  FUN_00219fa0(struct MenuScreen *);      /* draw_menu_icon_list */
extern void FUN_0021a1b0(struct MenuScreen *, s32); /* set_pause_action_list */
extern void FUN_0021a1e0(struct MenuScreen *, s32); /* rebuild_configured_text_label_list */
extern void FUN_0021a308(struct MenuScreen *, s32); /* enter_menu_choice_list */
extern void FUN_0021a310(struct MenuScreen *, s32); /* leave_menu_choice_list */
extern void FUN_0021a318(struct MenuScreen *, s32); /* clear_menu_selected_index */
extern s32  FUN_0021a328(struct MenuScreen *);      /* render_configured_text_label */
extern s32  FUN_0021abf8(struct MenuScreen *);      /* update_menu_entry_actions */
extern s32  FUN_0021b1c8(struct MenuScreen *);      /* render_localized_ui_entry_list */
extern s32  FUN_0021b858(struct MenuScreen *);      /* update_menu_grid_selection */
extern void FUN_0021bda0(struct MenuScreen *, s32); /* leave_map_screen */
extern s32  FUN_0021be60(struct MenuScreen *);      /* draw_map_screen */
extern void FUN_0021c420(struct MenuScreen *, s32); /* enter_menu_mission_list */
extern s32  FUN_0021c4c0(struct MenuScreen *);      /* update_menu_mission_list */
extern void FUN_0021ca98(struct MenuScreen *, s32); /* assign_heli_pack_label_text_table */
extern void FUN_0021cac8(struct MenuScreen *, s32); /* reset_pause_memory_card_state */
extern void FUN_0021cae0(struct MenuScreen *, s32); /* reset_menu_graphics_buffers */
extern void FUN_0021cb00(struct MenuScreen *, s32); /* set_pause_sound_volume */
extern s32  FUN_0021cb30(struct MenuScreen *);      /* sound_options_menu */
extern s32  FUN_0021ce00(struct MenuScreen *);      /* draw_sound_menu */
extern s32  FUN_0021d4a8(struct MenuScreen *);      /* draw_menu_text_list */
extern s32  FUN_0021d948(struct MenuScreen *);      /* draw_menu_item_grid */
extern void FUN_0021df30(struct MenuScreen *, s32); /* enter_item_preview */
extern void FUN_0021df58(struct MenuScreen *, s32); /* clear_menu_preview_pair */
extern s32  FUN_0021e230(struct MenuScreen *);      /* update_item_preview_binding */
extern s32  FUN_0021e608(struct MenuScreen *);      /* draw_available_item_preview_mobys */
extern s32  FUN_0021e7c8(struct MenuScreen *);
extern s32  FUN_0021e890(struct MenuScreen *);      /* draw_quit_game_menu */
extern void FUN_0021ea48(struct MenuScreen *, s32); /* create_rotating_preview_moby */
extern void FUN_0021eaf0(struct MenuScreen *, s32); /* release_item_preview_moby */
extern s32  FUN_0021eb20(struct MenuScreen *);      /* draw_items_menu */
extern s32  FUN_0021ef78(struct MenuScreen *);      /* setup_menu_display_text */
extern s32  FUN_0021f5f8(struct MenuScreen *);      /* draw_mission_menu_labels */
extern s32  FUN_0021f688(struct MenuScreen *);      /* draw_missions_menu2 */
extern s32  FUN_0021f990(struct MenuScreen *);      /* update_menu_stream_image */
extern s32  FUN_0021fc68(struct MenuScreen *);      /* draw_transition_overlay */
extern void FUN_0021fce0(struct MenuScreen *, s32); /* enter_menu_stream_image */
extern void FUN_0021fd78(struct MenuScreen *, s32); /* leave_menu_stream_image */
extern s32  FUN_0021fdc8(struct MenuScreen *);      /* update_menu_resource_stream */
extern s32  FUN_00220348(struct MenuScreen *);      /* draw_checking_memory_card_data_menu */
extern s32  FUN_00220850(struct MenuScreen *);      /* draw_map_screen_layers */
extern s32  FUN_00220e28(struct MenuScreen *);      /* update_menu_option_list */
extern s32  FUN_00221030(struct MenuScreen *);      /* draw_cheats_menu */
extern s32  FUN_002212b8(struct MenuScreen *);      /* update_menu_choice_list */
extern s32  FUN_00221460(struct MenuScreen *);      /* draw_menu_choice_list */
extern void FUN_002215f8(struct MenuScreen *, s32); /* build_pause_item_list */
extern void FUN_002216c0(struct MenuScreen *, s32); /* build_filtered_menu_item_list */
extern void FUN_002217f8(struct MenuScreen *, s32); /* pause_page_enter_noop */
extern void FUN_00221800(struct MenuScreen *, s32);
extern s32  FUN_00221908(struct MenuScreen *);      /* update_level_selection_map_screen */
extern s32  FUN_00221930(struct MenuScreen *);      /* draw_level_selection_map_screen */
extern s32  FUN_00221968(struct MenuScreen *);      /* handle_menu_action_buttons */
extern void FUN_00221a48(struct MenuScreen *, s32); /* enter_menu_prompt */
extern void FUN_00221a88(struct MenuScreen *, s32); /* leave_menu_prompt */
extern s32  FUN_00221ab8(struct MenuScreen *);
extern s32  FUN_00221b50(struct MenuScreen *);      /* draw_prompt_box */
extern void FUN_00221d28(struct MenuScreen *, s32); /* assign_pause_level_sprite_list */
extern s32  FUN_00221d68(struct MenuScreen *);      /* handle_cyclic_menu_selection_input */
extern s32  FUN_00221e50(struct MenuScreen *);      /* update_menu_cycle_selection */
extern s32  FUN_00221f58(struct MenuScreen *);      /* draw_menu_side_indicator */
extern s32  FUN_002220f0(struct MenuScreen *);      /* draw_selected_menu_item_text */
extern s32  FUN_00222290(struct MenuScreen *);      /* process_global_state_flags */
extern void FUN_002223d8(struct MenuScreen *, s32); /* enter_timed_menu_sequence */
extern s32  FUN_002223f0(struct MenuScreen *);      /* update_timed_menu_sequence */
extern s32  FUN_00222768(struct MenuScreen *);      /* draw_menu_text_box */
extern s32  FUN_00222948(struct MenuScreen *);      /* obtain_all_gold_weapons_menu */
extern s32  FUN_00222a98(struct MenuScreen *);      /* draw_localized_three_option_menu */
extern s32  FUN_00222d98(struct MenuScreen *);      /* draw_menu_selection_text */
extern void FUN_00222f18(struct MenuScreen *, s32); /* enter_save_slot_screen */
extern void FUN_00222f58(struct MenuScreen *, s32); /* leave_save_slot_screen */
extern s32  FUN_00222f88(struct MenuScreen *);      /* saving_data_menu */
extern s32  FUN_002232d8(struct MenuScreen *);      /* loading_data_menu */
extern s32  FUN_002235b8(struct MenuScreen *);      /* saving_data_menu2 */
extern s32  FUN_002239e0(struct MenuScreen *);      /* draw_save_slot_list */
extern void FUN_00225588(struct MenuScreen *, s32); /* enter_streamed_moby_screen */
extern void FUN_00225660(struct MenuScreen *, s32); /* leave_streamed_moby_screen */
extern s32  FUN_002256e8(struct MenuScreen *);      /* update_streamed_moby_animation */
extern s32  FUN_00225a68(struct MenuScreen *);      /* draw_moby_entries_from_object */

extern struct MenuStreamEntry D_00137EA8[];
extern struct MenuStreamEntry D_00137F20[];
extern struct MenuStreamEntry D_00137F98[];
extern struct MenuStreamEntry D_00138040[];
extern struct MenuStreamEntry D_00138308[];
extern struct MenuStreamEntry D_001384D0[];
extern struct MenuStreamEntry D_00138520[];
extern struct MenuStreamEntry D_001385B8[];
extern struct MenuStreamEntry D_00138650[];
extern struct MenuStreamEntry D_00138748[];
extern struct MenuStreamEntry D_00138928[];
extern struct MenuStreamEntry D_00138A18[];
extern struct MenuStreamEntry D_00138A38[];
extern u8                     D_0015EDDC[];
extern u8                     D_0015EDE0[];
extern u8                     D_0015EDE4[];
extern u8                     D_0015EE40[];
extern s32                    D_001601E8[];
extern struct MenuTextItem    D_001601F0[];
extern struct MenuTextItem    D_00160200[];
extern struct MenuIcon        D_001D60E0[];
extern u8                     D_001D6178[];
extern struct MenuIcon        D_001D61B8[];
extern u8                     D_001D6248[];
extern u8                     D_001D6280[];
extern struct MenuTextItem    D_001D62B8[];

struct MenuTextListScreen D_001D0740 = {
    {FUN_0021abf8, FUN_0021b1c8},
    {      0x1000,   D_001D0628}
};

struct MenuStreamScreen D_001D0790 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_00137EA8,         0x20,        0x100,         0xD0}
};

struct MenuLabelScreen D_001D07F8 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021ca98},
    {        0x90,            0,            4}
};

struct MenuPage D_001D0850 = {
    {0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x14, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001CE488,
    8,
    SCREEN(D_001D09F8),
    {SCREEN(D_001D08D8),
     SCREEN(D_001D0A68),
     SCREEN(D_001D0930),
     SCREEN(D_001D09F8),
     SCREEN(D_001CFB10)}
};

struct MenuLabelScreen D_001D08D8 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {0x200F, 0x4F1D}
};

struct MenuStreamScreen D_001D0930 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_00138040,         0x20,         0x93,         0x85}
};

struct MenuTextItem D_001D0998[8] = {
    {0x4F1E, 3, {(s32)&D_001D0AC0}},
    {0x4F1F, 4, {0x1D15C8}},
    {0x4F20, 5, {0x1D1650}},
    {0x4F22, 3, {(s32)&D_001D1498}},
    {0x4F23, 3, {(s32)&D_001D1960}},
    {0x4F24, 3, {(s32)&D_001D1208}},
    {0x4F26, 3, {(s32)&D_001D1AE0}},
    {0},
};

struct MenuTextListScreen D_001D09F8 = {
    {FUN_0021abf8, FUN_0021b1c8},
    {      0x1000,   D_001D0998}
};

s32 D_001D0A48[7] = {0x4F31, 0x4F32, 0x4F33, 0x4F35, 0x4F36, 0x4F37, 0x4F39};

struct MenuLabelScreen D_001D0A68 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {        0x90, (s32)D_001D0A48,            4}
};

struct MenuPage D_001D0AC0 = {
    {0x49, 0x4A, 0x4B, 0x4C, 0x4D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D0850,
    0x13,
    SCREEN(D_001D0BE0),
    {SCREEN(D_001D0B48), &D_001CE2F8, &D_001CE2F8, SCREEN(D_001D0BE0), SCREEN(D_001CE3A0)}
};

struct MenuLabelScreen D_001D0B48 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4F40}
};

struct MenuOption D_001D0BA0[3] = {
    {0x4F9F, &D_0015EE1C, 0x4F5A, 0x4F5B},
    {0x4FA0, &D_0015EE1D, 0x4F5A, 0x4F5B},
    {0},
};

struct MenuOptionListScreen D_001D0BE0 = {
    {FUN_00220e28, FUN_00221030},
    {           0,   D_001D0BA0}
};

struct MenuPage D_001D0C20 = {
    {0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 6, 0x61, 0x62, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001CF898,
    0x18,
    SCREEN(D_001D0D00),
    {SCREEN(D_001D0CA8),
     SCREEN(D_001D0D00),
     SCREEN(D_001D0D68),
     SCREEN(D_001D0E28),
     SCREEN(D_001CEF80),
     SCREEN(D_001CE8C8),
     &D_001CE2F8,
     SCREEN(D_001CE438),
     SCREEN(D_001D0DD0)}
};

struct MenuLabelScreen D_001D0CA8 = {
    {FUN_00219d80, FUN_0021a328, FUN_002215f8},
    {0xF, 0x4F99}
};

struct MenuIconListScreen D_001D0D00 = {
    {FUN_00219e10, FUN_00219fa0},
    {{0}, 0, 0, {0}, D_001D60E0}
};

struct MenuStreamScreen D_001D0D68 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_00137F20,         0x28,         0xEE,         0xD5}
};

struct MenuLabelScreen D_001D0DD0 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {       0x110, (s32)D_001D6178,            4}
};

struct MenuItemPreviewScreen D_001D0E28 = {
    {FUN_0021e230, FUN_0021e608, FUN_0021df30, FUN_0021df58, 2}
};

struct MenuItemPreviewScreen D_001D0EE8 = {
    {FUN_0021e230, FUN_0021e608, FUN_0021df30, FUN_0021df58, 2},
    {1}
};

struct MenuPage D_001D0FA8 = {
    {0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 6, 0x74, 0x75, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001CF898,
    0x19,
    SCREEN(D_001D1088),
    {SCREEN(D_001D1030),
     SCREEN(D_001D1088),
     SCREEN(D_001D10F0),
     SCREEN(D_001D0EE8),
     SCREEN(D_001D11B0),
     SCREEN(D_001CE8C8),
     &D_001CE2F8,
     SCREEN(D_001CE438),
     SCREEN(D_001D1158)}
};

struct MenuLabelScreen D_001D1030 = {
    {FUN_00219d80, FUN_0021a328, FUN_002216c0},
    {0xF, 0x4F9A}
};

struct MenuIconListScreen D_001D1088 = {
    {FUN_00219e10, FUN_00219fa0},
    {{0}, 0, 0, {0}, D_001D61B8}
};

struct MenuStreamScreen D_001D10F0 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_00137F98,         0x28,         0xEE,         0x9F}
};

struct MenuLabelScreen D_001D1158 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {       0x110, (s32)D_001D6248,            4}
};

struct MenuLabelScreen D_001D11B0 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {       0x103, (s32)D_001D6280,            4}
};

struct MenuPage D_001D1208 = {
    {0x49, 0x4A, 0x4B, 0x4C, 0x4D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D0850,
    0x1B,
    SCREEN(D_001D1310),
    {SCREEN(D_001D1290), &D_001CE2F8, &D_001CE2F8, SCREEN(D_001D1310), SCREEN(D_001CE3A0)}
};

struct MenuLabelScreen D_001D1290 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4F6F}
};

struct MenuOption D_001D12E8[2] = {
    {0x4F24, D_0015EE40, 0x4F5A, 0x4F5B},
    {0},
};

struct MenuOptionListScreen D_001D1310 = {
    {FUN_00220e28, FUN_00221030},
    {           0,   D_001D12E8}
};

struct MenuPage D_001D1350 = {
    {0x76, 0x77, 0x78, 0x7B, 0x7A, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D0850,
    0x1C,
    SCREEN(D_001D1458),
    {SCREEN(D_001D13D8), &D_001CE2F8, &D_001CE2F8, SCREEN(D_001D1458), SCREEN(D_001CE3A0)}
};

struct MenuLabelScreen D_001D13D8 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4F6F}
};

struct MenuOption D_001D1430[2] = {
    {0x4FA1, 0, 0x4F5A, 0x4F5B},
    {0},
};

struct MenuOptionListScreen D_001D1458 = {
    {FUN_00220e28, FUN_00221030},
    {           0,   D_001D1430}
};

struct MenuPage D_001D1498 = {
    {0x76, 0xD4, 0xD5, 0xD6, 0xD7, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D0850,
    0x20,
    SCREEN(D_001D1578),
    {SCREEN(D_001D1520), &D_001CE2F8, &D_001CE2F8, SCREEN(D_001D1578), SCREEN(D_001CE3A0)}
};

struct MenuLabelScreen D_001D1520 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4F43}
};

struct MenuPlainScreen50 D_001D1578 = {
    {FUN_0021cb30, FUN_0021ce00, 0, FUN_0021cb00}
};

struct MenuPage D_001D15C8 = {
    {0x7C, 0x7D, 0x7E, 0x7F, 0x80, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D0850,
    0x1E,
    SCREEN(D_001D17E0),
    {SCREEN(D_001D16D8),
     SCREEN(D_001D17E0),
     SCREEN(D_001D18F8),
     SCREEN(D_001D1788),
     SCREEN(D_001D18A8)}
};

struct MenuPage D_001D1650 = {
    {0x7C, 0x7D, 0x7E, 0x7F, 0x80, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D0850,
    0x1F,
    SCREEN(D_001D1830),
    {SCREEN(D_001D1730),
     SCREEN(D_001D1830),
     SCREEN(D_001D18F8),
     SCREEN(D_001D1788),
     SCREEN(D_001D18A8)}
};

struct MenuLabelScreen D_001D16D8 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4F29}
};

struct MenuLabelScreen D_001D1730 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4F28}
};

struct MenuPlainScreen58 D_001D1788 = {
    {0, FUN_00222d98}
};

struct MenuSaveSlotScreen D_001D17E0 = {
    {FUN_00222f88, FUN_002239e0, FUN_00222f18, FUN_00222f58}
};

struct MenuSaveSlotScreen D_001D1830 = {
    {FUN_002232d8, FUN_002239e0, FUN_00222f18, FUN_00222f58}
};

struct MenuTextItem D_001D1880[3] = {
    {0x4F27},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen D_001D18A8 = {
    {     0, FUN_0021b1c8},
    {0x4002,   D_001D1880}
};

struct MenuStreamScreen D_001D18F8 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_00138308,        0x120,        0x132,         0x6C}
};

struct MenuPage D_001D1960 = {
    {0x76, 0x77, 0x78, 0x79, 0x7A, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D0850,
    0x1D,
    SCREEN(D_001D1AA0),
    {SCREEN(D_001D19E8), &D_001CE2F8, &D_001CE2F8, SCREEN(D_001D1AA0), SCREEN(D_001CE3A0)}
};

struct MenuLabelScreen D_001D19E8 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4F42}
};

struct MenuChoice D_001D1A40[4] = {
    {0x4FA2, D_0015EDE0, {0x4FA5, 0x4FA4}},
    {0x4FA3, D_0015EDDC, {0x4FA5, 0x4FA4}},
    {0x521B, D_0015EDE4, {0x521A, 0x5219, 0x5218}},
    {0},
};

struct MenuChoiceListScreen D_001D1AA0 = {
    {FUN_002212b8, FUN_00221460, FUN_0021a308, FUN_0021a310},
    {{0}, D_001D1A40}
};

struct MenuPage D_001D1AE0 = {
    {0x4E, 0x4F, 0x50, 0x51, 0x52, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D0850,
    0xD,
    SCREEN(D_001D1B68),
    {&D_001CE2F8, &D_001CE2F8, &D_001CE2F8, SCREEN(D_001D1B68)}
};

struct MenuPlainScreen50 D_001D1B68 = {
    {FUN_0021e7c8, FUN_0021e890}
};

struct MenuPage D_001D1BB8 = {
    {0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x14, 0x5A, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001CE488,
    6,
    SCREEN(D_001D1D60),
    {SCREEN(D_001D1C40),
     SCREEN(D_001D1D60),
     SCREEN(D_001D1DC8),
     SCREEN(D_001D1EB0),
     SCREEN(D_001D1C98),
     SCREEN(D_001CE438),
     &D_001CE2F8,
     SCREEN(D_001D1F30)}
};

struct MenuLabelScreen D_001D1C40 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4EE5}
};

struct MenuStreamScreen D_001D1C98 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_00138A38,         0x24,         0xBB,         0x5E}
};

struct MenuGridCell D_001D1D00[9] = {
    {0xEA7E, 0, 0, 0x1E},
    {0xEAC9, 0, 0, 0x23},
    {0xEAC9, 2, 0, 0x22},
    {0xEAC9, 3, 1, 4},
    {0xEAC9, 4, 1, 5},
    {0xEAC9, 1, 0, 0x21},
    {0xEAC8, 0, 1},
    {0xEAC8, 1, 1, 1},
    {0xEAC8, 2, 1, 2},
};

struct MenuGridScreen D_001D1D60 = {
    {FUN_0021b858, FUN_0021d948},
    {1, 0.5f, 0.1f, 0, 3, 3, D_001D1D00}
};

struct MenuItemPreviewScreen D_001D1DC8 = {
    {0, FUN_0021eb20, FUN_0021ea48, FUN_0021eaf0, 2}
};

s32 D_001D1E88[9] = {0x4F44, 0x4F45, 0x4F46, 0x4F47, 0x4F48, 0x4F49, 0x4F4A, 0x4F4B, 0x4F4C};

struct MenuLabelScreen D_001D1EB0 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {       0x110, (s32)D_001D1E88,            4}
};

s32 D_001D1F08[9] = {0x4E69, 0x4E73, 0x4E78, 0x4E71, 0x4E72, 0x4E70, 0x4E74, 0x4E75, 0x4E77};

struct MenuLabelScreen D_001D1F30 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {       0x103, (s32)D_001D1F08,            4}
};

struct MenuPage D_001D1F88 = {
    {0x63, 0x64, 0x65, 0x66, 0x67, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    0xF,
    SCREEN(D_001D21B8),
    {SCREEN(D_001D2078),
     SCREEN(D_001D21B8),
     SCREEN(D_001D2260),
     SCREEN(D_001CFB10),
     SCREEN(D_001D2010),
     SCREEN(D_001D2290)}
};

struct MenuStreamScreen D_001D2010 = {
    {FUN_0021f990, FUN_0021fc68, FUN_0021fce0, FUN_0021fd78},
    {  D_00138308,         0x22,        0x132,         0x6C}
};

struct MenuLabelScreen D_001D2078 = {
    {FUN_00219d80, FUN_0021a328, FUN_002217f8},
    {0xF, 0x4EF0}
};

struct MenuTextItem D_001D20D0[19] = {
    {0x4EBA, 1, {0}, 0x4ECD},
    {0x4EBB, 1, {0}, 0x4ECE},
    {0x4EBC, 1, {0}, 0x4ECF},
    {0x4EBD, 1, {0}, 0x4ED0},
    {0x4EBE, 1, {0}, 0x4ED1},
    {0x4EBF, 1, {0}, 0x4ED2},
    {0x4EC0, 1, {0}, 0x4ED3},
    {0x4EC1, 1, {0}, 0x4ED4},
    {0x4EC2, 1, {0}, 0x4ED5},
    {0x4EC3, 1, {0}, 0x4ED6},
    {0x4EC4, 1, {0}, 0x4ED7},
    {0x4EC5, 1, {0}, 0x4ED8},
    {0x4EC6, 1, {0}, 0x4ED9},
    {0x4EC7, 1, {0}, 0x4EDA},
    {0x4EC8, 1, {0}, 0x4EDB},
    {0x4EC9, 1, {0}, 0x4EDC},
    {0x4ECA, 1, {0}, 0x4EDD},
    {0x4ECB, 1, {0}, 0x4EDE},
    {0},
};

struct MenuTextListScreen D_001D21B8 = {
    {FUN_0021abf8, FUN_0021d4a8, FUN_00221800},
    {0x139, D_001D62B8}
};

struct MenuPlainScreen58 D_001D2208 = {
    {FUN_00219d80, FUN_0021f5f8}
};

struct MenuLevelMapScreen D_001D2260 = {
    {FUN_00221908, FUN_00221930}
};

struct MenuMapScreen D_001D2290 = {
    {FUN_0021be60, 0, 0, FUN_0021bda0},
    {{0}, 0xC0}
};

struct MenuPage level_selection_map_cross_page = {
    {0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D1F88,
    0x10,
    SCREEN(D_001D2380),
    {SCREEN(D_001CF768),
     SCREEN(D_001CF4F8),
     &D_001D23D0,
     SCREEN(D_001D2450),
     SCREEN(D_001D24B8),
     SCREEN(D_001D2380)}
};

struct MenuTextListScreen D_001D2380 = {
    {FUN_00221968, FUN_0021b1c8},
    {      0x4082,   D_001CE408}
};

struct MenuScreen D_001D23D0 = {FUN_0021c4c0, FUN_0021f688, FUN_0021c420};

struct MenuMapScreen D_001D2450 = {
    {FUN_0021be60, FUN_00220850, FUN_0021cac8, FUN_0021bda0},
    {{0}, 0x40}
};

struct MenuItemPreviewScreen D_001D24B8 = {
    {0, FUN_0021ef78, FUN_0021ea48, FUN_0021eaf0, 2}
};

struct MenuPage D_001D2578 = {
    {0x81, 0x82, 0x83, 0x84, 0x85, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D15C8,
    0x1A,
    SCREEN(D_001D2650),
    {SCREEN(D_001D16D8), SCREEN(D_001D2600), &D_001CE2F8, SCREEN(D_001D2650), SCREEN(D_001D26D0)}
};

struct MenuSaveSlotScreen D_001D2600 = {
    {0, FUN_002239e0}
};

struct MenuPromptScreen D_001D2650 = {
    {FUN_00221ab8, FUN_00221b50, FUN_00221a48, FUN_00221a88}
};

struct MenuTextItem D_001D26A8[3] = {
    {0x524F},
    {0x524B},
    {0},
};

struct MenuTextListScreen D_001D26D0 = {
    {     0, FUN_0021b1c8},
    {0x4002,   D_001D26A8}
};

struct MenuPage D_001D2720 = {
    {0x8E, 0x8F, 0x90, 0x91, 0x92, 0x13, 0x14, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001CE488,
    9,
    SCREEN(D_001D28F8),
    {SCREEN(D_001D27A8),
     SCREEN(D_001D28F8),
     SCREEN(D_001D2948),
     SCREEN(D_001CFB10),
     SCREEN(D_001D28A0)}
};

struct MenuLabelScreen D_001D27A8 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {0xF, 0x4FC3}
};

struct MenuTextItem skill_point_and_gold_weapon_menu[10] = {
    {0x4FC4, 3, {(s32)&D_001D2C40}},
    {0x4FC5, 0xB},
    {0x4FC6, 3, {(s32)&D_001D3A30}},
    {0x4FC7, 3, {(s32)&D_001D3CD0}},
    {0x4FC8, 3, {(s32)&D_001D3FE0}},
    {0x4FC9, -1, {0x1D3558}},
    {0x4FCA, -1, {0x1D33B8}},
    {0x4FCB, -1, {3}},
    {0x4FCC, -1, {0x1D37F0}},
    {0},
};

s32 skill_point_and_gold_weapon_menu_labels[9] = {
    0x4FCE, 0x4FCF, 0x4FD0, 0x4FD1, 0x4FD2, -1, -1, -1, -1};

struct MenuLabelScreen D_001D28A0 = {
    {FUN_00219d80,                                 FUN_0021a328, FUN_0021a318},
    {        0x90, (s32)skill_point_and_gold_weapon_menu_labels,            4}
};

struct MenuTextListScreen D_001D28F8 = {
    {FUN_0021abf8,                     FUN_0021b1c8},
    {      0x1080, skill_point_and_gold_weapon_menu}
};

struct MenuStreamScreen D_001D2948 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_001384D0,       0x4220,         0xF9,        0x10B}
};

struct MenuPage D_001D29B0 = {
    {0xA5, 0xA6, 0xA7, 0xA8, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D2720,
    0x25,
    SCREEN(D_001D2B18),
    {SCREEN(D_001D2AC0), SCREEN(D_001D2B18), SCREEN(D_001D2B70), SCREEN(D_001D2BD8)}
};

struct MenuPage D_001D2A38 = {
    {0x86, 0x87, 0x88, 0x89, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    0x25,
    SCREEN(D_001D2B18),
    {SCREEN(D_001D2AC0), SCREEN(D_001D2B18), SCREEN(D_001D2B70), SCREEN(D_001D2BD8)}
};

struct MenuLabelScreen D_001D2AC0 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {0xF, 0x50A9}
};

struct MenuPlainScreen58 D_001D2B18 = {
    {FUN_002223f0, FUN_00222768, FUN_002223d8, 0, 1}
};

struct MenuStreamScreen D_001D2B70 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_00138520,        0xC20,         0xB6,         0xA9}
};

struct MenuStreamScreen D_001D2BD8 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_001385B8,        0xC20,         0xB6,         0xA9}
};

struct MenuPage D_001D2C40 = {
    {0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D2720,
    0x24,
    SCREEN(D_001D30A0),
    {SCREEN(D_001D2CC8),
     SCREEN(D_001D30A0),
     SCREEN(D_001D2D78),
     SCREEN(D_001CE438),
     SCREEN(D_001D3048),
     SCREEN(D_001D2D20)}
};

struct MenuLabelScreen D_001D2CC8 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {0xF, 0x4FC4}
};

struct MenuPlainScreen58 D_001D2D20 = {
    {0, FUN_002220f0}
};

struct MenuStreamScreen D_001D2D78 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_00138650,       0x2020,         0xF9,         0xB0}
};

struct MenuTextItem D_001D2DE0[31] = {
    {0x4FDC, 1, {1}},
    {0x4FDD, 1, {2}},
    {0x4FDE, 1, {2}},
    {0x4FDF, 1, {3}},
    {0x4FE0, 1, {3}},
    {0x4FE1, 1, {3}},
    {0x4FE2, 1, {4}},
    {0x4FE3, 1, {5}},
    {0x4FE4, 1, {5}},
    {0x4FE5, 1, {5}},
    {0x4FE6, 1, {6}},
    {0x4FE7, 1, {8}},
    {0x4FE8, 1, {8}},
    {0x4FE9, 1, {8}},
    {0x4FEA, 1, {9}},
    {0x4FEB, 1, {9}},
    {0x4FEC, 1, {0xA}},
    {0x4FED, 1, {0xA}},
    {0x4FEE, 1, {0xB}},
    {0x4FEF, 1, {0xB}},
    {0x4FF0, 1, {0xC}},
    {0x4FF1, 1, {0xC}},
    {0x4FF2, 1, {0xD}},
    {0x4FF3, 1, {0xE}},
    {0x4FF4, 1, {0xE}},
    {0x4FF5, 1, {0x10}},
    {0x4FF6, 1, {0x10}},
    {0x4FF7, 1, {0x11}},
    {0x4FF8, 1, {0x11}},
    {0x4FF9, 1, {0x12}},
    {0},
};

s32 D_001D2F58[30][2] = {
    {0x4FFA, 0x4FFA},
    {0x4FFB, 0x4FFB},
    {0x4FFC, 0x4FFC},
    {0x4FFD, 0x4FFD},
    {0x4FFE, 0x4FFE},
    {0x4FFF, 0x4FFF},
    {0x5000, 0x5000},
    {0x5001, 0x5001},
    {0x5002, 0x5002},
    {0x5003, 0x5004},
    {0x5005, 0x5005},
    {0x5006, 0x5006},
    {0x5007, 0x5007},
    {0x5008, 0x5008},
    {0x5009, 0x5009},
    {0x500A, 0x500A},
    {0x500B, 0x500B},
    {0x500C, 0x500C},
    {0x500D, 0x500D},
    {0x500E, 0x500E},
    {0x500F, 0x500F},
    {0x5010, 0x5010},
    {0x5011, 0x5011},
    {0x5012, 0x5012},
    {0x5013, 0x5013},
    {0x5014, 0x5014},
    {0x5015, 0x5015},
    {0x5016, 0x5016},
    {0x5017, 0x5017},
    {0x5018, 0x5018},
};

struct MenuLabelScreen D_001D3048 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {      0x8890, (s32)D_001D2F58,            8}
};

struct MenuTextListScreen D_001D30A0 = {
    {FUN_0021abf8, FUN_0021d4a8},
    {       0x200,   D_001D2DE0}
};

struct MenuPage D_001D30F0 = {
    {0x9E, 0x9F, 2, 0xA1, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    0x22,
    SCREEN(D_001D3360),
    {SCREEN(D_001D3178), SCREEN(D_001D3360), &D_001CE2F8, SCREEN(D_001D3260)}
};

struct MenuPlainScreenC0 D_001D3178 = {
    {FUN_002256e8, FUN_00225a68, FUN_00225588, FUN_00225660, 2}
};

struct MenuTextItem D_001D3238[3] = {
    {0x5184},
    {0x5185},
    {0},
};

struct MenuTextListScreen D_001D3260 = {
    {      0, FUN_0021b1c8},
    {0x24002,   D_001D3238}
};

struct MenuPlainScreen58 D_001D32B0 = {
    {FUN_00219d80, FUN_00222948}
};

struct MenuLabelScreen D_001D3308 = {
    {0, FUN_0021a328},
    {4,       0x4F9E}
};

struct MenuPlainScreen58 D_001D3360 = {
    {FUN_00222290, FUN_00222a98}
};

struct MenuPage D_001D33B8 = {
    {0xA9, 0xAA, 0xAB, 0xAC, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D2720,
    0x2A,
    SCREEN(D_001D3498),
    {SCREEN(D_001D3440), SCREEN(D_001D34F0), SCREEN(D_001D3498), SCREEN(D_001CE438)}
};

struct MenuCycleScreen D_001D3440 = {
    {0, FUN_00221f58}
};

struct MenuCycleScreen D_001D3498 = {
    {FUN_00221e50, FUN_00221f58},
    {         {0},            1}
};

struct MenuStreamScreen D_001D34F0 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {D_00138748, 0x1220, 0x11E, 0x169, &D_001D3498.cycle.selection}
};

struct MenuPage D_001D3558 = {
    {0xB3, 0xB4, 0xB5, 0xB6, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D2720,
    0x29,
    SCREEN(D_001D36D0),
    {SCREEN(D_001D3648), SCREEN(D_001D35E0), SCREEN(D_001D3798), SCREEN(D_001D36D0)}
};

struct MenuStreamScreen D_001D35E0 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {D_00138928, 0x220, 0x113, 0x166, D_001601E8}
};

struct MenuLabelScreen D_001D3648 = {
    {FUN_00219d80, FUN_0021a328},
    {         0xF,       0x4FC9}
};

struct MenuTextItem D_001D36A0[4] = {
    {0x508B},
    {0x508A},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen D_001D36D0 = {
    {FUN_00221d68, FUN_0021b1c8},
    {      0x4002,   D_001D36A0}
};

s32 D_001D3720[30] = {0x5187, 0x5188, 0x5189, 0x518A, 0x518B, 0x518C, 0x518D, 0x518E,
                      0x518F, 0x5190, 0x5191, 0x5192, 0x5193, 0x5194, 0x5195, 0x5196,
                      0x5197, 0x5198, 0x5199, 0x519A, 0x519B, 0x519C, 0x519D, 0x519E,
                      0x519F, 0x51A0, 0x51A1, 0x51A2, 0x51A3, 0x51A4};

struct MenuLabelScreen D_001D3798 = {
    {FUN_00219d80,    FUN_0021a328, FUN_0021a318},
    {        0x90, (s32)D_001D3720,            4}
};

struct MenuPage D_001D37F0 = {
    {0xAF, 0xB0, 0xB1, 0xB2, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D2720,
    0x2C,
    SCREEN(D_001D3900),
    {SCREEN(D_001D3878), SCREEN(D_001D3900), SCREEN(D_001D3950), SCREEN(D_001D39E0)}
};

struct MenuLabelScreen D_001D3878 = {
    {FUN_00219d80, FUN_0021a328},
    {         0xF,       0x4FCC}
};

struct MenuTextItem D_001D38D0[4] = {
    {0x508D, 0xA, {1}},
    {0x508E, 0xA, {2}},
    {0x508F, 0xA},
    {0},
};

struct MenuTextListScreen D_001D3900 = {
    {FUN_0021abf8, FUN_0021d4a8},
    {       0x490,   D_001D38D0}
};

struct MenuStreamScreen D_001D3950 = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {  D_00138A18,        0x220,        0x107,         0xC4}
};

struct MenuTextItem D_001D39B8[3] = {
    {0x5091},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen D_001D39E0 = {
    {FUN_00219d80, FUN_0021b1c8},
    {      0x4002,   D_001D39B8}
};

struct MenuPage D_001D3A30 = {
    {0x99, 0x9A, 0x9B, 0x9C, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D2720,
    0x26,
    SCREEN(D_001D3C18),
    {SCREEN(D_001D3AB8), SCREEN(D_001D3C18), SCREEN(D_001D3C80)}
};

struct MenuLabelScreen D_001D3AB8 = {
    {FUN_00219d80, FUN_0021a328},
    {         0xF,       0x4FC6}
};

struct MenuOption configured_label_entries[13] = {
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

struct MenuOptionListScreen D_001D3C18 = {
    {FUN_00220e28, FUN_00221030, FUN_0021a1e0},
    {1, configured_label_entries}
};

struct MenuTextItem D_001D3C58[3] = {
    {0x4EDF},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen D_001D3C80 = {
    {FUN_00219d80, FUN_0021b1c8},
    {      0x4002,   D_001D3C58}
};

struct MenuPage D_001D3CD0 = {
    {0xA3, 0x9A, 0x9B, 0x9C, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D2720,
    0x27,
    SCREEN(D_001D3F90),
    {SCREEN(D_001D3D58), SCREEN(D_001D3F90), SCREEN(D_001D3DD8)}
};

struct MenuLabelScreen D_001D3D58 = {
    {FUN_00219d80, FUN_0021a328},
    {         0xF,       0x4FC7}
};

struct MenuTextItem D_001D3DB0[3] = {
    {0x5089},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen D_001D3DD8 = {
    {FUN_00219d80, FUN_0021b1c8},
    {      0x4002,   D_001D3DB0}
};

struct MenuTextItem D_001D3E28[30] = {
    {0x506C, 8},
    {0x506D, 8, {1}},
    {0x506E, 7, {1}},
    {0x506F, 8, {3}},
    {0x5070, 8, {5}},
    {0x5071, 8, {4}},
    {0x5072, 7, {2}},
    {0x5073, 7, {3}},
    {0x5074, 7, {4}},
    {0x5075, 8, {6}},
    {0x5076, 7, {6}},
    {0x5077, 7, {5}},
    {0x5078, 7, {7}},
    {0x5079, 7, {8}},
    {0x507A, 8, {7}},
    {0x507B, 7, {9}},
    {0x507C, 7, {0xA}},
    {0x507D, 7, {0xB}},
    {0x507E, 7, {0xC}},
    {0x507F, 8, {8}},
    {0x5080, 7, {0xD}},
    {0x5081, 7, {0xE}},
    {0x5082, 8, {9}},
    {0x5083, 7, {0xF}},
    {0x5084, 7, {0x10}},
    {0x5085, 8, {0xA}},
    {0x5086, 7, {0x11}},
    {0x5087, 7, {0x12}},
    {0x5088, 8, {0xB}},
    {0},
};

struct MenuTextListScreen D_001D3F90 = {
    {FUN_0021abf8, FUN_0021d4a8},
    {       0x400,   D_001D3E28}
};

struct MenuPage D_001D3FE0 = {
    {0xA4, 0x9A, 0x9B, 0x9C, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D2720,
    0x28,
    SCREEN(D_001D4578),
    {SCREEN(D_001D4068), SCREEN(D_001D4578), SCREEN(D_001D3DD8)}
};

struct MenuLabelScreen D_001D4068 = {
    {FUN_00219d80, FUN_0021a328},
    {         0xF,       0x4FC8}
};

struct MenuTextItem D_001D40C0[4] = {
    {0x5022, 6, {1}},
    {0x5023, 6, {2}},
    {0x5024, 6, {3}},
    {0},
};

struct MenuTextItem D_001D40F0[7] = {
    {0x5025, 6, {5}},
    {0x5026, 6, {3}},
    {0x5027, 6, {0x20004}},
    {0x5028, 6, {0x30000}},
    {0x5029, 6, {0x30001}},
    {0x502A, 6, {0x30002}},
    {0},
};

struct MenuTextItem D_001D4148[5] = {
    {0x502B, 6, {0x100000}},
    {0x502C, 6, {1}},
    {0x502D, 6, {2}},
    {0x502E, 6, {3}},
    {0},
};

struct MenuTextItem D_001D4188[8] = {
    {0x502F, 6, {0x140000}},
    {0x5030, 6, {0x140001}},
    {0x5031, 6, {0x150002}},
    {0x5032, 6, {0x150003}},
    {0x5033, 6, {4}},
    {0x5034, 6, {0x180005}},
    {0x5035, 6, {0x180008}},
    {0},
};

struct MenuTextItem D_001D41E8[3] = {
    {0x5036, 6, {0x240000}},
    {0x5037, 6, {0x240001}},
    {0},
};

struct MenuTextItem D_001D4210[6] = {
    {0x5038, 6, {0x260000}},
    {0x5039, 6, {0x260009}},
    {0x503A, 6, {0x270005}},
    {0x503B, 6, {0xA}},
    {0x503C, 6, {0xB}},
    {0},
};

struct MenuTextItem D_001D4258[3] = {
    {0x503D, 6, {0x2F0000}},
    {0x503E, 6, {1}},
    {0},
};

struct MenuTextItem D_001D4280[5] = {
    {0x503F, 6},
    {0x5040, 6, {0x330001}},
    {0x5041, 6, {0x340002}},
    {0x5042, 6, {0x350004}},
    {0},
};

struct MenuTextItem D_001D42C0[8] = {
    {0x5043, 6},
    {0x5044, 6, {0x370001}},
    {0x5045, 6, {0x370002}},
    {0x5046, 6, {0x380003}},
    {0x5047, 6, {0x380004}},
    {0x5048, 6, {0x390006}},
    {0x5049, 6, {0x390008}},
    {0},
};

struct MenuTextItem D_001D4320[3] = {
    {0x504A, 6},
    {0x504B, 6, {1}},
    {0},
};

struct MenuTextItem D_001D4348[7] = {
    {0x504C, 6, {0x420000}},
    {0x504D, 6, {0x420001}},
    {0x504E, 6, {0x430003}},
    {0x504F, 6, {0x430004}},
    {0x5050, 6, {0x440005}},
    {0x5051, 6, {0x440006}},
    {0},
};

struct MenuTextItem D_001D43A0[5] = {
    {0x5052, 6, {1}},
    {0x5053, 6, {0x480002}},
    {0x5054, 6, {0x460003}},
    {0x5055, 6, {0x460004}},
    {0},
};

struct MenuTextItem D_001D43E0[5] = {
    {0x5056, 6},
    {0x5057, 6, {0x4B0001}},
    {0x5058, 6, {2}},
    {0x5059, 6, {3}},
    {0},
};

struct MenuTextItem D_001D4420[6] = {
    {0x505A, 6, {0x560000}},
    {0x505B, 6, {0x570001}},
    {0x505C, 6, {0x570002}},
    {0x505D, 6, {0x580003}},
    {0x505E, 6, {0x580004}},
    {0},
};

struct MenuTextItem D_001D4468[6] = {
    {0x505F, 6},
    {0x5060, 6, {1}},
    {0x5061, 6, {2}},
    {0x5062, 6, {3}},
    {0x5063, 6, {4}},
    {0},
};

struct MenuTextItem D_001D44B0[4] = {
    {0x5064, 6, {0x660000}},
    {0x5065, 6, {0x660001}},
    {0x5066, 6, {0x650002}},
    {0},
};

struct MenuTextItem D_001D44E0[6] = {
    {0x5067, 6},
    {0x5068, 6, {2}},
    {0x5069, 6, {3}},
    {0x506A, 6, {4}},
    {0x506B, 6, {5}},
    {0},
};

struct MenuTextItem *pause_level_sprite_lists[19] = {
    D_001D40C0, D_001D40F0, D_001D4148, D_001D4188, D_001D41E8, D_001D4210, D_001D4258,
    D_001D4280, D_001D42C0, D_001601F0, D_001D4320, D_001D4348, D_001D43A0, D_001D43E0,
    D_001D4420, D_001D4468, D_001D44B0, D_00160200, D_001D44E0,
};

struct MenuTextListScreen D_001D4578 = {
    {FUN_0021abf8, FUN_0021d4a8, FUN_00221d28},
    {0x10410}
};

struct MenuPage menu_preview_initial_page = {
    {0, 1, 0x10, 0x11, 0x12, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    0x2E,
    SCREEN(D_001D4698),
    {&D_001CE2F8, &D_001CE2F8, SCREEN(D_001D4698), SCREEN(D_001D46E8), SCREEN(D_001D4738)}
};

struct MenuTextItem D_001D4650[2] = {
    {0x4F2A, 4, {0x1D4C88}},
    {0},
};

struct MenuTextItem D_001D4668[2] = {
    {0x4F28, 5, {0x1D4C00}},
    {0},
};

struct MenuTextItem D_001D4680[2] = {
    {0x4EE7, 3, {(s32)&D_001D4788}},
    {0},
};

struct MenuTextListScreen D_001D4698 = {
    {FUN_0021abf8, FUN_0021b1c8},
    {4, D_001D4650, SCREEN(D_001D4738), SCREEN(D_001D46E8)}
};

struct MenuTextListScreen D_001D46E8 = {
    {FUN_0021abf8, FUN_0021b1c8},
    {4, D_001D4668, SCREEN(D_001D4698), SCREEN(D_001D4738)}
};

struct MenuTextListScreen D_001D4738 = {
    {FUN_0021abf8, FUN_0021b1c8},
    {4, D_001D4680, SCREEN(D_001D46E8), SCREEN(D_001D4698)}
};

struct MenuPage D_001D4788 = {
    {0, 1, 0xBE, 0xBF, 0xC0, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &menu_preview_initial_page,
    0x2F,
    SCREEN(D_001D4868),
    {&D_001CE2F8, &D_001CE2F8, SCREEN(D_001D08D8), SCREEN(D_001D4868), SCREEN(D_001CFB10)}
};

struct MenuTextItem pause_action_list_nonzero_mode[4] = {
    {0x4F2B, 3, {(s32)&D_001D4940}},
    {0x4F21, 3, {(s32)&D_001D4AB8}},
    {0x4F22, 3, {(s32)&D_001D48B8}},
    {0},
};

struct MenuTextItem pause_action_list_zero_mode[3] = {
    {0x4F2B, 3, {(s32)&D_001D4940}},
    {0x4F22, 3, {(s32)&D_001D48B8}},
    {0},
};

struct MenuTextListScreen D_001D4868 = {
    {FUN_0021abf8, FUN_0021b1c8, FUN_0021a1b0}
};

struct MenuPage D_001D48B8 = {
    {0, 1, 0xCA, 0xD2, 0xD7, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D4788,
    0x20,
    SCREEN(D_001D1578),
    {&D_001CE2F8, &D_001CE2F8, SCREEN(D_001D1520), SCREEN(D_001D1578), SCREEN(D_001CE3A0)}
};

struct MenuPage D_001D4940 = {
    {0, 1, 0xC1, 0xC2, 0xC3, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D4788,
    0x30,
    SCREEN(D_001D4A68),
    {&D_001CE2F8, &D_001CE2F8, SCREEN(D_001D49C8), SCREEN(D_001D4A68), SCREEN(D_001CFB10)}
};

struct MenuLabelScreen D_001D49C8 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {0xF, 0x4F2B}
};

struct MenuTextItem D_001D4A20[6] = {
    {0x4F2C, 9},
    {0x4F2D, 9, {2}},
    {0x4F2F, 9, {3}},
    {0x4F2E, 9, {4}},
    {0x4F30, 9, {5}},
    {0},
};

struct MenuTextListScreen D_001D4A68 = {
    {FUN_0021abf8, FUN_0021d4a8},
    {     0x10810,   D_001D4A20}
};

struct MenuPage D_001D4AB8 = {
    {0, 1, 0xCA, 0xCB, 0xC3, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D4788,
    0x31,
    SCREEN(D_001D4BC0),
    {&D_001CE2F8, &D_001CE2F8, SCREEN(D_001D4B40), SCREEN(D_001D4BC0), SCREEN(D_001CFB10)}
};

struct MenuLabelScreen D_001D4B40 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {0xF, 0x4F3A}
};

struct MenuOption D_001D4B98[2] = {
    {0x4F3B, &requested_video_mode, 0x4F3C, 0x4F3D, 1},
    {0},
};

struct MenuOptionListScreen D_001D4BC0 = {
    {FUN_00220e28, FUN_00221030},
    {           0,   D_001D4B98}
};

struct MenuPage D_001D4C00 = {
    {0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &menu_preview_initial_page,
    0x1F,
    SCREEN(D_001D1830),
    {SCREEN(D_001D1730),
     SCREEN(D_001D1830),
     SCREEN(D_001D18F8),
     SCREEN(D_001D1788),
     SCREEN(D_001D18A8)}
};

struct MenuPage D_001D4C88 = {
    {0xC4, 0xC5, 0xC6, 0xC7, 0xC9, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &menu_preview_initial_page,
    0x1F,
    SCREEN(D_001D4D68),
    {SCREEN(D_001D4D10),
     SCREEN(D_001D4D68),
     SCREEN(D_001D18F8),
     SCREEN(D_001D1788),
     SCREEN(D_001D4DE8)}
};

struct MenuLabelScreen D_001D4D10 = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {0xF, 0x4F2A}
};

struct MenuSaveSlotScreen D_001D4D68 = {
    {FUN_002235b8, FUN_002239e0, FUN_00222f18, FUN_00222f58}
};

struct MenuTextItem D_001D4DB8[4] = {
    {0x4F27},
    {0x5232},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen D_001D4DE8 = {
    {     0, FUN_0021b1c8},
    {0x4002,   D_001D4DB8}
};

struct MenuPage D_001D4E38 = {
    {0x81, 0x82, 0x83, 0x84, 0xD1, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D4C88,
    0x1A,
    SCREEN(D_001D2650),
    {SCREEN(D_001D4D10), SCREEN(D_001D4D68), &D_001CE2F8, SCREEN(D_001D2650), SCREEN(D_001D26D0)}
};

struct MenuPage circle_target_menu_page = {
    {0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &D_001D30F0,
    0x22,
    SCREEN(D_001D4F48),
    {SCREEN(D_001D4D10),
     SCREEN(D_001D4F48),
     SCREEN(D_001D18F8),
     SCREEN(D_001D1788),
     SCREEN(D_001D18A8)}
};

struct MenuSaveSlotScreen D_001D4F48 = {
    {FUN_002235b8, FUN_002239e0, FUN_00222f18, FUN_00222f58},
    {0x2000}
};

struct MenuPage D_001D4F98 = {
    {0x81, 0x82, 0x83, 0x84, 0x85, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &circle_target_menu_page,
    0x1A,
    SCREEN(D_001D2650),
    {SCREEN(D_001D16D8), SCREEN(D_001D4F48), &D_001CE2F8, SCREEN(D_001D2650), SCREEN(D_001D26D0)}
};
