/* Level 01 moby class bounded_scroll_layers (0x2a6): on first use computes the bounding spheres of its two scroll layers, then queues their draw callback. */
#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "qzero.h"
#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/shared/ui/hud/hud_keep_visible.h"

typedef struct {
    Vec4 *pts;
    u8    pad4[4];
    Vec4 *out;
    s32   count;
    u8    pad10[0x50];
} BoundGroup;

s32               D_L01_00161BA0 = 0;
extern BoundGroup D_L01_001E3380[];
void              update_and_draw_scrolling_overlay_layers(s32, void *) __asm__("FUN_L01_002b96e0");

void draw_bounded_scroll_layers(void) __asm__("FUN_L01_002f6150");

/* Draw callback: draws the class's two scroll layers. */
void draw_bounded_scroll_layers(void) {
    mine_blend_color();
    update_and_draw_scrolling_overlay_layers(0x2, D_L01_001E3380);
}

void update_bounded_scroll_layers(void *callback_argument) __asm__("FUN_L01_002f6180");

void update_bounded_scroll_layers(void *callback_argument) {
    Vec4  bounding_sphere;
    Vec4  point_to_center;
    Vec4 *point_cursor;
    s32   i;
    s32   j;
    s32   k;
    f32   r;

    if (D_L01_00161BA0 == 0) {
        D_L01_00161BA0 = 1;
        for (i = 0; i < 2; i++) {
            qzero(&bounding_sphere);
            point_cursor = (D_L01_001E3380 + i)->pts;
            for (j = 0; j < (D_L01_001E3380 + i)->count; j++) {
                fast_vec_add(&bounding_sphere, &bounding_sphere, point_cursor);
                point_cursor++;
            }
            fast_vec_scale(&bounding_sphere,
                           &bounding_sphere,
                           1.0f / convert_integer_to_float((D_L01_001E3380 + i)->count));
            bounding_sphere.f[3] = 0.0f;
            point_cursor = (D_L01_001E3380 + i)->pts;
            for (k = 0; k < (D_L01_001E3380 + i)->count; k++) {
                fast_vec_sub(&point_to_center, &bounding_sphere, point_cursor);
                r = fast_vec_length(&point_to_center);
                if (bounding_sphere.f[3] < r) {
                    bounding_sphere.f[3] = r;
                }
                point_cursor++;
            }
            qcopy((D_L01_001E3380 + i)->out, &bounding_sphere);
        }
    }
    add_draw_callback(draw_bounded_scroll_layers, callback_argument);
}
