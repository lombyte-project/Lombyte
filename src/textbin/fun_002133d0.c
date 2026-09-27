#include "types.h"

extern f32 func_001F9DC8(f32);

f32 FUN_002133d0(s32 arg0, f32 from, f32 to, f32 t)
{
    if (t == 0.0f) {
        return from;
    }
    if (t == 1.0f) {
        return to;
    }
    return from + (to - from) * ((1.0f - func_001F9DC8(t * 3.1415927f)) * 0.5f);
}
