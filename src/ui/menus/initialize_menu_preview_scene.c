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
#include "rnc/game/options.h"

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

extern struct MenuStreamEntry disc_help_moves[] __asm__("D_00137EA8");
extern struct MenuStreamEntry disc_help_weapons[] __asm__("D_00137F20");
extern struct MenuStreamEntry disc_help_gadgets[] __asm__("D_00137F98");
extern struct MenuStreamEntry disc_options_images[] __asm__("D_00138040");
extern struct MenuStreamEntry disc_planet_images[] __asm__("D_00138308");
extern struct MenuStreamEntry disc_goodies_images[] __asm__("D_001384D0");
extern struct MenuStreamEntry disc_character_sketches[] __asm__("D_00138520");
extern struct MenuStreamEntry disc_character_renders[] __asm__("D_001385B8");
extern struct MenuStreamEntry disc_skill_images[] __asm__("D_00138650");
extern struct MenuStreamEntry disc_epilogue_images[] __asm__("D_00138748");
extern struct MenuStreamEntry disc_sketchbook[] __asm__("D_00138928");
extern struct MenuStreamEntry disc_commercials[] __asm__("D_00138A18");
extern struct MenuStreamEntry disc_item_images[] __asm__("D_00138A38");
extern s32                    D_001601E8[];
extern struct MenuTextItem    in_level_movies_gaspar[] __asm__("D_001601F0");
extern struct MenuTextItem    in_level_movies_drek_fleet[] __asm__("D_00160200");
extern struct MenuIcon        pause_item_list_entries[] __asm__("D_001D60E0");
extern struct MenuIcon        filtered_menu_item_records[] __asm__("D_001D61B8");
extern int                    pause_item_variant_values[] __asm__("D_001D6178");
extern u8                     subtitles_enabled __asm__("D_0015EE40");
extern struct MenuTextItem    planet_select_items[] __asm__("D_001D62B8");

struct MenuTextListScreen moves_help_list = {
    {  FUN_0021abf8,     FUN_0021b1c8},
    {MENU_LIST_WRAP, moves_help_items},
};

struct MenuStreamScreen moves_help_image = {
    {   FUN_0021fdc8,       FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_help_moves, MENU_STREAM_UNPACK,        0x100,         0xD0},
};

struct MenuLabelScreen moves_help_label = {
    {                                        FUN_00219d80, FUN_0021a328, FUN_0021ca98},
    {MENU_LABEL_SMALL_FONT | MENU_LABEL_BY_LIST_SELECTION,            0,            4},
};

struct MenuPage pause_options_page = {
    {0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x14, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_root_page,
    8,
    SCREEN(options_list),
    {SCREEN(options_title_label),
     SCREEN(options_description_label),
     SCREEN(options_image),
     SCREEN(options_list),
     SCREEN(select_exit_hint_list)}
};

struct MenuLabelScreen options_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT |
         MENU_LABEL_NO_SCROLL, 0x4F1D},
};

struct MenuStreamScreen options_image = {
    {       FUN_0021fdc8,       FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_options_images, MENU_STREAM_UNPACK,         0x93,         0x85},
};

struct MenuTextItem options_items[8] = {
    {0x4F1E, MENU_ACTION_SET_NEXT_PAGE, {(s32)&helpdesk_page}},
    {0x4F1F, MENU_ACTION_SET_NEXT_PAGE_OR_INITIALIZE_FREEZE, {(s32)&save_game_page}},
    {0x4F20, MENU_ACTION_SET_NEXT_PAGE_OR_INITIALIZE_TIMED_FREEZE, {(s32)&load_game_page}},
    {0x4F22, MENU_ACTION_SET_NEXT_PAGE, {(s32)&sound_page}},
    {0x4F23, MENU_ACTION_SET_NEXT_PAGE, {(s32)&camera_page}},
    {0x4F24, MENU_ACTION_SET_NEXT_PAGE, {(s32)&subtitles_page}},
    {0x4F26, MENU_ACTION_SET_NEXT_PAGE, {(s32)&quit_game_page}},
    {0},
};

struct MenuTextListScreen options_list = {
    {  FUN_0021abf8,  FUN_0021b1c8},
    {MENU_LIST_WRAP, options_items},
};

s32 options_description_text_ids[7] = {0x4F31, 0x4F32, 0x4F33, 0x4F35, 0x4F36, 0x4F37, 0x4F39};

struct MenuLabelScreen options_description_label = {
    {                                        FUN_00219d80,                      FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_SMALL_FONT | MENU_LABEL_BY_LIST_SELECTION, (s32)options_description_text_ids,            4},
};

struct MenuPage helpdesk_page = {
    {0x49, 0x4A, 0x4B, 0x4C, 0x4D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_options_page,
    0x13,
    SCREEN(helpdesk_option_list),
    {SCREEN(helpdesk_title_label),
     &menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(helpdesk_option_list),
     SCREEN(toggle_exit_hint_list)}
};

struct MenuLabelScreen helpdesk_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F40},
};

struct MenuOption helpdesk_options[3] = {
    {0x4F9F, &D_0015EE1C, 0x4F5A, 0x4F5B},
    {0x4FA0, &D_0015EE1D, 0x4F5A, 0x4F5B},
    {0},
};

struct MenuOptionListScreen helpdesk_option_list = {
    {FUN_00220e28,     FUN_00221030},
    {           0, helpdesk_options},
};

struct MenuPage weapon_help_page = {
    {0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 6, 0x61, 0x62, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_help_page,
    0x18,
    SCREEN(weapon_help_icon_list),
    {SCREEN(weapon_help_title_label),
     SCREEN(weapon_help_icon_list),
     SCREEN(weapon_help_image),
     SCREEN(item_preview_screen),
     SCREEN(pause_item_preview_screen),
     SCREEN(pause_item_info_label),
     &menu_placeholder_screen,
     SCREEN(exit_hint_list),
     SCREEN(weapon_help_text_label)}
};

struct MenuLabelScreen weapon_help_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_002215f8},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F99},
};

struct MenuIconListScreen weapon_help_icon_list = {
    {FUN_00219e10, FUN_00219fa0},
    {{0}, 0, 0, {0}, pause_item_list_entries},
};

struct MenuStreamScreen weapon_help_image = {
    {     FUN_0021fdc8,                                   FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_help_weapons, MENU_STREAM_BY_CELL_ENTRY | MENU_STREAM_UNPACK,         0xEE,         0xD5},
};

