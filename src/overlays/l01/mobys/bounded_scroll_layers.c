/* Level 01 moby class bounded_scroll_layers (0x2a6): on first use computes the bounding spheres of its two scroll layers, then queues their draw callback. */
#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "qzero.h"
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"
#include "rnc/overlay/scroll_layer.h"

s32  bounded_scroll_layers_bounds_computed __asm__("D_L01_00161BA0") = 0;
void update_and_draw_scrolling_overlay_layers(s32, void *) __asm__("FUN_L01_002b96e0");

void draw_bounded_scroll_layers(void) __asm__("FUN_L01_002f6150");

/* Draw callback: draws the class's two scroll layers. */
void draw_bounded_scroll_layers(void) {
    hud_counter_update();
    update_and_draw_scrolling_overlay_layers(0x2, bounded_scroll_layers);
}

void bounded_scroll_layers_update(void *callback_argument) __asm__("FUN_L01_002f6180");

/* Moby class 678 */
void bounded_scroll_layers_update(void *callback_argument) {
    Vec4   bounding_sphere;
    Vec4   point_to_center;
    Vec4f *point_cursor;
    s32    i;
    s32    j;
    s32    k;
    f32    r;

    if (bounded_scroll_layers_bounds_computed == 0) {
        bounded_scroll_layers_bounds_computed = 1;
        for (i = 0; i < 2; i++) {
            qzero(&bounding_sphere);
            point_cursor = (bounded_scroll_layers + i)->verts;
            for (j = 0; j < (bounded_scroll_layers + i)->count; j++) {
                fast_vec_add(&bounding_sphere, &bounding_sphere, point_cursor);
                point_cursor++;
            }
            fast_vec_scale(&bounding_sphere,
                           &bounding_sphere,
                           1.0f / convert_integer_to_float((bounded_scroll_layers + i)->count));
            bounding_sphere.f[3] = 0.0f;
            point_cursor = (bounded_scroll_layers + i)->verts;
            for (k = 0; k < (bounded_scroll_layers + i)->count; k++) {
                fast_vec_sub(&point_to_center, &bounding_sphere, point_cursor);
                r = fast_vec_length(&point_to_center);
                if (bounding_sphere.f[3] < r) {
                    bounding_sphere.f[3] = r;
                }
                point_cursor++;
            }
            qcopy((bounded_scroll_layers + i)->bounds, &bounding_sphere);
        }
    }
    add_draw_callback(draw_bounded_scroll_layers, callback_argument);
}

Vec4f bounded_scroll_layer_0_bounds __asm__("D_L01_00161BB0") = {0};

Vec4f bounded_scroll_layer_1_bounds __asm__("D_L01_00161BC0") = {0};

Vec4f bounded_scroll_layer_0_verts[4] __asm__("D_L01_001E32C0") = {
    { 265.72f, 171.94f, 82.25f, 82.25f},
    {2.6e+02f, 163.57f, 82.25f, 82.25f},
    { 300.41f, 147.32f, 82.25f, 82.25f},
    {  293.8f,  138.5f, 82.25f, 82.25f},
};

f32 bounded_scroll_layer_0_uvs[4][2] __asm__("D_L01_001E3300") = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
};

Vec4f bounded_scroll_layer_1_verts[4] __asm__("D_L01_001E3320") = {
    { 274.3f, 183.83f, 82.25f, 82.25f},
    {269.12f, 176.54f, 82.25f, 82.25f},
    {308.61f, 159.26f, 82.25f, 82.25f},
    {303.21f, 151.42f, 82.25f, 82.25f},
};

f32 bounded_scroll_layer_1_uvs[4][2] __asm__("D_L01_001E3360") = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
};

ScrollLayer bounded_scroll_layers[2] = {
    {.verts = bounded_scroll_layer_0_verts,
     .uvs = bounded_scroll_layer_0_uvs,
     .bounds = &bounded_scroll_layer_0_bounds,
     .count = 4,
     .rate0 = 0.0012f,
     .rate1 = 0.0007f,
     .unk18 = 0.007f,
     .frame_step = 1,
     .du0 = -0.1f,
     .dv0 = -0.9f,
     .du1 = 0.1f,
     .dv1 = -0.9f,
     .color = 0xb8a0a0,
     .tex0 = 0x2c,
     .tex1 = 0x2c,
     .alpha0 = 0x30,
     .alpha1 = 0x30},
    {.verts = bounded_scroll_layer_1_verts,
     .uvs = bounded_scroll_layer_1_uvs,
     .bounds = &bounded_scroll_layer_1_bounds,
     .count = 4,
     .rate0 = 0.0012f,
     .rate1 = 0.0007f,
     .unk18 = 0.007f,
     .frame_step = 1,
     .du0 = -0.1f,
     .dv0 = -0.9f,
     .du1 = 0.1f,
     .dv1 = -0.9f,
     .color = 0xb8a0a0,
     .tex0 = 0x2c,
     .tex1 = 0x2c,
     .alpha0 = 0x30,
     .alpha1 = 0x30},
};
