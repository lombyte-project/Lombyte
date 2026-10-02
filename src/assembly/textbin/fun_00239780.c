#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00239780/FUN_00239780.s", FUN_00239780);
#else
#include "types.h"
#include "eetypes.h"

typedef union { u128 q; f32 f[4]; } Vec4;
struct Inset { f32 a; f32 b; f32 c; f32 d; };

extern struct Inset D_001E6218[];
struct Flag { s32 v; };
extern struct Flag D_00161298;
extern s32 D_001612A0;
extern s32 D_0016129C;
extern s32 D_00161294;
extern s32 D_0015ED80[];
extern s64 D_0015EED0;

extern void func_0020CD48();
extern void func_001F9A28(void *out, void *a, void *b);
extern void func_001F9A10(void *out, void *a, void *b);
extern f32 func_001F9AF0(void *v);
extern void func_001F9BF8(void *out, void *v, f32 s);
extern f32 func_001FA6C0(s32);
extern s32 func_001FA6D0(f32);
extern void func_00237C80();
extern void func_00239690(s32, s32, f32);
extern void func_001FB740(s32, s32);
extern void func_00233980(s32, s64);
extern void func_00233B68(void);
extern void func_00238520();
extern void func_002389E0();
extern void func_00238630();
extern void func_00238F08();
extern void func_00239160();
extern void func_002386E8();
extern void func_00239328(s32, f32, f32);
extern void func_00239750(void);
extern void func_001F55D8(f32, f32, f32, f32, s32, s32, s32, s32, s64, s64);

void FUN_00239780(s32 arg0) {
    Vec4 a;
    Vec4 b;
    Vec4 org;
    Vec4 pts[3];
    s32 idx[3];
    Vec4 tmp;
    Vec4 q[9];
    Vec4 p0;
    Vec4 p1;
    f32 x0, y0, x1, y1;
    f32 x2, y2, x3, y3;
    s32 i;
    f32 len;
    f32 f;
    s64 tex;
    Vec4 *pp;
    s32 *pi;
    Vec4 *po;
    Vec4 *pq;

    pp = pts;
    pi = idx;
    po = &org;
    for (i = 0; i < 6; i++) {
        do { idx[0] = i * 4; idx[1] = i * 4 + 1; idx[2] = i * 4 + 2; func_0020CD48(arg0, 3, pi, pp); po->q = pp->q; } while (0);
        func_001F9A28(&a, &pts[1], pp);
        func_001F9A28(&b, &pts[2], pp);
        len = func_001F9AF0(&a);
        func_001F9BF8(&tmp, &a, D_001E6218[i].a);
        func_001F9A10(po, po, &tmp);
        func_001F9BF8(&a, &a, len - 2.0f * D_001E6218[i].c);
        len = func_001F9AF0(&b);
        func_001F9BF8(&tmp, &b, D_001E6218[i].b);
        func_001F9A10(po, po, &tmp);
        func_001F9BF8(&b, &b, len - 2.0f * D_001E6218[i].d);
        func_001F9A10(&tmp, po, &a);
        func_001F9A10(&tmp, &tmp, &b);
        func_00237C80(po, &tmp, &x0, &y0, &x1, &y1);
        pq = q;
        if (D_00161298.v != 0 || D_001612A0 != 0) {
            f = 1.0f;
            if (D_001612A0 != 0) {
                f = func_001FA6C0(8 - D_0016129C) * 0.125f;
            }
            if (D_00161298.v != 0) {
                f = func_001FA6C0(D_00161294) * 0.125f;
            }
            len = func_001F9AF0(&a);
            func_001F9BF8(&tmp, &a, len * (1.0f - f) * 0.5f);
            func_001F9BF8(&a, &a, len * f);
            func_001F9A10(po, po, &tmp);
            len = func_001F9AF0(&b);
            func_001F9BF8(&tmp, &b, len * (1.0f - f) * 0.5f);
            func_001F9BF8(&b, &b, len * f);
            func_001F9A10(po, po, &tmp);
        }
        pq->q = po->q;
        func_001F9A10(&q[1], po, &a);
        func_001F9A10(&q[2], po, &b);
        func_001F9A10(&q[3], &q[2], &a);
        p0.q = q[0].q;
        p1.q = q[3].q;
        func_00237C80(&p0, &p1, &x2, &y2, &x3, &y3);
        func_00239690(9, 7, 1.0f);
        func_001FB740(0x200, 0x200);
        func_00233980(0x42, 0x8000000064LL);
        switch (i) {
        case 0:
            func_00238520(arg0);
            break;
        case 1:
            func_002389E0(arg0);
            break;
        case 2:
            func_00238630(arg0);
            break;
        case 3:
            func_00238F08(arg0, (s32)x2, (s32)y2);
            break;
        case 4:
            func_00239160(arg0);
            break;
        case 5:
            func_002386E8(arg0);
            break;
        }
        func_00239328(i, x2, y2);
        func_00239750();
        func_001FB740(0x200, 0x200);
        func_00233980(0x42, 0x8000000064LL);
        func_00233980(8, 5);
        func_00233B68();
        tex = D_0015EED0;
        if (D_0015ED80[0] != 0) {
            f32 fa, fb, fc, fd;
            fa = x3; fb = y3; fc = x2; fd = y2;
            func_001F55D8(fa, fb, fc, fd, 0, 0, (s32)(x0 - 1.0f), func_001FA6D0(y0 * 0.92857146f - 1.0f), 0x80808080, tex);
        } else {
            func_001F55D8(x3, y3, x2, y2, 0, 0, (s32)(x0 - 1.0f), (s32)(y0 - 1.0f), 0x80808080, tex);
        }
        func_00233980(8, 0);
    }
}
#endif /* NON_MATCHING */
