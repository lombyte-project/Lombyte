#include "types.h"

extern f32 fast_cos(f32) __asm__("func_001F9DC8");

f32 cosine_interpolate(s32 unused_arg,
                       f32 from,
                       f32 to,
                       f32 interpolation_progress) __asm__("FUN_002133d0");

f32 cosine_interpolate(s32 unused_arg, f32 from, f32 to, f32 interpolation_progress) {
    if (interpolation_progress == 0.0f) {
        return from;
    }
    if (interpolation_progress == 1.0f) {
        return to;
    }
    return from + (to - from) * ((1.0f - fast_cos(interpolation_progress * 3.1415927f)) * 0.5f);
}
