#include "types.h"

extern f32 func_001F9B48(s32, u32);
extern s32 FUN_0022c6f8(f32 *, f32, f32, f32);
extern u8 D_00187080[];

s32 calculate_voice_volume(s32 *voice, s32 position) __asm__("FUN_0022c7e8");

s32 calculate_voice_volume(s32 *voice, s32 position) {
    f32 distance = func_001F9B48(position, (u32)D_00187080);
    f32 *definition_values = (f32 *)voice[2];

    return FUN_0022c6f8(definition_values, distance, definition_values[0], definition_values[1]);
}
/* Recovered original symbol name. */
extern __typeof__(calculate_voice_volume) sound_GetFade__FP9SoundDataP4vec4 __attribute__((alias("FUN_0022c7e8")));
