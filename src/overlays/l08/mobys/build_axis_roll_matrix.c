/* Level 08 math helper: builds a matrix that rolls around a direction. */
#include "types.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/math/asm_helpers.h"

/* Builds a matrix that rolls by roll around the direction dir and writes it to out. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/mobyutil_0026B0E8.c: func_L08_0026B0E8), where it is exact; names translated to the US level program. */

extern void copy_matrix3x4(void *, void *);

void build_axis_roll_matrix(void *out,
                            void *roll_axis_direction,
                            float roll) __asm__("FUN_L08_0026a0f8");

void build_axis_roll_matrix(void *roll_matrix_destination, void *roll_axis_direction, float roll) {
    float M[4][4];
    float Rz[4][4];
    float Ry[4][4];
    float Rx[4][4];
    float rotation_angles[4];
    float working_direction[4];
    float alignment_angle;
    qcopy(working_direction, roll_axis_direction);
    set_identity_matrix(M);
    alignment_angle = -fast_arc_tan(working_direction[0], working_direction[1]);
    rotation_angles[0] = 0.0f;
    rotation_angles[1] = 0.0f;
    rotation_angles[2] = alignment_angle;
    euler_rotation_matrix(Rz, rotation_angles);
    transform_vector(working_direction, working_direction, Rz);
    alignment_angle = -fast_arc_tan(working_direction[2], working_direction[0]);
    rotation_angles[0] = 0.0f;
    rotation_angles[2] = 0.0f;
    rotation_angles[1] = alignment_angle;
    euler_to_matrix_ee(Ry, rotation_angles);
    transform_vector(working_direction, working_direction, Ry);
    rotation_angles[2] = roll;
    rotation_angles[0] = 0.0f;
    rotation_angles[1] = 0.0f;
    euler_to_matrix_ee(Rx, rotation_angles);
    multiply_matrix(M, Rz, M);
    multiply_matrix(M, Ry, M);
    multiply_matrix(M, Rx, M);
    transpose_rotation_matrix(Ry, Ry);
    multiply_matrix(M, Ry, M);
    transpose_rotation_matrix(Rz, Rz);
    multiply_matrix(M, Rz, M);
    qcopy(working_direction, roll_axis_direction);
    transform_vector(working_direction, working_direction, M);
    copy_matrix3x4(roll_matrix_destination, M);
}
