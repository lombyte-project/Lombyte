#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00237c80/FUN_00237c80.s", FUN_00237c80);
#else
#include "types.h"
#include "eetypes.h"

struct ProjectionViewport {
    u8 pad_0[0x190];
    f32 scale_x;
    f32 scale_y;
};


extern u8 camera_position[] __asm__("D_00187080");
extern struct ProjectionViewport view_context __asm__("D_0018CD00");
#include "rnc/rendering/screen.h"

extern void fast_vec_sub(void *, void *, void *) __asm__("func_001F9A28");
extern void fast_vec_scale(void *, void *, f32) __asm__("func_001F9A68");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");

void project_graphics_bounds_float(u128 *first, u128 *opposite, f32 *width, f32 *height, f32 *x,
                                   f32 *y) __asm__("FUN_00237c80");

/* Project opposing world bounds into a floating-point screen rectangle. The first
 * point supplies x/y; subtracting it from the opposite point supplies width/height.
 * Keep the four output stores in retail order because callers may alias outputs.
 */
void project_graphics_bounds_float(u128 *first, u128 *opposite, f32 *width, f32 *height, f32 *x,
                                   f32 *y) {
    f32 first_projected[4] __attribute__((aligned(16)));
    f32 opposite_projected[4] __attribute__((aligned(16)));
    f32 *opposite_pointer;
    f32 scale_x;
    f32 scale_y;

    opposite_pointer = opposite_projected;
    *(u128 *)first_projected = *first;
    *(u128 *)opposite_pointer = *opposite;
    fast_vec_sub(first_projected, first_projected, camera_position);
    fast_vec_sub(opposite_pointer, opposite_pointer, camera_position);
    fast_vec_scale(first_projected, first_projected, 1024.0f);
    fast_vec_scale(opposite_pointer, opposite_pointer, 1024.0f);
    opposite_pointer[3] = 1.0f;
    first_projected[3] = 1.0f;
    transform_vector(first_projected, first_projected, camera_position - 0x40);
    transform_vector(opposite_pointer, opposite_pointer, camera_position - 0x40);
    first_projected[0] *= 1.0f / first_projected[3];
    first_projected[1] *= 1.0f / first_projected[3];
    opposite_pointer[0] *= 1.0f / opposite_pointer[3];
    *(volatile f32 *)&opposite_pointer[1] *= 1.0f / opposite_pointer[3];
    scale_y = *(volatile f32 *)&view_context.scale_y;
    scale_x = *(volatile f32 *)&view_context.scale_x;
    first_projected[0] *= scale_x;
    first_projected[1] *= scale_y;
    opposite_pointer[0] *= scale_x;
    opposite_pointer[1] *= scale_y;
    *x = first_projected[0] * 0.25f + (f32)screen_extent.half_width;
    *y = first_projected[1] * 0.25f + (f32)screen_extent.half_height;
    *(volatile f32 *)width = (opposite_pointer[0] - first_projected[0]) * 0.25f;
    *height = (*(volatile f32 *)&opposite_pointer[1] - first_projected[1]) * 0.25f;
}

extern __typeof__(project_graphics_bounds_float) func_00237C80
    __attribute__((alias("FUN_00237c80")));

#endif /* NON_MATCHING */
