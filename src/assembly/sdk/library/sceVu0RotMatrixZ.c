#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sceVu0RotMatrixZ/FUN_001252b8.s",
            FUN_001252b8);
#else
/* The polynomial coefficients are the vector loaded by the resident helper. */
static const float *const sine_coefficients = (const float *)0x00132e00;

void sceVu0RotMatrixZ(float output[4][4], const float input[4][4],
                      float radians) __asm__("FUN_001252b8");
void sceVu0RotMatrixZ(float output[4][4], const float input[4][4], float radians) {
    float angle;
    float cosine;
    float sine;
    float square;
    float term;
    float z;
    float w;
    int negative;
    int i;

    negative = radians < 0.0f;
    if (negative) {
        angle = 1.5707963705062866f + radians;
    } else {
        angle = 1.5707963705062866f - radians;
    }

    square = angle * angle;
    term = sine_coefficients[3] * angle;
    term *= square;
    cosine = angle + term;
    term = sine_coefficients[2] * angle;
    term *= square;
    term *= square;
    cosine += term;
    term = sine_coefficients[1] * angle;
    term *= square;
    term *= square;
    term *= square;
    cosine += term;
    term = sine_coefficients[0] * angle;
    term *= square;
    term *= square;
    term *= square;
    term *= square;
    cosine += term;
    sine = __builtin_sqrtf(1.0f - cosine * cosine);
    if (negative) {
        sine = -sine;
    }

    for (i = 0; i < 4; i++) {
        float x = input[i][0];
        float y = input[i][1];
        z = input[i][2];
        w = input[i][3];
        output[i][0] = cosine * x - sine * y;
        output[i][1] = sine * x + cosine * y;
        output[i][2] = z;
        output[i][3] = w;
    }
}
#endif /* NON_MATCHING */