struct MenuLabelScreen weapon_help_text_label = {
    {                                   FUN_00219d80,                   FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_SMALL_FONT | MENU_LABEL_BY_GRID_CELL, (s32)pause_item_variant_values,            4},
};

struct MenuItemPreviewScreen item_preview_screen = {
    {FUN_0021e230, FUN_0021e608, FUN_0021df30, FUN_0021df58, MENU_SCREEN_DRAW_3D},
};

struct MenuItemPreviewScreen gadget_help_preview_screen = {
    {FUN_0021e230, FUN_0021e608, FUN_0021df30, FUN_0021df58, MENU_SCREEN_DRAW_3D},
    {1},
};

struct MenuPage gadget_help_page = {
    {0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 6, 0x74, 0x75, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_help_page,
    0x19,
    SCREEN(gadget_help_icon_list),
    {SCREEN(gadget_help_title_label),
     SCREEN(gadget_help_icon_list),
     SCREEN(gadget_help_image),
     SCREEN(gadget_help_preview_screen),
     SCREEN(gadget_help_text_label),
     SCREEN(pause_item_info_label),
     &menu_placeholder_screen,
     SCREEN(exit_hint_list),
     SCREEN(gadget_help_name_label)}
};

struct MenuLabelScreen gadget_help_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_002216c0},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F9A},
};

struct MenuIconListScreen gadget_help_icon_list = {
    {FUN_00219e10, FUN_00219fa0},
    {{0}, 0, 0, {0}, filtered_menu_item_records},
};

struct MenuStreamScreen gadget_help_image = {
    {     FUN_0021fdc8,                                   FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_help_gadgets, MENU_STREAM_BY_CELL_ENTRY | MENU_STREAM_UNPACK,         0xEE,         0x9F},
};

struct MenuLabelScreen gadget_help_name_label = {
    {                                   FUN_00219d80,                   FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_SMALL_FONT | MENU_LABEL_BY_GRID_CELL, (s32)gadget_help_name_text_ids,            4},
};

struct MenuLabelScreen gadget_help_text_label = {
    {                                                       FUN_00219d80,FUN_0021a328,FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_BY_GRID_CELL,
     (s32)gadget_help_text_ids,
     4       },
};

struct MenuPage subtitles_page = {
    {0x49, 0x4A, 0x4B, 0x4C, 0x4D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_options_page,
    0x1B,
    SCREEN(subtitles_option_list),
    {SCREEN(subtitles_title_label),
     &menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(subtitles_option_list),
     SCREEN(toggle_exit_hint_list)}
};

struct MenuLabelScreen subtitles_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F6F},
};

struct MenuOption subtitles_options[2] = {
    {0x4F24, &subtitles_enabled, 0x4F5A, 0x4F5B},
    {0},
};

struct MenuOptionListScreen subtitles_option_list = {
    {FUN_00220e28,      FUN_00221030},
    {           0, subtitles_options},
};

struct MenuPage vibration_page = {
    {0x76, 0x77, 0x78, 0x7B, 0x7A, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_options_page,
    0x1C,
    SCREEN(vibration_option_list),
    {SCREEN(vibration_title_label),
     &menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(vibration_option_list),
     SCREEN(toggle_exit_hint_list)}
};

struct MenuLabelScreen vibration_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F6F},
};

struct MenuOption vibration_options[2] = {
    {0x4FA1, 0, 0x4F5A, 0x4F5B},
    {0},
};

struct MenuOptionListScreen vibration_option_list = {
    {FUN_00220e28,      FUN_00221030},
    {           0, vibration_options},
};

struct MenuPage sound_page = {
    {0x76, 0xD4, 0xD5, 0xD6, 0xD7, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_options_page,
    0x20,
    SCREEN(sound_volume_screen),
    {SCREEN(sound_title_label),
     &menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(sound_volume_screen),
     SCREEN(toggle_exit_hint_list)}
};

struct MenuLabelScreen sound_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F43},
};

struct MenuPlainScreen50 sound_volume_screen = {
    {FUN_0021cb30, FUN_0021ce00, 0, FUN_0021cb00}
};

struct MenuPage save_game_page = {
    {0x7C, 0x7D, 0x7E, 0x7F, 0x80, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_options_page,
    0x1E,
    SCREEN(save_game_slot_list),
    {SCREEN(save_game_title_label),
     SCREEN(save_game_slot_list),
     SCREEN(save_slot_image),
     SCREEN(save_slot_selection_text),
     SCREEN(save_page_hint_list)}
};

struct MenuPage load_game_page = {
    {0x7C, 0x7D, 0x7E, 0x7F, 0x80, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_options_page,
    0x1F,
    SCREEN(load_game_slot_list),
    {SCREEN(load_game_title_label),
     SCREEN(load_game_slot_list),
     SCREEN(save_slot_image),
     SCREEN(save_slot_selection_text),
     SCREEN(save_page_hint_list)}
};

struct MenuLabelScreen save_game_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F29},
};

struct MenuLabelScreen load_game_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F28},
};

struct MenuPlainScreen58 save_slot_selection_text = {
    {0, FUN_00222d98}
};

struct MenuSaveSlotScreen save_game_slot_list = {
    {FUN_00222f88, FUN_002239e0, FUN_00222f18, FUN_00222f58},
};

struct MenuSaveSlotScreen load_game_slot_list = {
    {FUN_002232d8, FUN_002239e0, FUN_00222f18, FUN_00222f58},
};

struct MenuTextItem save_page_hint_items[3] = {
    {0x4F27},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen save_page_hint_list = {
    {                                              0,         FUN_0021b1c8},
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST, save_page_hint_items},
};

struct MenuStreamScreen save_slot_image = {
    {      FUN_0021fdc8,                                  FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_planet_images, MENU_STREAM_UNPACK | MENU_STREAM_BY_SAVE_SLOT,        0x132,         0x6C},
};

struct MenuPage camera_page = {
    {0x76, 0x77, 0x78, 0x79, 0x7A, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_options_page,
    0x1D,
    SCREEN(camera_choice_list),
    {SCREEN(camera_title_label),
     &menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(camera_choice_list),
     SCREEN(toggle_exit_hint_list)}
};

struct MenuLabelScreen camera_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F42},
};

struct MenuChoice camera_choices[4] = {
    {0x4FA2, (u8 *)&opt_cam_invert, {0x4FA5, 0x4FA4}},
    {0x4FA3, (u8 *)&invert52, {0x4FA5, 0x4FA4}},
    {0x521B, (u8 *)&opt_cam_yaw_rate, {0x521A, 0x5219, 0x5218}},
    {0},
};

