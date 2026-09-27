/* Ported from rac1-decomp, the PAL decompilation (src/game/mobyutil.c, func_002156E0). */
#include "qcopy.h"
extern float func_001F99C0(float arg0);
extern void FUN_001f9bf8(void *, void *, float);
extern void FUN_00214530(void *arg0, void *axis, float angle);
extern void func_00214800(void *arg0, void *arg1, void *arg2);
/* dst = vec rotated `angle` around axis. A tiny angle skips the rotation
   (dst = vec); otherwise the axis is normalised to unit length and turned
   into an axis-angle quaternion in a scratch buffer, which then rotates
   vec into dst. */
void build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");

void build_look_at_matrix(void *dst, void *vec, void *axis, float angle) {
    float q[4];

    if (func_001F99C0(angle) < 0.00001f) {
        qcopy(dst, vec);
        return;
    }
    FUN_001f9bf8(q, axis, 1.0f);
    FUN_00214530(q, q, angle);
    func_00214800(dst, vec, q);
}

extern __typeof__(build_look_at_matrix) func_00214890 __attribute__((alias("FUN_00214890")));
