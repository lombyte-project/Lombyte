/* Vendor: the vendor spring. */
#include "types.h"
#include "asm.h"
#include "rnc/math/fast_math.h"

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_0025C918), where it is exact; names translated to the US level program. */

extern void advance_damped_scalar_velocity(
    float *ptr, float rate, float b, float c, float limit) __asm__("FUN_L00_0025b798");

float vendor_spring(float *p, float *v, float t, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
float vendor_spring(float *value,
                    float *velocity,
                    float  target_value,
                    float  error_gain,
                    float  damping_coefficient,

                    float eps) {
    float next_value, remaining_error;
    advance_damped_scalar_velocity(
        velocity, target_value - *value, error_gain, damping_coefficient, eps);
    next_value = *value + *velocity;
    remaining_error = target_value - next_value;
    *value = next_value;
    if (absolute_float(remaining_error) < eps * 0.01f) {
        *value = target_value;
        *(int *)velocity = 0;
        return *velocity;
    }
    return remaining_error;
}