struct MenuChoiceListScreen camera_choice_list = {
    {FUN_002212b8, FUN_00221460, FUN_0021a308, FUN_0021a310},
    {{0}, camera_choices},
};

struct MenuPage quit_game_page = {
    {0x4E, 0x4F, 0x50, 0x51, 0x52, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_options_page,
    0xD,
    SCREEN(quit_game_screen),
    {&menu_placeholder_screen,
     &menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(quit_game_screen)}
};

struct MenuPlainScreen50 quit_game_screen = {
    {FUN_0021e7c8, FUN_0021e890}
};

struct MenuPage pause_items_page = {
    {0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x14, 0x5A, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_root_page,
    6,
    SCREEN(items_grid),
    {SCREEN(items_title_label),
     SCREEN(items_grid),
     SCREEN(gold_bolt_screen),
     SCREEN(items_description_label),
     SCREEN(items_image),
     SCREEN(exit_hint_list),
     &menu_placeholder_screen,
     SCREEN(items_name_label)}
};

struct MenuLabelScreen items_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4EE5},
};

struct MenuStreamScreen items_image = {
    {    FUN_0021fdc8,                                  FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_item_images, MENU_STREAM_BY_GRID_CELL | MENU_STREAM_UNPACK,         0xBB,         0x5E},
};

struct MenuGridCell items_grid_cells[9] = {
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

struct MenuGridScreen items_grid = {
    {FUN_0021b858, FUN_0021d948},
    {MENU_GRID_NO_CONFIRM, 0.5f, 0.1f, 0, 3, 3, items_grid_cells},
};

struct MenuItemPreviewScreen gold_bolt_screen = {
    {0, FUN_0021eb20, FUN_0021ea48, FUN_0021eaf0, MENU_SCREEN_DRAW_3D},
};

s32 items_description_text_ids[9] = {
    0x4F44, 0x4F45, 0x4F46, 0x4F47, 0x4F48, 0x4F49, 0x4F4A, 0x4F4B, 0x4F4C};

struct MenuLabelScreen items_description_label = {
    {                                   FUN_00219d80,                    FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_SMALL_FONT | MENU_LABEL_BY_GRID_CELL, (s32)items_description_text_ids,            4},
};

s32 items_name_text_ids[9] = {
    0x4E69, 0x4E73, 0x4E78, 0x4E71, 0x4E72, 0x4E70, 0x4E74, 0x4E75, 0x4E77};

struct MenuLabelScreen items_name_label = {
    {                                                       FUN_00219d80,FUN_0021a328,FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_BY_GRID_CELL,
     (s32)items_name_text_ids,
     4       },
};

struct MenuPage planet_select_page = {
    {0x63, 0x64, 0x65, 0x66, 0x67, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    0xF,
    SCREEN(planet_select_list),
    {SCREEN(planet_select_title_label),
     SCREEN(planet_select_list),
     SCREEN(planet_select_galaxy_map),
     SCREEN(select_exit_hint_list),
     SCREEN(planet_select_image),
     SCREEN(planet_select_map_screen)}
};

struct MenuStreamScreen planet_select_image = {
    {      FUN_0021f990,                                       FUN_0021fc68, FUN_0021fce0, FUN_0021fd78},
    {disc_planet_images, MENU_STREAM_BY_SELECTED_LEVEL | MENU_STREAM_UNPACK,        0x132,         0x6C},
};

struct MenuLabelScreen planet_select_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_002217f8},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4EF0},
};

/* Location name and planet name (subtext) of each level. */
struct MenuTextItem planet_select_level_items[19] = {
    {0x4EBA, MENU_ACTION_NO_OP, {0}, 0x4ECD},
    {0x4EBB, MENU_ACTION_NO_OP, {0}, 0x4ECE},
    {0x4EBC, MENU_ACTION_NO_OP, {0}, 0x4ECF},
    {0x4EBD, MENU_ACTION_NO_OP, {0}, 0x4ED0},
    {0x4EBE, MENU_ACTION_NO_OP, {0}, 0x4ED1},
    {0x4EBF, MENU_ACTION_NO_OP, {0}, 0x4ED2},
    {0x4EC0, MENU_ACTION_NO_OP, {0}, 0x4ED3},
    {0x4EC1, MENU_ACTION_NO_OP, {0}, 0x4ED4},
    {0x4EC2, MENU_ACTION_NO_OP, {0}, 0x4ED5},
    {0x4EC3, MENU_ACTION_NO_OP, {0}, 0x4ED6},
    {0x4EC4, MENU_ACTION_NO_OP, {0}, 0x4ED7},
    {0x4EC5, MENU_ACTION_NO_OP, {0}, 0x4ED8},
    {0x4EC6, MENU_ACTION_NO_OP, {0}, 0x4ED9},
    {0x4EC7, MENU_ACTION_NO_OP, {0}, 0x4EDA},
    {0x4EC8, MENU_ACTION_NO_OP, {0}, 0x4EDB},
    {0x4EC9, MENU_ACTION_NO_OP, {0}, 0x4EDC},
    {0x4ECA, MENU_ACTION_NO_OP, {0}, 0x4EDD},
    {0x4ECB, MENU_ACTION_NO_OP, {0}, 0x4EDE},
    {0},
};

struct MenuTextListScreen planet_select_list = {
    {FUN_0021abf8, FUN_0021d4a8, FUN_00221800},
    {MENU_LIST_BUTTONS_ONLY | MENU_LIST_SMALL_FONT | MENU_LIST_FIXED_STEP | MENU_LIST_SETS_LEVEL |
         MENU_LIST_SHOULDER_KEYS, planet_select_items},
};

struct MenuPlainScreen58 mission_labels_screen = {
    {FUN_00219d80, FUN_0021f5f8}
};

struct MenuLevelMapScreen planet_select_galaxy_map = {
    {FUN_00221908, FUN_00221930},
};

struct MenuMapScreen planet_select_map_screen = {
    {FUN_0021be60, 0, 0, FUN_0021bda0},
    {{0}, 0xC0},
};

