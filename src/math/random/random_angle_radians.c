#include "types.h"
#include "rnc/math/random.h"

extern s32 rand();

f32 random_angle_radians(void) __asm__("FUN_00213308");

f32 random_angle_radians(void) {
    return (f32)(((rand() >> 0x10) & 0xFFF) - 0x800) * 3.1415927f * 0.00048828125f;
}

extern __typeof__(random_angle_radians) func_00213308 __attribute__((alias("FUN_00213308")));
