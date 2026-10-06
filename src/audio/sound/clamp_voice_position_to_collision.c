#include "types.h"
extern s32 D_0013E5BC[];
extern u8 D_00187080[];
extern u8 D_00194120[];
extern s32 func_001EFA68();
extern void func_001F9A10();
extern void func_001F9A28();
extern void func_001F9A68(s32, s32, f32);
extern void random_spherical_offset(f32, f32) __asm__("func_00213358");

void clamp_voice_position_to_collision(s32 position) __asm__("FUN_0022c5a8");

void clamp_voice_position_to_collision(s32 position) {
    random_spherical_offset(0.5f, 6.0f);
    func_001F9A10(position, position, D_00187080);
    if (func_001EFA68(D_00187080, position, 0x82, D_0013E5BC[0], 0) != 0) {
        func_001F9A28(position, D_00194120, D_00187080);
        func_001F9A68(position, position, 0.75f);
        func_001F9A10(position, position, D_00187080);
    }
}

extern __typeof__(clamp_voice_position_to_collision) func_0022C5A8
    __attribute__((alias("FUN_0022c5a8")));