struct MenuPage planet_confirm_page = {
    {0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &planet_select_page,
    0x10,
    SCREEN(planet_confirm_hint_list),
    {SCREEN(missions_location_label),
     SCREEN(map_planet_label),
     &planet_confirm_missions_screen,
     SCREEN(planet_confirm_map_screen),
     SCREEN(planet_confirm_gold_bolt_screen),
     SCREEN(planet_confirm_hint_list)}
};

struct MenuTextListScreen planet_confirm_hint_list = {
    {                                                              FUN_00221968,FUN_0021b1c8            },
    {MENU_LIST_FIXED_COLOR | MENU_LIST_NO_COLOR_CODES | MENU_LIST_CENTER_WIDEST,
     level_map_hint_items},
};

struct MenuScreen planet_confirm_missions_screen = {FUN_0021c4c0, FUN_0021f688, FUN_0021c420};

struct MenuMapScreen planet_confirm_map_screen = {
    {FUN_0021be60, FUN_00220850, FUN_0021cac8, FUN_0021bda0},
    {{0}, 0x40}
};

struct MenuItemPreviewScreen planet_confirm_gold_bolt_screen = {
    {0, FUN_0021ef78, FUN_0021ea48, FUN_0021eaf0, MENU_SCREEN_DRAW_3D},
};

struct MenuPage save_confirm_page = {
    {0x81, 0x82, 0x83, 0x84, 0x85, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &save_game_page,
    0x1A,
    SCREEN(save_confirm_prompt),
    {SCREEN(save_game_title_label),
     SCREEN(save_confirm_slot_list),
     &menu_placeholder_screen,
     SCREEN(save_confirm_prompt),
     SCREEN(confirm_hint_list)}
};

struct MenuSaveSlotScreen save_confirm_slot_list = {
    {0, FUN_002239e0},
};

struct MenuPromptScreen save_confirm_prompt = {
    {FUN_00221ab8, FUN_00221b50, FUN_00221a48, FUN_00221a88},
};

struct MenuTextItem confirm_hint_items[3] = {
    {0x524F},
    {0x524B},
    {0},
};

struct MenuTextListScreen confirm_hint_list = {
    {                                              0,       FUN_0021b1c8},
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST, confirm_hint_items},
};

struct MenuPage goodies_page = {
    {0x8E, 0x8F, 0x90, 0x91, 0x92, 0x13, 0x14, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &pause_root_page,
    9,
    SCREEN(goodies_list),
    {SCREEN(goodies_title_label),
     SCREEN(goodies_list),
     SCREEN(goodies_image),
     SCREEN(select_exit_hint_list),
     SCREEN(goodies_description_label)}
};

struct MenuLabelScreen goodies_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4FC3},
};

struct MenuTextItem skill_point_and_gold_weapon_menu[10] = {
    {0x4FC4, MENU_ACTION_SET_NEXT_PAGE, {(s32)&skill_points_page}},
    {0x4FC5, MENU_ACTION_REQUEST_CLOSE_PRESERVING_PAYLOAD},
    {0x4FC6, MENU_ACTION_SET_NEXT_PAGE, {(s32)&cheats_page}},
    {0x4FC7, MENU_ACTION_SET_NEXT_PAGE, {(s32)&cinematics_page}},
    {0x4FC8, MENU_ACTION_SET_NEXT_PAGE, {(s32)&in_level_movies_page}},
    {0x4FC9, MENU_ACTION_LOCKED, {(s32)&sketchbook_page}},
    {0x4FCA, MENU_ACTION_LOCKED, {(s32)&epilogue_page}},
    {0x4FCB, MENU_ACTION_LOCKED, {3}},
    {0x4FCC, MENU_ACTION_LOCKED, {(s32)&commercials_page}},
    {0},
};

s32 skill_point_and_gold_weapon_menu_labels[9] = {
    0x4FCE, 0x4FCF, 0x4FD0, 0x4FD1, 0x4FD2, -1, -1, -1, -1};

struct MenuLabelScreen goodies_description_label = {
    {                                        FUN_00219d80,FUN_0021a328,FUN_0021a318},
    {MENU_LABEL_SMALL_FONT | MENU_LABEL_BY_LIST_SELECTION,
     (s32)skill_point_and_gold_weapon_menu_labels,
     4       },
};

struct MenuTextListScreen goodies_list = {
    {                             FUN_0021abf8,                     FUN_0021b1c8},
    {MENU_LIST_NO_COLOR_CODES | MENU_LIST_WRAP, skill_point_and_gold_weapon_menu},
};

struct MenuStreamScreen goodies_image = {
    {       FUN_0021fdc8,             FUN_00220348,FUN_0021fce0, FUN_0021fd78},
    {disc_goodies_images,
     MENU_STREAM_UNPACK | MENU_STREAM_BUFFER_ARG | MENU_STREAM_DENIED_ENTRY,
     0xF9,        0x10B    },
};

struct MenuPage goodies_credits_page = {
    {0xA5, 0xA6, 0xA7, 0xA8, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &goodies_page,
    0x25,
    SCREEN(credits_text_screen),
    {SCREEN(credits_title_label),
     SCREEN(credits_text_screen),
     SCREEN(credits_sketch_image),
     SCREEN(credits_render_image)}
};

struct MenuPage credits_page = {
    {0x86, 0x87, 0x88, 0x89, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    0x25,
    SCREEN(credits_text_screen),
    {SCREEN(credits_title_label),
     SCREEN(credits_text_screen),
     SCREEN(credits_sketch_image),
     SCREEN(credits_render_image)}
};

struct MenuLabelScreen credits_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x50A9},
};

struct MenuPlainScreen58 credits_text_screen = {
    {FUN_002223f0, FUN_00222768, FUN_002223d8, 0, 1}
};

struct MenuStreamScreen credits_sketch_image = {
    {           FUN_0021fdc8,                                   FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_character_sketches, MENU_STREAM_UNPACK | MENU_STREAM_CYCLE | 0x800,         0xB6,         0xA9},
};

struct MenuStreamScreen credits_render_image = {
    {          FUN_0021fdc8,                                   FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_character_renders, MENU_STREAM_UNPACK | MENU_STREAM_CYCLE | 0x800,         0xB6,         0xA9},
};

struct MenuPage skill_points_page = {
    {0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &goodies_page,
    0x24,
    SCREEN(skill_point_list),
    {SCREEN(skill_points_title_label),
     SCREEN(skill_point_list),
     SCREEN(skill_points_image),
     SCREEN(exit_hint_list),
     SCREEN(skill_point_description_label),
     SCREEN(skill_point_level_screen)}
};

struct MenuLabelScreen skill_points_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4FC4},
};

struct MenuPlainScreen58 skill_point_level_screen = {
    {0, FUN_002220f0}
};

struct MenuStreamScreen skill_points_image = {
    {     FUN_0021fdc8,                                      FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_skill_images, MENU_STREAM_UNPACK | MENU_STREAM_SKILL_POINT_LOCK,         0xF9,         0xB0},
};

