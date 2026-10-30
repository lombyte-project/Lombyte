#include "types.h"
#include "rnc/math/random.h"
#include "rnc/math/spherical_offset.h"

void random_spherical_offset(void *out, f32 a, f32 b) __asm__("FUN_00213358");

void random_spherical_offset(void *offset_vector, f32 min_radius, f32 max_radius) {
    f32 x = random_angle_radians();
    f32 y = random_angle_radians();

    build_spherical_offset(offset_vector, random_float_between(min_radius, max_radius), x, y);
}

extern __typeof__(random_spherical_offset) func_00213358 __attribute__((alias("FUN_00213358")));
