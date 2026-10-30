#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/camera/camera_globals.h"

extern f32 clamp_float(f32, f32, f32) __asm__("FUN_001f99e8");
s32 calculate_voice_pan(s32 unused, f32 *position, void *listener_matrix) __asm__("FUN_0022c830");

s32 calculate_voice_pan(s32 unused, f32 *position, void *listener_matrix) {
    f32 listener_relative_position[4] __attribute__((aligned(16)));
    f32 pan_scale;

    fast_vec_sub(listener_relative_position, position, camera_position);
    transform_vector(listener_relative_position, listener_relative_position, listener_matrix);
    pan_scale = clamp_float(vector_length_xy(listener_relative_position) - 1.0f, 0.0f, 1.0f);
    return truncate_float_to_s32(
        -fast_arc_tan(listener_relative_position[0], listener_relative_position[1]) * 180.0f *
        pan_scale * 0.31830987f);
}

extern __typeof__(calculate_voice_pan) func_0022C830 __attribute__((alias("FUN_0022c830")));
