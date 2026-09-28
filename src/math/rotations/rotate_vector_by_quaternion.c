/* Ported from rac1-decomp, the PAL decompilation (src/game/mobyutil.c, func_00215650). */
#include "qcopy.h"
extern void FUN_001f9a68(void *, void *, float);
extern void FUN_001fa3c0(void *, void *, void *);
/* Conjugate-style sandwich: arg0 = arg2 * (arg1 with w = 0) * a, where
   a is arg2 negated with its w kept (FUN_001fa3c0 is the quaternion
   multiply). The 16-byte copy of arg1 is qcopy's lq/sq. */
void rotate_vector_by_quaternion(void *arg0, void *arg1, void *arg2) __asm__("FUN_00214800");

void rotate_vector_by_quaternion(void *arg0, void *arg1, void *arg2) {
    float a[4];
    float b[4];
    float c[4];

    FUN_001f9a68(a, arg2, -1.0f);
    a[3] = ((float *)arg2)[3];
    qcopy(b, arg1);
    b[3] = 0.0f;
    FUN_001fa3c0(c, arg2, b);
    FUN_001fa3c0(arg0, c, a);
}

extern __typeof__(rotate_vector_by_quaternion) func_00214800 __attribute__((alias("FUN_00214800")));
