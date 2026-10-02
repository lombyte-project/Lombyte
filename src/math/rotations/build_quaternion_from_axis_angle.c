#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0xC];
    f32 unkC;
};

extern s32 func_001F9A68(s32, s32, f32);
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
void build_quaternion_from_axis_angle(struct M2c_arg0 *arg0, s32 arg1, f32 fparg0) __asm__("FUN_00214530");

void build_quaternion_from_axis_angle(struct M2c_arg0 *arg0, s32 arg1, f32 fparg0) {
    f32 temp_f20_9;

    temp_f20_9 = fparg0 * 0.5f;
    func_001F9A68(arg0, arg1, fast_sin(temp_f20_9));
    arg0->unkC = fast_cos(temp_f20_9);
}

