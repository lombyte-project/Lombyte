/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "asm.h"

float random_float_between(float *, float, float) __asm__("FUN_002132a8");
float random_angle_radians(void) __asm__("FUN_00213308");
float fast_cos(float) __asm__("FUN_001f9dc8");
float fast_sin(float) __asm__("FUN_001f9de0");
void random_vector(float *out, float lo, float hi) __asm__("FUN_L00_00257d78");
void random_vector(float *out, float lo, float hi) {
    float r = random_float_between(out, lo, hi);
    float a = random_angle_radians();
    float b = random_angle_radians();
    out[0] = fast_cos(a) * fast_sin(b) * r;
    out[1] = fast_sin(a) * fast_sin(b) * r;
    out[2] = fast_cos(b) * r;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_00258E58), where it is exact; names translated to the US level program. */

/* Cubic interpolation through four points a..d at t. */
float cubic_hermite_scalar(float a, float b, float c, float d, float t) __asm__("FUN_L00_00257e20");
float cubic_hermite_scalar(float a, float b, float c, float d, float t) {
    float p = (d - c) - (a - b);
    float q = (a - b) - p;
    float t_squared = t * t;
    float t_cubed = t_squared * t;
    return p * t_cubed + q * t_squared + (c - a) * t + b;
}
