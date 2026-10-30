#include "types.h"
#include "rnc/ui/map/map_state.h"
#include "rnc/rendering/fs_aa_buffer.h"

#include "rnc/game/game_globals.h"

extern s32  map_layer_screen_insets[] __asm__("D_001DDF68");
extern void append_power_of_two_textured_screen_sprite(
    u64, s32, s32, s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_00200468");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

s32 draw_map_screen_layers(void) __asm__("FUN_00220850");

s32 draw_map_screen_layers(void) {
    s32 loaded_map_index = level_map_selection.loaded;
    s32 size;
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
        size = map_layer_screen_insets[loaded_map_index];
        screen_extent_reduction = size * 2;
        ph = map_adjusted_frame_count % 0x800;
        vu1_add_g_sregister(0x47, 0);
        vu1_add_g_sregister(8, 0);
        append_power_of_two_textured_screen_sprite(
            level_map_selection.tex0,
            size,
            size,
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
            size,
            size,
            7,
            7,
            (fs_aa_buffer.target_width << 4) - screen_extent_reduction,
            (fs_aa_buffer.target_height << 4) - screen_extent_reduction,
            0,
            0,
            0x80);
        append_power_of_two_textured_screen_sprite(
            level_map_selection.tex2,
            size,
            size,
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
