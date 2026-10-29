#include "types.h"
extern s32 sound_probe_ignore[] __asm__("D_0013E5BC");
extern u8 D_00187080[];
extern char D_00194120[];
extern s32 collision_line() __asm__("func_001EFA68");
extern void fast_vec_add() __asm__("func_001F9A10");
extern void fast_vec_sub() __asm__("func_001F9A28");
extern void fast_vec_scale(s32, s32, f32) __asm__("func_001F9A68");
extern void random_spherical_offset(f32, f32) __asm__("func_00213358");

void clamp_voice_position_to_collision(s32 position) __asm__("FUN_0022c5a8");

void clamp_voice_position_to_collision(s32 position) {
    random_spherical_offset(0.5f, 6.0f);
    fast_vec_add(position, position, D_00187080);
    if (collision_line(D_00187080, position, 0x82, sound_probe_ignore[0], 0) != 0) {
        fast_vec_sub(position, D_00194120, D_00187080);
        fast_vec_scale(position, position, 0.75f);
        fast_vec_add(position, position, D_00187080);
    }
}

extern __typeof__(clamp_voice_position_to_collision) func_0022C5A8
    __attribute__((alias("FUN_0022c5a8")));
