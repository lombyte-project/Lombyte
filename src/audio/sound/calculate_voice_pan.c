#include "types.h"
extern f32 D_00187080[4];
extern void FUN_001f9a28(f32 *, f32 *, f32 *);
extern void FUN_001f9d20(f32 *, f32 *, void *);
extern f32 FUN_001f9b20(f32 *);
extern f32 FUN_001f99e8(f32, f32, f32);
extern f32 FUN_001f9e90(f32, f32);
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
s32 calculate_voice_pan(s32 unused, f32 *position, void *listener_matrix) __asm__("FUN_0022c830");

s32 calculate_voice_pan(s32 unused, f32 *position, void *listener_matrix) {
    f32 listener_relative_position[4] __attribute__((aligned(16)));
    f32 pan_scale;

    FUN_001f9a28(listener_relative_position, position, D_00187080);
    FUN_001f9d20(listener_relative_position, listener_relative_position, listener_matrix);
    pan_scale = FUN_001f99e8(FUN_001f9b20(listener_relative_position) - 1.0f, 0.0f, 1.0f);
    return truncate_float_to_s32(-FUN_001f9e90(listener_relative_position[0], listener_relative_position[1]) * 180.0f * pan_scale * 0.31830987f);
}

extern __typeof__(calculate_voice_pan) func_0022C830 __attribute__((alias("FUN_0022c830")));
