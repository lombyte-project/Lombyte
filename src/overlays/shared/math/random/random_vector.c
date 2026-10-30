/* Random: a random vector, and a cubic Hermite scalar. */
#include "types.h"
#include "asm.h"
#include "rnc/math/fast_math.h"

float random_float_between(float *, float, float) __asm__("FUN_002132a8");
float random_angle_radians(void) __asm__("FUN_00213308");
void  random_vector(float *out, float lo, float hi) __asm__("FUN_L00_00257d78");

void random_vector(float *generated_vector, float lo, float hi) {
    float radial_scale = random_float_between(generated_vector, lo, hi);
    float azimuth_angle = random_angle_radians();
    float polar_angle = random_angle_radians();
    generated_vector[0] = fast_cos(azimuth_angle) * fast_sin(polar_angle) * radial_scale;
    generated_vector[1] = fast_sin(azimuth_angle) * fast_sin(polar_angle) * radial_scale;
    generated_vector[2] = fast_cos(polar_angle) * radial_scale;
}

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_00258E58), where it is exact; names translated to the US level program. */

/* Cubic interpolation through four points a..d at t. */
float cubic_hermite_scalar(float a, float b, float c, float d, float t) __asm__("FUN_L00_00257e20");
float cubic_hermite_scalar(float start_slope_reference_value,
                           float start_value,
                           float end_value,
                           float end_slope_reference_value,

                           float interpolation_fraction) {
    float cubic_coefficient =
        (end_slope_reference_value - end_value) - (start_slope_reference_value - start_value);
    float quadratic_coefficient = (start_slope_reference_value - start_value) - cubic_coefficient;
    float t_squared = interpolation_fraction * interpolation_fraction;
    float t_cubed = t_squared * interpolation_fraction;
    return cubic_coefficient * t_cubed + quadratic_coefficient * t_squared +
           (end_value - start_slope_reference_value) * interpolation_fraction + start_value;
}
