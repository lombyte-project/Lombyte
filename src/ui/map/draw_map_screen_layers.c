#include "types.h"
#include "rnc/ui/map/map_state.h"
#include "rnc/rendering/fs_aa_buffer.h"

#include "rnc/game/game_globals.h"
#include "rnc/rendering/packets.h"
#include "rnc/world/level_id.h"

extern s32 map_layer_screen_insets[LEVEL_COUNT] __asm__("D_001DDF68");

extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

s32 draw_map_screen_layers(void) __asm__("FUN_00220850");

s32 draw_map_screen_layers(void) {
    s32 loaded_map_index = level_map_selection.loaded;
    s32 screen_inset;
    s32 screen_extent_reduction;
    s32 map_adjusted_frame_count;
    s32 ph;

    if (loaded_map_index < 0) {
        return 0;
    }
    if (loaded_map_index == 6 || loaded_map_index == 13 || loaded_map_index == 17) {
        vu1_add_g_sregister(0x47, 0);
        vu1_add_g_sregister(8, 5);
        append_power_of_two_textured_screen_sprite(level_map_selection.tex0,
                                                   0,
                                                   0,
                                                   7,
                                                   7,
                                                   fs_aa_buffer.target_width << 4,
                                                   fs_aa_buffer.target_height << 4,
                                                   0,
                                                   0,
                                                   0x80);
        vu1_add_g_sregister(0x47, 0x360B);
        if (game_frame_counter % 60 < 40) {
            append_power_of_two_textured_screen_sprite(level_map_selection.tex1,
                                                       0,
                                                       0,
                                                       7,
                                                       7,
                                                       fs_aa_buffer.target_width << 4,
                                                       fs_aa_buffer.target_height << 4,
                                                       0,
                                                       0,
                                                       0x80);
        }
        if (game_frame_counter % 150 < 90) {
            append_power_of_two_textured_screen_sprite(level_map_selection.tex2,
                                                       0,
                                                       0,
                                                       7,
                                                       7,
                                                       fs_aa_buffer.target_width << 4,
                                                       fs_aa_buffer.target_height << 4,
                                                       0,
                                                       0,
                                                       0x80);
        }
    } else {
        map_adjusted_frame_count = game_frame_counter + loaded_map_index * 0x2AB;
        screen_inset = map_layer_screen_insets[loaded_map_index];
        screen_extent_reduction = screen_inset * 2;
        ph = map_adjusted_frame_count % 0x800;
        vu1_add_g_sregister(0x47, 0);
        vu1_add_g_sregister(8, 0);
        append_power_of_two_textured_screen_sprite(
            level_map_selection.tex0,
            screen_inset,
            screen_inset,
            7,
            7,
            (fs_aa_buffer.target_width << 4) - screen_extent_reduction,
            (fs_aa_buffer.target_height << 4) - screen_extent_reduction,
            ph,
            0,
            0x80);
        vu1_add_g_sregister(0x47, 0x360B);
        vu1_add_g_sregister(8, 5);
        append_power_of_two_textured_screen_sprite(
            level_map_selection.tex1,
            screen_inset,
            screen_inset,
            7,
            7,
            (fs_aa_buffer.target_width << 4) - screen_extent_reduction,
            (fs_aa_buffer.target_height << 4) - screen_extent_reduction,
            0,
            0,
            0x80);
        append_power_of_two_textured_screen_sprite(
            level_map_selection.tex2,
            screen_inset,
            screen_inset,
            7,
            7,
            (fs_aa_buffer.target_width << 4) - screen_extent_reduction,
            (fs_aa_buffer.target_height << 4) - screen_extent_reduction,
            0,
            0,
            0x80);
    }
    return 4;
}

extern __typeof__(draw_map_screen_layers) func_00220850 __attribute__((alias("FUN_00220850")));

/* How far (in 1/16 pixels) each level's map layers sit inside the screen edges; the three
   levels without a map (space) have none. */
s32 map_layer_screen_insets[LEVEL_COUNT] = {
    [LEVEL_VELDIN1] = 1200, [LEVEL_NOVALIS] = 1000,    [LEVEL_ARIDIA] = 1300,
    [LEVEL_KERWAN] = 400,   [LEVEL_EUDORA] = 1100,     [LEVEL_RILGAR] = 900,
    [LEVEL_NEBULA_G34] = 0, [LEVEL_UMBRIS] = 1200,     [LEVEL_BATALIA] = 700,
    [LEVEL_GASPAR] = 190,   [LEVEL_ORXON] = 800,       [LEVEL_POKITARU] = 1550,
    [LEVEL_HOVEN] = 300,    [LEVEL_OLTANIS_ORBIT] = 0, [LEVEL_OLTANIS] = 600,
    [LEVEL_QUARTU] = 900,   [LEVEL_KALEBO3] = 500,     [LEVEL_VELDIN_ORBIT] = 0,
    [LEVEL_VELDIN2] = 1200,
};