/* param: the level the skill point is earned on. */
struct MenuTextItem skill_point_items[31] = {
    {0x4FDC, MENU_ACTION_NO_OP, {1}},
    {0x4FDD, MENU_ACTION_NO_OP, {2}},
    {0x4FDE, MENU_ACTION_NO_OP, {2}},
    {0x4FDF, MENU_ACTION_NO_OP, {3}},
    {0x4FE0, MENU_ACTION_NO_OP, {3}},
    {0x4FE1, MENU_ACTION_NO_OP, {3}},
    {0x4FE2, MENU_ACTION_NO_OP, {4}},
    {0x4FE3, MENU_ACTION_NO_OP, {5}},
    {0x4FE4, MENU_ACTION_NO_OP, {5}},
    {0x4FE5, MENU_ACTION_NO_OP, {5}},
    {0x4FE6, MENU_ACTION_NO_OP, {6}},
    {0x4FE7, MENU_ACTION_NO_OP, {8}},
    {0x4FE8, MENU_ACTION_NO_OP, {8}},
    {0x4FE9, MENU_ACTION_NO_OP, {8}},
    {0x4FEA, MENU_ACTION_NO_OP, {9}},
    {0x4FEB, MENU_ACTION_NO_OP, {9}},
    {0x4FEC, MENU_ACTION_NO_OP, {0xA}},
    {0x4FED, MENU_ACTION_NO_OP, {0xA}},
    {0x4FEE, MENU_ACTION_NO_OP, {0xB}},
    {0x4FEF, MENU_ACTION_NO_OP, {0xB}},
    {0x4FF0, MENU_ACTION_NO_OP, {0xC}},
    {0x4FF1, MENU_ACTION_NO_OP, {0xC}},
    {0x4FF2, MENU_ACTION_NO_OP, {0xD}},
    {0x4FF3, MENU_ACTION_NO_OP, {0xE}},
    {0x4FF4, MENU_ACTION_NO_OP, {0xE}},
    {0x4FF5, MENU_ACTION_NO_OP, {0x10}},
    {0x4FF6, MENU_ACTION_NO_OP, {0x10}},
    {0x4FF7, MENU_ACTION_NO_OP, {0x11}},
    {0x4FF8, MENU_ACTION_NO_OP, {0x11}},
    {0x4FF9, MENU_ACTION_NO_OP, {0x12}},
    {0},
};

/* Per skill point: {text, text when pal_mode is set} (MENU_LABEL_VIDEO_MODE_VARIANT). */
s32 skill_point_description_text_ids[30][2] = {
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

struct MenuLabelScreen skill_point_description_label = {
    {                          FUN_00219d80,                          FUN_0021a328,FUN_0021a318},
    {MENU_LABEL_SMALL_FONT | MENU_LABEL_BY_LIST_SELECTION | MENU_LABEL_LOCKED_TEXT |
MENU_LABEL_VIDEO_MODE_VARIANT, (s32)skill_point_description_text_ids,
     8       },
};

struct MenuTextListScreen skill_point_list = {
    {               FUN_0021abf8,      FUN_0021d4a8},
    {MENU_LIST_SKILL_POINT_MARKS, skill_point_items},
};

struct MenuPage game_end_page = {
    {0x9E, 0x9F, 2, 0xA1, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    0x22,
    SCREEN(game_end_stats_screen),
    {SCREEN(helpdesk_girl_screen),
     SCREEN(game_end_stats_screen),
     &menu_placeholder_screen,
     SCREEN(game_end_hint_list)}
};

struct MenuPlainScreenC0 helpdesk_girl_screen = {
    {FUN_002256e8, FUN_00225a68, FUN_00225588, FUN_00225660, 2}
};

struct MenuTextItem game_end_hint_items[3] = {
    {0x5184},
    {0x5185},
    {0},
};

struct MenuTextListScreen game_end_hint_list = {
    {                                                                        0,FUN_0021b1c8           },
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST | MENU_LIST_SHRINK_TO_FIT,
     game_end_hint_items},
};

struct MenuPlainScreen58 gold_weapons_screen = {
    {FUN_00219d80, FUN_00222948}
};

struct MenuLabelScreen game_end_fixed_label = {
    {                    0, FUN_0021a328},
    {MENU_LABEL_FIXED_TEXT,       0x4F9E},
};

struct MenuPlainScreen58 game_end_stats_screen = {
    {FUN_00222290, FUN_00222a98}
};

struct MenuPage epilogue_page = {
    {0xA9, 0xAA, 0xAB, 0xAC, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &goodies_page,
    0x2A,
    SCREEN(epilogue_pager),
    {SCREEN(epilogue_side_indicator),
     SCREEN(epilogue_image),
     SCREEN(epilogue_pager),
     SCREEN(exit_hint_list)}
};

struct MenuCycleScreen epilogue_side_indicator = {
    {0, FUN_00221f58},
};

struct MenuCycleScreen epilogue_pager = {
    {FUN_00221e50, FUN_00221f58},
    {         {0},            1},
};

struct MenuStreamScreen epilogue_image = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_epilogue_images,
     MENU_STREAM_UNPACK | MENU_STREAM_BUFFER_ARG | MENU_STREAM_BY_LANGUAGE,
     0x11E, 0x169,
     &epilogue_pager.cycle.selection},
};

struct MenuPage sketchbook_page = {
    {0xB3, 0xB4, 0xB5, 0xB6, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &goodies_page,
    0x29,
    SCREEN(sketchbook_hint_list),
    {SCREEN(sketchbook_title_label),
     SCREEN(sketchbook_image),
     SCREEN(sketchbook_caption_label),
     SCREEN(sketchbook_hint_list)}
};

struct MenuStreamScreen sketchbook_image = {
    {FUN_0021fdc8, FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_sketchbook, MENU_STREAM_UNPACK | MENU_STREAM_BUFFER_ARG, 0x113, 0x166, D_001601E8},
};

struct MenuLabelScreen sketchbook_title_label = {
    {                                                                             FUN_00219d80,FUN_0021a328},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4FC9  },
};

struct MenuTextItem sketchbook_hint_items[4] = {
    {0x508B},
    {0x508A},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen sketchbook_hint_list = {
    {                                   FUN_00221d68,          FUN_0021b1c8},
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST, sketchbook_hint_items},
};

