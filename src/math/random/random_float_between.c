#include "types.h"
extern s32 rand();
f32 random_float_between(s32 arg0, f32 fparg0, f32 fparg1) __asm__("FUN_002132a8");

f32 random_float_between(s32 unused_parameter, f32 range_start, f32 range_end) {
    return range_start + ((f32)((rand() >> 0x10) & 0x7FFF) * (range_end - range_start) * 0.000030517578f);
}

extern f32 func_002132A8(s32, f32, f32) __attribute__((alias("FUN_002132a8")));
