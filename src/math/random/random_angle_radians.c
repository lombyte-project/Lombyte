/* FUN_00213308 exact recovery: 12-bit angle extraction from rand() with the 9 E notation conversion; the patched profile (lq/sq saves + classic mult) reproduces all 20 instructions. Caller-facing func_00213308 alias required for the oracle link. */

#include "types.h"
extern s32 rand();
f32 random_angle_radians(s32 arg0) __asm__("FUN_00213308");

f32 random_angle_radians(s32 arg0) {
    return (f32)(((rand() >> 0x10) & 0xFFF) - 0x800) * 3.1415927f * 0.00048828125f;
}
extern __typeof__(random_angle_radians) func_00213308 __attribute__((alias("FUN_00213308")));