s32 sketchbook_caption_text_ids[30] = {
    0x5187, 0x5188, 0x5189, 0x518A, 0x518B, 0x518C, 0x518D, 0x518E, 0x518F, 0x5190,
    0x5191, 0x5192, 0x5193, 0x5194, 0x5195, 0x5196, 0x5197, 0x5198, 0x5199, 0x519A,
    0x519B, 0x519C, 0x519D, 0x519E, 0x519F, 0x51A0, 0x51A1, 0x51A2, 0x51A3, 0x51A4};

struct MenuLabelScreen sketchbook_caption_label = {
    {                                        FUN_00219d80,                     FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_SMALL_FONT | MENU_LABEL_BY_LIST_SELECTION, (s32)sketchbook_caption_text_ids,            4},
};

struct MenuPage commercials_page = {
    {0xAF, 0xB0, 0xB1, 0xB2, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &goodies_page,
    0x2C,
    SCREEN(commercials_list),
    {SCREEN(commercials_title_label),
     SCREEN(commercials_list),
     SCREEN(commercials_image),
     SCREEN(commercials_hint_list)}
};

struct MenuLabelScreen commercials_title_label = {
    {                                                                             FUN_00219d80,FUN_0021a328},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4FCC  },
};

struct MenuTextItem commercials_items[4] = {
    {0x508D, MENU_ACTION_REQUEST_CLOSE_6_WITH_WORD_PAYLOAD, {1}},
    {0x508E, MENU_ACTION_REQUEST_CLOSE_6_WITH_WORD_PAYLOAD, {2}},
    {0x508F, MENU_ACTION_REQUEST_CLOSE_6_WITH_WORD_PAYLOAD},
    {0},
};

struct MenuTextListScreen commercials_list = {
    {                                                        FUN_0021abf8,      FUN_0021d4a8},
    {MENU_LIST_FIXED_STEP | MENU_LIST_NO_COLOR_CODES | MENU_LIST_CENTERED, commercials_items},
};

struct MenuStreamScreen commercials_image = {
    {    FUN_0021fdc8,                                FUN_00220348, FUN_0021fce0, FUN_0021fd78},
    {disc_commercials, MENU_STREAM_UNPACK | MENU_STREAM_BUFFER_ARG,        0x107,         0xC4},
};

struct MenuTextItem commercials_hint_items[3] = {
    {0x5091},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen commercials_hint_list = {
    {                                   FUN_00219d80,           FUN_0021b1c8},
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST, commercials_hint_items},
};

struct MenuPage cheats_page = {
    {0x99, 0x9A, 0x9B, 0x9C, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &goodies_page,
    0x26,
    SCREEN(cheats_option_list),
    {SCREEN(cheats_title_label), SCREEN(cheats_option_list), SCREEN(cheats_hint_list)}
};

struct MenuLabelScreen cheats_title_label = {
    {                                                                             FUN_00219d80,FUN_0021a328},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4FC6  },
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

struct MenuOptionListScreen cheats_option_list = {
    {FUN_00220e28, FUN_00221030, FUN_0021a1e0},
    {1, configured_label_entries},
};

struct MenuTextItem cheats_hint_items[3] = {
    {0x4EDF},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen cheats_hint_list = {
    {                                   FUN_00219d80,      FUN_0021b1c8},
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST, cheats_hint_items},
};

struct MenuPage cinematics_page = {
    {0xA3, 0x9A, 0x9B, 0x9C, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &goodies_page,
    0x27,
    SCREEN(cinematics_list),
    {SCREEN(cinematics_title_label), SCREEN(cinematics_list), SCREEN(movie_page_hint_list)}
};

struct MenuLabelScreen cinematics_title_label = {
    {                                                                             FUN_00219d80,FUN_0021a328},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4FC7  },
};

struct MenuTextItem movie_page_hint_items[3] = {
    {0x5089},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen movie_page_hint_list = {
    {                                   FUN_00219d80,          FUN_0021b1c8},
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST, movie_page_hint_items},
};

struct MenuTextItem cinematics_items[30] = {
    {0x506C, MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD},
    {0x506D, MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD, {1}},
    {0x506E, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {1}},
    {0x506F, MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD, {3}},
    {0x5070, MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD, {5}},
    {0x5071, MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD, {4}},
    {0x5072, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {2}},
    {0x5073, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {3}},
    {0x5074, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {4}},
    {0x5075, MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD, {6}},
    {0x5076, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {6}},
    {0x5077, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {5}},
    {0x5078, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {7}},
    {0x5079, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {8}},
    {0x507A, MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD, {7}},
    {0x507B, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {9}},
    {0x507C, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {0xA}},
    {0x507D, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {0xB}},
    {0x507E, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {0xC}},
    {0x507F, MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD, {8}},
    {0x5080, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {0xD}},
    {0x5081, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {0xE}},
    {0x5082, MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD, {9}},
    {0x5083, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {0xF}},
    {0x5084, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {0x10}},
    {0x5085, MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD, {0xA}},
    {0x5086, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {0x11}},
    {0x5087, MENU_ACTION_REQUEST_CLOSE_WITH_WORD_PAYLOAD, {0x12}},
    {0x5088, MENU_ACTION_REQUEST_CLOSE_4_WITH_WORD_PAYLOAD, {0xB}},
    {0},
};

struct MenuTextListScreen cinematics_list = {
    {      FUN_0021abf8,     FUN_0021d4a8},
    {MENU_LIST_CENTERED, cinematics_items},
};

struct MenuPage in_level_movies_page = {
    {0xA4, 0x9A, 0x9B, 0x9C, 0x9D, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &goodies_page,
    0x28,
    SCREEN(in_level_movies_list),
    {SCREEN(in_level_movies_title_label),
     SCREEN(in_level_movies_list),
     SCREEN(movie_page_hint_list)}
};

struct MenuLabelScreen in_level_movies_title_label = {
    {                                                                             FUN_00219d80,FUN_0021a328},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4FC8  },
};

/* The In-Level Movies lists, one per level (in_level_movie_lists). param.half:
   {lo: menu_system.unkE4, hi: menu_action_messages index, 0 = none}. */
struct MenuTextItem in_level_movies_veldin[4] = {
    {0x5022, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 0}}},
    {0x5023, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 0}}},
    {0x5024, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {3, 0}}},
    {0},
};

struct MenuTextItem in_level_movies_novalis[7] = {
    {0x5025, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {5, 0}}},
    {0x5026, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {3, 0}}},
    {0x5027, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {4, 2}}},
    {0x5028, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {0, 3}}},
    {0x5029, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 3}}},
    {0x502A, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 3}}},
    {0},
};

