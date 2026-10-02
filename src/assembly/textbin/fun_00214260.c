#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00214260/FUN_00214260.s", FUN_00214260);
#else
#include "types.h"

struct Next3 {
    s32 v[3];
};

extern struct Next3 D_0015FFD8;
extern f32 func_001F9988(f32);

void FUN_00214260(f32 *q, f32 m[3][4]) {
    struct Next3 nxt;
    f32 mat[3][3];
    f32 tr;
    f32 s;
    s32 i;
    s32 j;
    s32 k;

    nxt = D_0015FFD8;
    tr = m[0][0] + m[1][1] + m[2][2];
    if (tr > 0.0f) {
        s = func_001F9988(tr + 1.0f);
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
        j = nxt.v[i];
        k = nxt.v[j];
        s = func_001F9988(mat[i][i] - (mat[j][j] + mat[k][k]) + 1.0f);
        q[i] = s * 0.5f;
        if (s != 0.0f) {
            s = 0.5f / s;
        }
        q[3] = (mat[k][j] - mat[j][k]) * s;
        q[j] = (mat[j][i] + mat[i][j]) * s;
        q[k] = (mat[k][i] + mat[i][k]) * s;
    }
}
#endif /* NON_MATCHING */
