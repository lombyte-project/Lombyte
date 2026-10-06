#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00237a78/FUN_00237a78.s", FUN_00237a78);
#else
#include "types.h"
#include "eetypes.h"
struct ProjectionScreenOrigin {
    u8 pad_0[8];
    s32 x;
    s32 y;
};
extern struct ProjectionScreenOrigin screen_offsets __asm__("D_0013E500");
extern volatile f32 view_context[] __asm__("D_0018CD00");
extern u8 camera_position[] __asm__("D_00187080");
extern void fast_vec_sub(void *, void *, void *) __asm__("func_001F9A28");
extern void fast_vec_scale(void *, void *, f32) __asm__("func_001F9A68");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern s32 convert_float_to_integer(f32) __asm__("func_001FA6D0");

void project_graphics_bounds(f32 *first, f32 *opposite, s32 *width, s32 *height, s32 *x,
                             s32 *y) __asm__("FUN_00237a78");

/* Project two world-space corners, then round their screen origin and extent. */
void project_graphics_bounds(f32 *first, f32 *opposite, s32 *width, s32 *height, s32 *x, s32 *y) {
    f32 first_projected[4] __attribute__((aligned(16)));
    f32 opposite_projected[4] __attribute__((aligned(16)));
    f32 *opposite_pointer = opposite_projected;
    f32 scale_x;
    f32 scale_y;

    *(u128 *)first_projected = *(u128 *)first;
    *(u128 *)opposite_projected = *(u128 *)opposite;
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
    scale_y = view_context[101];
    scale_x = view_context[100];
    first_projected[0] *= scale_x;
    first_projected[1] *= scale_y;
    opposite_pointer[0] *= scale_x;
    opposite_pointer[1] *= scale_y;
    *x = convert_float_to_integer(first_projected[0] * 0.25f + (f32)screen_offsets.x);
    *y = convert_float_to_integer(first_projected[1] * 0.25f + (f32)screen_offsets.y);
    *(volatile s32 *)width =
        convert_float_to_integer((opposite_pointer[0] - first_projected[0]) * 0.25f);
    {
        f32 first_y = *(volatile f32 *)&first_projected[1];
        *height =
            convert_float_to_integer((*(volatile f32 *)&opposite_pointer[1] - first_y) * 0.25f);
    }
}

extern __typeof__(project_graphics_bounds) func_00237A78 __attribute__((alias("FUN_00237a78")));

#endif /* NON_MATCHING */
