#include "types.h"
#include "rnc/math/vector.h"
#include "rnc/rendering/screen.h"
#include "rnc/rendering/view.h"
#include "rnc/camera/camera_globals.h"

extern void fast_vec_sub(void *, void *, void *) __asm__("func_001F9A28");
extern void fast_vec_scale(void *, void *, f32) __asm__("func_001F9A68");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern s32  truncate_float_to_s32(f32) __asm__("func_001FA6D0");

void project_graphics_bounds(
    u128 *first, u128 *opposite, s32 *width, s32 *height, s32 *x, s32 *y) __asm__("FUN_00237a78");

void project_graphics_bounds(u128 *first, u128 *opposite, s32 *width, s32 *height, s32 *x, s32 *y) {
    Vec4  first_projected;
    Vec4  opposite_projected;
    Vec4 *opposite_pointer;
    f32   scale_x;
    f32   scale_y;
    s32   output_byte;

    opposite_pointer = &opposite_projected;
    first_projected.q = *first;
    opposite_pointer->q = *opposite;
    /* Fakematch: combine folds this never-true byte test; until then the second set
       hides opposite_pointer's stack base from alias analysis. */
    output_byte = *(u8 *)x;
    if (output_byte < 0) {
        opposite_pointer = (Vec4 *)x;
        opposite_pointer->f[0] = 0.0f;
        opposite_pointer->f[1] = 0.0f;
        return;
    }
    fast_vec_sub(&first_projected, &first_projected, camera_position);
    fast_vec_sub(opposite_pointer, opposite_pointer, camera_position);
    fast_vec_scale(&first_projected, &first_projected, 1024.0f);
    fast_vec_scale(opposite_pointer, opposite_pointer, 1024.0f);
    opposite_pointer->f[3] = 1.0f;
    first_projected.f[3] = 1.0f;
    transform_vector(&first_projected, &first_projected, camera_position - 0x40);
    transform_vector(opposite_pointer, opposite_pointer, camera_position - 0x40);
    first_projected.f[0] *= 1.0f / first_projected.f[3];
    first_projected.f[1] *= 1.0f / first_projected.f[3];
    opposite_pointer->f[0] *= 1.0f / opposite_pointer->f[3];
    opposite_pointer->f[1] *= 1.0f / opposite_pointer->f[3];
    scale_x = view_context.screen_scale.f[0];
    scale_y = view_context.screen_scale.f[1];
    first_projected.f[0] *= scale_x;
    first_projected.f[1] *= scale_y;
    opposite_pointer->f[0] *= scale_x;
    opposite_pointer->f[1] *= scale_y;
    *x = truncate_float_to_s32(first_projected.f[0] * 0.25f + (f32)screen_extent.half_width);
    *y = truncate_float_to_s32(first_projected.f[1] * 0.25f + (f32)screen_extent.half_height);
    *width = truncate_float_to_s32((opposite_pointer->f[0] - first_projected.f[0]) * 0.25f);
    *height = truncate_float_to_s32((opposite_pointer->f[1] - first_projected.f[1]) * 0.25f);
}

extern __typeof__(project_graphics_bounds) func_00237A78 __attribute__((alias("FUN_00237a78")));
