#include "types.h"
extern f32 D_00187080[4];
extern void subtract_vector_xyz(f32 *, f32 *, f32 *) __asm__("FUN_001f9a28");
extern void transform_vector(f32 *, f32 *, void *) __asm__("FUN_001f9d20");
extern f32 vector_length_xy(f32 *) __asm__("FUN_001f9b20");
extern f32 clamp_float(f32, f32, f32) __asm__("FUN_001f99e8");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
s32 calculate_voice_pan(s32 unused, f32 *position, void *listener_matrix) __asm__("FUN_0022c830");

s32 calculate_voice_pan(s32 unused, f32 *position, void *listener_matrix) {
    f32 listener_relative_position[4] __attribute__((aligned(16)));
    f32 pan_scale;

    subtract_vector_xyz(listener_relative_position, position, D_00187080);
    transform_vector(listener_relative_position, listener_relative_position, listener_matrix);
    pan_scale = clamp_float(vector_length_xy(listener_relative_position) - 1.0f, 0.0f, 1.0f);
    return truncate_float_to_s32(
        -fast_arc_tan(listener_relative_position[0], listener_relative_position[1]) * 180.0f *
        pan_scale * 0.31830987f);
}

extern __typeof__(calculate_voice_pan) func_0022C830 __attribute__((alias("FUN_0022c830")));
