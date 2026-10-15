/* Ported from rac1-decomp (src/game/mobyutil.c, func_00215650). */
#include "qcopy.h"
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void multiply_quaternions(void *, void *, void *) __asm__("FUN_001fa3c0");
/* Conjugate-style sandwich: out = quat * (vec with w = 0) * quat_conj, where
   quat_conj is quat negated with its w kept (multiply_quaternions is the quaternion
   multiply). The 16-byte copy of vec is qcopy's lq/sq. */
void rotate_vector_by_quaternion(void *out, void *vec, void *quat) __asm__("FUN_00214800");

void rotate_vector_by_quaternion(void *out, void *vec, void *quat) {
    float quat_conj[4];
    float vec_w0[4];
    float rotated_partial[4];

    fast_vec_scale(quat_conj, quat, -1.0f);
    quat_conj[3] = ((float *)quat)[3];
    qcopy(vec_w0, vec);
    vec_w0[3] = 0.0f;
    multiply_quaternions(rotated_partial, quat, vec_w0);
    multiply_quaternions(out, rotated_partial, quat_conj);
}

extern __typeof__(rotate_vector_by_quaternion) func_00214800 __attribute__((alias("FUN_00214800")));
