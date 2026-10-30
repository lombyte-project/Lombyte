/* Ported from rac1-decomp (src/game/mobyutil.c,
   func_002153E8). */
#include "eetypes.h"

typedef union {
    float m[4][4];
    u128  q[4];
} Mtx44;

extern void  clear_u64_value(float *) __asm__("func_001F99F8");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void  euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void  euler_to_matrix_ee(float *, float *) __asm__("FUN_001fa070");
extern void  multiply_matrix(Mtx44 *, float *, Mtx44 *) __asm__("FUN_001fa378");

/* Matrix to Euler angles: on a copy of src with the translation cleared,
   read the Z angle from row 0, undo it (euler_rotation_matrix builds the Z
   rotation, multiply_matrix multiplies), read and undo Y the same way,
   then read X from row 1; out = (x, y, z). The last undo vector is
   filled but never used. The four TImode row copies give retail's
   interleaved lq/sq (a whole-struct copy becomes ld/sd here). */
void matrix_to_euler(Mtx44 *src, float *out) __asm__("FUN_00214598");

void matrix_to_euler(Mtx44 *input_matrix, float *out) {
    float cancellation_matrix[16];
    Mtx44 m;
    float cancellation_angles[4];
    float x;
    float y;
    float z;

    m.q[0] = input_matrix->q[0];
    m.q[1] = input_matrix->q[1];
    m.q[2] = input_matrix->q[2];
    m.q[3] = input_matrix->q[3];
    clear_u64_value(m.m[3]);
    m.m[3][3] = 1.0f;
    z = fast_arc_tan(m.m[0][0], m.m[0][1]);
    cancellation_angles[0] = 0.0f;
    cancellation_angles[1] = 0.0f;
    cancellation_angles[2] = -z;
    euler_rotation_matrix(cancellation_matrix, cancellation_angles);
    multiply_matrix(&m, cancellation_matrix, &m);
    y = fast_arc_tan(m.m[0][0], -m.m[0][2]);
    cancellation_angles[0] = 0.0f;
    cancellation_angles[2] = 0.0f;
    cancellation_angles[1] = -y;
    euler_to_matrix_ee(cancellation_matrix, cancellation_angles);
    multiply_matrix(&m, cancellation_matrix, &m);
    x = fast_arc_tan(m.m[1][1], m.m[1][2]);
    cancellation_angles[1] = 0.0f;
    cancellation_angles[0] = -x;
    cancellation_angles[2] = 0.0f;
    out[2] = z;
    out[1] = y;
    out[0] = x;
}

extern __typeof__(matrix_to_euler) func_00214598 __attribute__((alias("FUN_00214598")));
