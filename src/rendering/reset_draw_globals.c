#include "types.h"
extern s32 D_0015F330;
extern s32 D_0015F334;
extern s32 screen_overlay_enabled __asm__("D_0015F34C") __attribute__((sda));
extern s32 D_0015F360 __attribute__((sda));
extern s32 occl_debug_overlay_enabled __asm__("D_0015F370") __attribute__((sda));
extern s32 occl_chain_active __asm__("D_0015F444");
extern s32 occl_chain_frames __asm__("D_0015F448");
extern s32 draw_callback_count __asm__("D_0015F464");
extern s32 draw_callback2_count __asm__("D_0015F468");
extern s32 draw_callback3_count __asm__("D_0015F46C");
extern s32 draw_callback4_count __asm__("D_0015F470");
extern s32 effect_quad_count __asm__("D_0015F474");
extern s32 D_0015F648;
extern s32 D_00161190;
extern s32 occl_cell_table __asm__("D_00161194");
extern s32 occl_cell_count __asm__("D_00161198");
extern s32 moby_occl_cluster_count __asm__("D_0016119C") __attribute__((sda));

void reset_draw_globals(void) __asm__("FUN_001f37e8");

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
    D_00161190 = 0;
    occl_cell_table = 0;
    occl_cell_count = 0;
    moby_occl_cluster_count = 0;
}

extern __typeof__(reset_draw_globals) func_001F37E8 __attribute__((alias("FUN_001f37e8")));
