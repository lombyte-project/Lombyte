#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

typedef union { u128 q; f32 f[4]; } Vec4;

struct Sub {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s16 h10;
    s16 h12;
    s16 h14;
    s16 h16;
    s16 h18;
};

struct Obj {
    u8 pad_0;
    u8 b1;
    u8 b2;
    u8 b3;
    s32 w4;
    u8 b8;
    u8 b9;
    s16 hA;
    f32 fC;
    u128 q10;
    struct Sub sub;
};

extern u8 *D_001CDF80[];
extern u8 *D_001CDFDC[];
extern struct Obj *FUN_00217a30(s32);
extern s32 func_001FA6D0(f32);
extern f32 func_002132A8(f32, f32);
extern void func_00214A98(void *, void *);
extern void func_00214BC0(void *, void *);

struct Obj *FUN_00218888(u128 *arg0, f32 *arg1, f32 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    struct Obj *o;
    struct Sub *s;
    f32 t0[4];
    f32 t1[4];

    o = FUN_00217a30(2);
    if (o != 0) {
        qcopy(&o->q10, arg0);
        o->w4 = arg3;
        o->b9 = func_001FA6D0(4.0f) + 0x40;
        o->fC = 0.0f;
        o->b1 = 0;
        o->b3 = 0x44;
        o->b8 = func_001FA6D0(func_002132A8(o->fC, 255.0f));
        if (arg8 == -1) {
            o->b2 = *D_001CDFDC[0];
        } else {
            o->b2 = *D_001CDF80[(u16)arg8];
            if (arg8 & 0x10000) {
                o->b3 = 0x48;
            }
        }
        o->hA = arg5;
        s = &o->sub;
        s->h16 = func_001FA6D0(arg1[3] * 210000.0f / 1000.0f);
        s->h18 = func_001FA6D0(arg2[3] * 210000.0f / 1000.0f);
        func_00214A98(arg1, &s->a);
        func_00214A98(arg2, &s->b);
        func_00214BC0(t0, &s->a);
        func_00214BC0(t1, &s->b);
        s->c = arg3;
        s->d = arg4;
        s->h10 = arg5;
        s->h12 = arg6;
        s->h14 = arg7;
    }
    return o;
}

extern __typeof__(FUN_00218888) func_00218888 __attribute__((alias("FUN_00218888")));