struct MenuTextItem in_level_movies_aridia[5] = {
    {0x502B, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {0, 16}}},
    {0x502C, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 0}}},
    {0x502D, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 0}}},
    {0x502E, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {3, 0}}},
    {0},
};

struct MenuTextItem in_level_movies_kerwan[8] = {
    {0x502F, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {0, 20}}},
    {0x5030, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 20}}},
    {0x5031, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 21}}},
    {0x5032, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {3, 21}}},
    {0x5033, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {4, 0}}},
    {0x5034, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {5, 24}}},
    {0x5035, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {8, 24}}},
    {0},
};

struct MenuTextItem in_level_movies_eudora[3] = {
    {0x5036, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {0, 36}}},
    {0x5037, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 36}}},
    {0},
};

struct MenuTextItem in_level_movies_rilgar[6] = {
    {0x5038, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {0, 38}}},
    {0x5039, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {9, 38}}},
    {0x503A, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {5, 39}}},
    {0x503B, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {10, 0}}},
    {0x503C, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {11, 0}}},
    {0},
};

struct MenuTextItem in_level_movies_blarg_station[3] = {
    {0x503D, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {0, 47}}},
    {0x503E, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 0}}},
    {0},
};

struct MenuTextItem in_level_movies_umbris[5] = {
    {0x503F, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE},
    {0x5040, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 51}}},
    {0x5041, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 52}}},
    {0x5042, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {4, 53}}},
    {0},
};

struct MenuTextItem in_level_movies_batalia[8] = {
    {0x5043, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE},
    {0x5044, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 55}}},
    {0x5045, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 55}}},
    {0x5046, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {3, 56}}},
    {0x5047, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {4, 56}}},
    {0x5048, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {6, 57}}},
    {0x5049, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {8, 57}}},
    {0},
};

struct MenuTextItem in_level_movies_orxon[3] = {
    {0x504A, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE},
    {0x504B, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 0}}},
    {0},
};

struct MenuTextItem in_level_movies_pokitaru[7] = {
    {0x504C, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {0, 66}}},
    {0x504D, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 66}}},
    {0x504E, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {3, 67}}},
    {0x504F, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {4, 67}}},
    {0x5050, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {5, 68}}},
    {0x5051, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {6, 68}}},
    {0},
};

struct MenuTextItem in_level_movies_hoven[5] = {
    {0x5052, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 0}}},
    {0x5053, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 72}}},
    {0x5054, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {3, 70}}},
    {0x5055, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {4, 70}}},
    {0},
};

struct MenuTextItem in_level_movies_gemlik[5] = {
    {0x5056, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE},
    {0x5057, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 75}}},
    {0x5058, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 0}}},
    {0x5059, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {3, 0}}},
    {0},
};

struct MenuTextItem in_level_movies_oltanis[6] = {
    {0x505A, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {0, 86}}},
    {0x505B, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 87}}},
    {0x505C, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 87}}},
    {0x505D, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {3, 88}}},
    {0x505E, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {4, 88}}},
    {0},
};

struct MenuTextItem in_level_movies_quartu[6] = {
    {0x505F, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE},
    {0x5060, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 0}}},
    {0x5061, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 0}}},
    {0x5062, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {3, 0}}},
    {0x5063, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {4, 0}}},
    {0},
};

struct MenuTextItem in_level_movies_kalebo[4] = {
    {0x5064, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {0, 102}}},
    {0x5065, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {1, 102}}},
    {0x5066, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 101}}},
    {0},
};

struct MenuTextItem in_level_movies_veldin_finale[6] = {
    {0x5067, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE},
    {0x5068, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {2, 0}}},
    {0x5069, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {3, 0}}},
    {0x506A, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {4, 0}}},
    {0x506B, MENU_ACTION_REQUEST_CLOSE_WITH_OPTIONAL_MESSAGE, {.half = {5, 0}}},
    {0},
};

/* By level: assign_pause_level_sprite_list picks [current_level_index % 19]. */
struct MenuTextItem *in_level_movie_lists[19] = {
    in_level_movies_veldin,        in_level_movies_novalis, in_level_movies_aridia,
    in_level_movies_kerwan,        in_level_movies_eudora,  in_level_movies_rilgar,
    in_level_movies_blarg_station, in_level_movies_umbris,  in_level_movies_batalia,
    in_level_movies_gaspar,        in_level_movies_orxon,   in_level_movies_pokitaru,
    in_level_movies_hoven,         in_level_movies_gemlik,  in_level_movies_oltanis,
    in_level_movies_quartu,        in_level_movies_kalebo,  in_level_movies_drek_fleet,
    in_level_movies_veldin_finale,
};

struct MenuTextListScreen in_level_movies_list = {
    {FUN_0021abf8, FUN_0021d4a8, FUN_00221d28},
    {MENU_LIST_FIXED_STEP | MENU_LIST_CENTERED | MENU_LIST_NO_SCROLL},
};

