#include "types.h"

extern f32 fast_cos(f32) __asm__("func_001F9DC8");

f32 cosine_interpolate(s32 arg0, f32 from, f32 to, f32 t) __asm__("FUN_002133d0");

f32 cosine_interpolate(s32 arg0, f32 from, f32 to, f32 t) {
    if (t == 0.0f) {
        return from;
    }
    if (t == 1.0f) {
        return to;
    }
    return from + (to - from) * ((1.0f - fast_cos(t * 3.1415927f)) * 0.5f);
}
