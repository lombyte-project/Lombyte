#include "rnc/math/asm_helpers.h"

typedef struct {
    int next[3];
} QuatNext;

extern QuatNext matrix_to_quaternion_next_axis_table __asm__("D_0015FFD8");
/* Rotation matrix (rows of four floats) to quaternion (x, y, z, w), by
   Shoemake's method: from the trace when it is positive, else from the
   largest diagonal element, working on a packed 3x3 copy. */
void matrix_to_quaternion(void *output_quaternion, void *rotation_matrix) __asm__("FUN_00214260");

void matrix_to_quaternion(void *output_quaternion, void *rotation_matrix) {
    float *quaternion_components = output_quaternion;
    float(*m)[4] = rotation_matrix;
    QuatNext next_axis_indices = matrix_to_quaternion_next_axis_table;
    float    packed_rotation_matrix[3][3];
    float    trace;
    float    quaternion_scale;
    int      i, j, k;

    trace = m[0][0] + m[1][1] + m[2][2];
    if (trace > 0.0f) {
        quaternion_scale = square_root(trace + 1.0f);
        quaternion_components[3] = quaternion_scale * 0.5f;
        quaternion_scale = 0.5f / quaternion_scale;
        quaternion_components[0] = (m[2][1] - m[1][2]) * quaternion_scale;
        quaternion_components[1] = (m[0][2] - m[2][0]) * quaternion_scale;
        quaternion_components[2] = (m[1][0] - m[0][1]) * quaternion_scale;
    } else {
        packed_rotation_matrix[0][0] = m[0][0];
        packed_rotation_matrix[0][1] = m[0][1];
        packed_rotation_matrix[0][2] = m[0][2];
        packed_rotation_matrix[1][0] = m[1][0];
        packed_rotation_matrix[1][1] = m[1][1];
        packed_rotation_matrix[1][2] = m[1][2];
        packed_rotation_matrix[2][0] = m[2][0];
        packed_rotation_matrix[2][1] = m[2][1];
        packed_rotation_matrix[2][2] = m[2][2];
        i = 0;
        if (packed_rotation_matrix[1][1] > packed_rotation_matrix[0][0]) {
            i = 1;
        }
        if (packed_rotation_matrix[2][2] > packed_rotation_matrix[i][i]) {
            i = 2;
        }
        j = next_axis_indices.next[i];
        k = next_axis_indices.next[j];
        quaternion_scale =
            square_root(packed_rotation_matrix[i][i] -
                        (packed_rotation_matrix[j][j] + packed_rotation_matrix[k][k]) + 1.0f);
        quaternion_components[i] = quaternion_scale * 0.5f;
        if (quaternion_scale != 0.0f) {
            quaternion_scale = 0.5f / quaternion_scale;
        }
        quaternion_components[3] =
            (packed_rotation_matrix[k][j] - packed_rotation_matrix[j][k]) * quaternion_scale;
        quaternion_components[j] =
            (packed_rotation_matrix[j][i] + packed_rotation_matrix[i][j]) * quaternion_scale;
        quaternion_components[k] =
            (packed_rotation_matrix[k][i] + packed_rotation_matrix[i][k]) * quaternion_scale;
    }
}

extern __typeof__(matrix_to_quaternion) func_00214260 __attribute__((alias("FUN_00214260")));
