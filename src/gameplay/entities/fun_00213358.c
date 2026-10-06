#include "types.h"
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
void FUN_00213358(void *out, f32 a, f32 b) {
    f32 x = random_angle_radians();
    f32 y = random_angle_radians();

    build_spherical_offset(out, random_float_between(a, b), x, y);
}

extern __typeof__(FUN_00213358) func_00213358 __attribute__((alias("FUN_00213358")));
