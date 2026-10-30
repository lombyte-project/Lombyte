/* Level 01 moby class rippling_scroll_layers (0x2f9): lowers and ripples its four scroll layers once, then pulses their glow and queues their draw callback. */
#include "types.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/cuboid.h"
#include "sda.h"
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"

extern s32 level_frame_count __asm__("D_L01_0015F5CC") MACRO_ADDR;

/* The four scrolling layers draw_rippling_scroll_layers draws (0x60 bytes each, the ScrollLayer layout of l01/scroll_layers.c). */
typedef struct {
    Vec4 *verts;
    u8    pad4[8];
    s32   count;
    u8    pad10[0x4C];
    f32   glow;
} WaterLayer;

extern WaterLayer D_L01_001FBC80[];
s32               D_L01_00161CE0 = 0;
extern s32        level_frame_count __asm__("D_L01_0015F5CC");
void              update_and_draw_scrolling_overlay_layers(s32, void *) __asm__("FUN_L01_002b96e0");

void draw_rippling_scroll_layers(void) __asm__("FUN_L01_002feb28");

/* Draw callback: draws the class's four scroll layers. */
void draw_rippling_scroll_layers(void) {
    mine_blend_color();
    update_and_draw_scrolling_overlay_layers(0x4, D_L01_001FBC80);
}

/* On first use, lowers and ripples the layers' vertices; every frame, pulses their glow and queues draw_rippling_scroll_layers. */
void update_rippling_scroll_layers(void *callback_argument) __asm__("FUN_L01_002feb58");

void update_rippling_scroll_layers(void *callback_argument) {
    s32   i;
    s32   glow_layer_index;
    s32   j;
    s32   k;
    s32   ripple_layer_index;
    s32   vertex_pair_start_index;
    Vec4 *lowered_vertex;
    Vec4 *ripple_vertex_pair;
    f32   glow_factor;

    if (D_L01_00161CE0 == 0) {
        D_L01_00161CE0 = 1;
        for (i = 0; i < 4; i++) {
            lowered_vertex = (D_L01_001FBC80 + i)->verts;
            for (j = 0; j < (D_L01_001FBC80 + i)->count; j++) {
                lowered_vertex->f[3] = lowered_vertex->f[2] = lowered_vertex->f[2] - 0.45f;
                lowered_vertex++;
            }
        }
        k = 1;
        for (ripple_layer_index = 0; ripple_layer_index < 4; ripple_layer_index++) {
            k--;
            ripple_vertex_pair = (D_L01_001FBC80 + ripple_layer_index)->verts;
            for (vertex_pair_start_index = 0;
                 vertex_pair_start_index < (D_L01_001FBC80 + ripple_layer_index)->count;
                 vertex_pair_start_index += 2) {
                if (k & 1) {
                    ripple_vertex_pair[0].f[2] += 0.05f;
                    ripple_vertex_pair[0].f[3] -= 0.05f;
                    ripple_vertex_pair[1].f[2] -= 0.05f;
                    ripple_vertex_pair[1].f[3] += 0.05f;
                } else {
                    ripple_vertex_pair[0].f[2] -= 0.05f;
                    ripple_vertex_pair[0].f[3] += 0.05f;
                    ripple_vertex_pair[1].f[2] += 0.05f;
                    ripple_vertex_pair[1].f[3] -= 0.05f;
                }
                ripple_vertex_pair += 2;
                k++;
            }
        }
    }
    glow_factor =
        fast_sin((convert_integer_to_float(level_frame_count & 0x3F) - 32.0f) * 0.09817477f);
    glow_factor = glow_factor * 0.5f + 0.5f;
    for (glow_layer_index = 3; glow_layer_index >= 0; glow_layer_index--) {
        D_L01_001FBC80[glow_layer_index].glow = glow_factor;
    }
    add_draw_callback(draw_rippling_scroll_layers, callback_argument);
}
