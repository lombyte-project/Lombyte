extern float square_root(float) __asm__("FUN_001f9988");
typedef struct {
    int next[3];
} QuatNext;
extern QuatNext D_0015FFD8;
/* Rotation matrix (rows of four floats) to quaternion (x, y, z, w), by
   Shoemake's method: from the trace when it is positive, else from the
   largest diagonal element, working on a packed 3x3 copy. */
void matrix_to_quaternion(void *arg0, void *arg1) __asm__("FUN_00214260");

void matrix_to_quaternion(void *output_quaternion, void *rotation_matrix) {
    float *q = output_quaternion;
    float(*m)[4] = rotation_matrix;
    QuatNext n = D_0015FFD8;
    float mat[3][3];
    float trace;
    float s;
    int i, j, k;

    trace = m[0][0] + m[1][1] + m[2][2];
    if (trace > 0.0f) {
        s = square_root(trace + 1.0f);
        q[3] = s * 0.5f;
        s = 0.5f / s;
        q[0] = (m[2][1] - m[1][2]) * s;
        q[1] = (m[0][2] - m[2][0]) * s;
        q[2] = (m[1][0] - m[0][1]) * s;
    } else {
        mat[0][0] = m[0][0];
        mat[0][1] = m[0][1];
        mat[0][2] = m[0][2];
        mat[1][0] = m[1][0];
        mat[1][1] = m[1][1];
        mat[1][2] = m[1][2];
        mat[2][0] = m[2][0];
        mat[2][1] = m[2][1];
        mat[2][2] = m[2][2];
        i = 0;
        if (mat[1][1] > mat[0][0]) {
            i = 1;
        }
        if (mat[2][2] > mat[i][i]) {
            i = 2;
        }
        j = n.next[i];
        k = n.next[j];
        s = square_root(mat[i][i] - (mat[j][j] + mat[k][k]) + 1.0f);
        q[i] = s * 0.5f;
        if (s != 0.0f) {
            s = 0.5f / s;
        }
        q[3] = (mat[k][j] - mat[j][k]) * s;
        q[j] = (mat[j][i] + mat[i][j]) * s;
        q[k] = (mat[k][i] + mat[i][k]) * s;
    }
}

extern __typeof__(matrix_to_quaternion) func_00214260 __attribute__((alias("FUN_00214260")));
