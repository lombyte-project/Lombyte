/* Random: an int between two bounds, and a symmetric random float. */
#include "types.h"
#include "asm.h"

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_00258BC8), where it is exact; names translated to the US level program. */

extern s32 rand();

/* Random integer in lo..hi inclusive from rand(). */
int random_int_between(int lo, int hi) __asm__("FUN_L00_00257b90");

int random_int_between(int lo, int hi) {
    return ((rand() >> 16) & 0x7FFF) % (hi - lo + 1) + lo;
}

/* Random float in [lo, hi], randomly negated: uses a 12-bit fraction
   (bits 16-27 of the RNG word) for the magnitude and bit 16 for the
   sign. */
/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00258BC8.c: func_L00_00258C80), where it is exact; names translated to the US level program. */

/* Random float of magnitude lo..hi from rand(), negated on odd values. */
float randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");

float randf_sym(float lo, float hi) {
    int   shifted_random_bits = rand() >> 16;
    float range = hi - lo;
    float sampled_value = lo + (float)(shifted_random_bits & 0xFFF) * range * 0.000244140625f;

    if (shifted_random_bits & 1) {
        sampled_value = -sampled_value;
    }
    return sampled_value;
}
