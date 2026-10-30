#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"
extern s32 D_0015F330;
extern s32 D_0015F334;
extern s32 D_0015F360 __attribute__((sda));

void reset_draw_globals(void) __asm__("FUN_001f37e8");

extern s32 moby_occl_cluster_count_gp __asm__("D_0016119C") __attribute__((sda));
void reset_draw_globals(void) {
    draw_callback_count = 0;
    draw_callback3_count = 0;
    draw_callback4_count = 0;
    draw_callback2_count = 0;
    effect_quad_count = 0;
    D_0015F330 = 0;
    D_0015F334 = 0;
    occl_chain_active = 0;
    occl_chain_frames = 0;
    screen_overlay_enabled = 0;
    D_0015F360 = 0;
    occl_debug_overlay_enabled = 0;
    D_0015F648 = 0;
    surface_height_tiles = 0;
    occl_cell_table = 0;
    occl_cell_count = 0;
    moby_occl_cluster_count_gp = 0;
}

extern __typeof__(reset_draw_globals) func_001F37E8 __attribute__((alias("FUN_001f37e8")));
