#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/camera/camera_globals.h"

extern f32 fast_vec_dist(s32, u32) __asm__("func_001F9B48");
extern s32 calculate_voice_distance_volume(f32 *, f32, f32, f32) __asm__("FUN_0022c6f8");

s32 calculate_voice_volume(s32 *voice, s32 position) __asm__("FUN_0022c7e8");

s32 calculate_voice_volume(s32 *voice, s32 position) {
    f32  distance = fast_vec_dist(position, (u32)camera_position);
    f32 *definition_values = (f32 *)voice[2];

    return calculate_voice_distance_volume(
        definition_values, distance, definition_values[0], definition_values[1]);
}

/* Recovered original symbol name. */
extern __typeof__(calculate_voice_volume) sound_GetFade__FP9SoundDataP4vec4
    __attribute__((alias("FUN_0022c7e8")));
