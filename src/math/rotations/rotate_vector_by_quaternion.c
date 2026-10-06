/* Ported from rac1-decomp (src/game/mobyutil.c, func_00215650). */
#include "qcopy.h"
extern void FUN_001f9a68(void *, void *, float);
extern void FUN_001fa3c0(void *, void *, void *);
/* Conjugate-style sandwich: out = quat * (vec with w = 0) * quat_conj, where
   quat_conj is quat negated with its w kept (FUN_001fa3c0 is the quaternion
   multiply). The 16-byte copy of vec is qcopy's lq/sq. */
void rotate_vector_by_quaternion(void *out, void *vec, void *quat) __asm__("FUN_00214800");

void rotate_vector_by_quaternion(void *out, void *vec, void *quat) {
    float quat_conj[4];
    float vec_w0[4];
    float rotated_partial[4];

    FUN_001f9a68(quat_conj, quat, -1.0f);
    quat_conj[3] = ((float *)quat)[3];
    qcopy(vec_w0, vec);
    vec_w0[3] = 0.0f;
    FUN_001fa3c0(rotated_partial, quat, vec_w0);
    FUN_001fa3c0(out, rotated_partial, quat_conj);
}

extern __typeof__(rotate_vector_by_quaternion) func_00214800 __attribute__((alias("FUN_00214800")));