struct MenuPage menu_preview_initial_page = {
    {0, 1, 0x10, 0x11, 0x12, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    0,
    0x2E,
    SCREEN(title_new_game_entry),
    {&menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(title_new_game_entry),
     SCREEN(title_load_game_entry),
     SCREEN(title_options_entry)}
};

struct MenuTextItem title_new_game_entry_items[2] = {
    {0x4F2A, MENU_ACTION_SET_NEXT_PAGE_OR_INITIALIZE_FREEZE, {(s32)&new_game_page}},
    {0},
};

struct MenuTextItem title_load_game_entry_items[2] = {
    {0x4F28, MENU_ACTION_SET_NEXT_PAGE_OR_INITIALIZE_TIMED_FREEZE, {(s32)&title_load_game_page}},
    {0},
};

struct MenuTextItem title_options_entry_items[2] = {
    {0x4EE7, MENU_ACTION_SET_NEXT_PAGE, {(s32)&title_options_page}},
    {0},
};

struct MenuTextListScreen title_new_game_entry = {
    {FUN_0021abf8, FUN_0021b1c8},
    {MENU_LIST_LARGE_FONT,
     title_new_game_entry_items, SCREEN(title_options_entry),
     SCREEN(title_load_game_entry)},
};

struct MenuTextListScreen title_load_game_entry = {
    {FUN_0021abf8, FUN_0021b1c8},
    {MENU_LIST_LARGE_FONT,
     title_load_game_entry_items, SCREEN(title_new_game_entry),
     SCREEN(title_options_entry)},
};

struct MenuTextListScreen title_options_entry = {
    {FUN_0021abf8, FUN_0021b1c8},
    {MENU_LIST_LARGE_FONT,
     title_options_entry_items, SCREEN(title_load_game_entry),
     SCREEN(title_new_game_entry)},
};

struct MenuPage title_options_page = {
    {0, 1, 0xBE, 0xBF, 0xC0, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &menu_preview_initial_page,
    0x2F,
    SCREEN(title_options_list),
    {&menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(options_title_label),
     SCREEN(title_options_list),
     SCREEN(select_exit_hint_list)}
};

struct MenuTextItem title_options_items_with_video[4] = {
    {0x4F2B, MENU_ACTION_SET_NEXT_PAGE, {(s32)&language_page}},
    {0x4F21, MENU_ACTION_SET_NEXT_PAGE, {(s32)&video_page}},
    {0x4F22, MENU_ACTION_SET_NEXT_PAGE, {(s32)&title_sound_page}},
    {0},
};

struct MenuTextItem title_options_items_no_video[3] = {
    {0x4F2B, MENU_ACTION_SET_NEXT_PAGE, {(s32)&language_page}},
    {0x4F22, MENU_ACTION_SET_NEXT_PAGE, {(s32)&title_sound_page}},
    {0},
};

struct MenuTextListScreen title_options_list = {
    {FUN_0021abf8, FUN_0021b1c8, FUN_0021a1b0},
};

struct MenuPage title_sound_page = {
    {0, 1, 0xCA, 0xD2, 0xD7, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &title_options_page,
    0x20,
    SCREEN(sound_volume_screen),
    {&menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(sound_title_label),
     SCREEN(sound_volume_screen),
     SCREEN(toggle_exit_hint_list)}
};

struct MenuPage language_page = {
    {0, 1, 0xC1, 0xC2, 0xC3, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &title_options_page,
    0x30,
    SCREEN(language_list),
    {&menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(language_title_label),
     SCREEN(language_list),
     SCREEN(select_exit_hint_list)}
};

struct MenuLabelScreen language_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F2B},
};

struct MenuTextItem language_items[6] = {
    {0x4F2C, MENU_ACTION_SET_LANGUAGE},
    {0x4F2D, MENU_ACTION_SET_LANGUAGE, {2}},
    {0x4F2F, MENU_ACTION_SET_LANGUAGE, {3}},
    {0x4F2E, MENU_ACTION_SET_LANGUAGE, {4}},
    {0x4F30, MENU_ACTION_SET_LANGUAGE, {5}},
    {0},
};

struct MenuTextListScreen language_list = {
    {                                                         FUN_0021abf8,   FUN_0021d4a8},
    {MENU_LIST_FIXED_STEP | MENU_LIST_LANGUAGE_MARKS | MENU_LIST_NO_SCROLL, language_items},
};

struct MenuPage video_page = {
    {0, 1, 0xCA, 0xCB, 0xC3, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &title_options_page,
    0x31,
    SCREEN(video_option_list),
    {&menu_placeholder_screen,
     &menu_placeholder_screen,
     SCREEN(video_title_label),
     SCREEN(video_option_list),
     SCREEN(select_exit_hint_list)}
};

struct MenuLabelScreen video_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021cae0},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F3A},
};

struct MenuOption video_options[2] = {
    {0x4F3B, &requested_video_mode, 0x4F3C, 0x4F3D, MENU_OPTION_FADE_TOGGLE},
    {0},
};

struct MenuOptionListScreen video_option_list = {
    {FUN_00220e28,  FUN_00221030},
    {           0, video_options},
};

struct MenuPage title_load_game_page = {
    {0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &menu_preview_initial_page,
    0x1F,
    SCREEN(load_game_slot_list),
    {SCREEN(load_game_title_label),
     SCREEN(load_game_slot_list),
     SCREEN(save_slot_image),
     SCREEN(save_slot_selection_text),
     SCREEN(save_page_hint_list)}
};

struct MenuPage new_game_page = {
    {0xC4, 0xC5, 0xC6, 0xC7, 0xC9, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &menu_preview_initial_page,
    0x1F,
    SCREEN(new_game_slot_list),
    {SCREEN(new_game_title_label),
     SCREEN(new_game_slot_list),
     SCREEN(save_slot_image),
     SCREEN(save_slot_selection_text),
     SCREEN(new_game_hint_list)}
};

struct MenuLabelScreen new_game_title_label = {
    {FUN_00219d80, FUN_0021a328, FUN_0021a318},
    {MENU_LABEL_CENTER_X | MENU_LABEL_CENTER_Y | MENU_LABEL_FIXED_TEXT | MENU_LABEL_LARGE_FONT,
     0x4F2A},
};

struct MenuSaveSlotScreen new_game_slot_list = {
    {FUN_002235b8, FUN_002239e0, FUN_00222f18, FUN_00222f58},
};

struct MenuTextItem new_game_hint_items[4] = {
    {0x4F27},
    {0x5232},
    {0x4EE0},
    {0},
};

struct MenuTextListScreen new_game_hint_list = {
    {                                              0,        FUN_0021b1c8},
    {MENU_LIST_FIXED_COLOR | MENU_LIST_CENTER_WIDEST, new_game_hint_items},
};

struct MenuPage new_game_confirm_page = {
    {0x81, 0x82, 0x83, 0x84, 0xD1, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &new_game_page,
    0x1A,
    SCREEN(save_confirm_prompt),
    {SCREEN(new_game_title_label),
     SCREEN(new_game_slot_list),
     &menu_placeholder_screen,
     SCREEN(save_confirm_prompt),
     SCREEN(confirm_hint_list)}
};

struct MenuPage circle_target_menu_page = {
    {0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &game_end_page,
    0x22,
    SCREEN(challenge_save_slot_list),
    {SCREEN(new_game_title_label),
     SCREEN(challenge_save_slot_list),
     SCREEN(save_slot_image),
     SCREEN(save_slot_selection_text),
     SCREEN(save_page_hint_list)}
};

struct MenuSaveSlotScreen challenge_save_slot_list = {
    {FUN_002235b8, FUN_002239e0, FUN_00222f18, FUN_00222f58},
    {0x2000},
};

struct MenuPage challenge_confirm_page = {
    {0x81, 0x82, 0x83, 0x84, 0x85, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD},
    &circle_target_menu_page,
    0x1A,
    SCREEN(save_confirm_prompt),
    {SCREEN(save_game_title_label),
     SCREEN(challenge_save_slot_list),
     &menu_placeholder_screen,
     SCREEN(save_confirm_prompt),
     SCREEN(confirm_hint_list)}
};
