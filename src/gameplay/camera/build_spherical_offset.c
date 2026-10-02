#include "types.h"

extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");

void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b)
{
    out[0] = fast_cos(a) * scale * fast_cos(b);
    out[1] = fast_sin(a) * scale * fast_cos(b);
    out[2] = fast_sin(b) * scale;
}

extern __typeof__(build_spherical_offset) func_00214DB0 __attribute__((alias("FUN_00214db0")));
