#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

typedef union { u128 q; f32 f[4]; } Vec4;

struct Tab {
    s32 count;
    s32 pad[3];
    u128 q[1];
};

extern void FUN_001f9a28(void *, void *, void *);
extern void FUN_001f9a40(void *, void *, void *, f32);
extern f32 FUN_001f9b20(void *);
extern f32 FUN_001f9e90(f32, f32);
extern f32 func_001FA580(f32, f32);
extern f32 func_001FA5C8(f32, f32);
extern s32 func_001FA6D0(f32);

void FUN_00214e58(struct Tab *tab, s32 flag, void *dst, f32 *out, s32 flags, f32 t) {
    Vec4 q0;
    Vec4 q1;
    Vec4 q2;
    Vec4 v;
    s32 i;
    s32 j;
    s32 k;
    s32 clamp;
    f32 f;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    f32 e;
    f32 z;
    f32 m;

    clamp = 0;
    i = func_001FA6D0(t);
    if (flag == 0) {
        s32 n = tab->count - 2;
        if (!(i < n)) {
            i = n;
            clamp = 1;
        }
    }
    f = t - (f32)i;
    if (clamp) {
        if (1.0f < f) {
            f = 1.0f;
        }
    }
    k = i + 2;
    j = i + 1;
    if (!(k < tab->count)) {
        j = j % tab->count;
        k = k % tab->count;
    }
    z = 0.0f;
    qcopy(&q0.q, &tab->q[i]);
    qcopy(&q1.q, &tab->q[j]);
    FUN_001f9a40(dst, &q0, &q1, f);
    if (!(flags & 1)) {
        FUN_001f9a28(&v, &q1, &q0);
        b = FUN_001f9e90(v.f[0], v.f[1]);
        a = FUN_001f9e90(FUN_001f9b20(&v), v.f[2]);
        e = q0.f[3];
        m = a;
        if (clamp) {
            d = b;
            e = z;
        } else {
            qcopy(&q2.q, &tab->q[k]);
            FUN_001f9a28(&v, &q2, &q1);
            d = FUN_001f9e90(v.f[0], v.f[1]);
            m = FUN_001f9e90(FUN_001f9b20(&v), v.f[2]);
            z = q1.f[3];
        }
        *(s32 *)out = 0;
        out[1] = -func_001FA580(func_001FA5C8(m, a) * f, a);
        out[2] = func_001FA580(func_001FA5C8(d, b) * f, b);
        out[3] = -func_001FA580(func_001FA5C8(z, e) * f, e);
    }
}

extern __typeof__(FUN_00214e58) func_00214E58 __attribute__((alias("FUN_00214e58")));
