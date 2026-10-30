#include "types.h"
#include "rnc/math/random.h"

extern s32 rand();

f32 random_float_between(f32 range_start, f32 range_end) __asm__("FUN_002132a8");

f32 random_float_between(f32 range_start, f32 range_end) {
    return range_start +
           ((f32)((rand() >> 0x10) & 0x7FFF) * (range_end - range_start) * 0.000030517578f);
}

extern __typeof__(random_float_between) func_002132A8 __attribute__((alias("FUN_002132a8")));
