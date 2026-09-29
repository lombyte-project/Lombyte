/* Ported from rac1-decomp, the PAL decompilation (src/game/mobyutil.c,
   func_002153E8). */
#include "eetypes.h"

typedef union {
    float m[4][4];
    u128 q[4];
} Mtx44;

extern void func_001F99F8(float *);
extern float FUN_001f9e90(float, float);
extern void FUN_001fa050(float *, float *);
extern void FUN_001fa070(float *, float *);
extern void FUN_001fa378(Mtx44 *, float *, Mtx44 *);

/* Matrix to Euler angles: on a copy of src with the translation cleared,
   read the Z angle from row 0, undo it (FUN_001fa050 builds the Z
   rotation, FUN_001fa378 multiplies), read and undo Y the same way,
   then read X from row 1; out = (x, y, z). The last undo vector is
   filled but never used. The four TImode row copies give retail's
   interleaved lq/sq (a whole-struct copy becomes ld/sd here). */
void FUN_00214598(Mtx44 *src, float *out) {
    float rot[16];
    Mtx44 m;
    float v[4];
    float x;
    float y;
    float z;

    m.q[0] = src->q[0];
    m.q[1] = src->q[1];
    m.q[2] = src->q[2];
    m.q[3] = src->q[3];
    func_001F99F8(m.m[3]);
    m.m[3][3] = 1.0f;
    z = FUN_001f9e90(m.m[0][0], m.m[0][1]);
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = -z;
    FUN_001fa050(rot, v);
    FUN_001fa378(&m, rot, &m);
    y = FUN_001f9e90(m.m[0][0], -m.m[0][2]);
    v[0] = 0.0f;
    v[2] = 0.0f;
    v[1] = -y;
    FUN_001fa070(rot, v);
    FUN_001fa378(&m, rot, &m);
    x = FUN_001f9e90(m.m[1][1], m.m[1][2]);
    v[1] = 0.0f;
    v[0] = -x;
    v[2] = 0.0f;
    out[2] = z;
    out[1] = y;
    out[0] = x;
}

extern __typeof__(FUN_00214598) func_00214598 __attribute__((alias("FUN_00214598")));
