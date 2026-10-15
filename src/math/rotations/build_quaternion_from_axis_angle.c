#include "types.h"
struct Quaternion {
    u8 pad_0[0xC];
    f32 unkC;
};

extern s32 fast_vec_scale(s32, s32, f32) __asm__("func_001F9A68");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
void build_quaternion_from_axis_angle(struct Quaternion *quat, s32 axis,
                                      f32 angle) __asm__("FUN_00214530");

void build_quaternion_from_axis_angle(struct Quaternion *quat, s32 axis, f32 angle) {
    f32 half_angle;

    half_angle = angle * 0.5f;
    fast_vec_scale(quat, axis, fast_sin(half_angle));
    quat->unkC = fast_cos(half_angle);
}
