#include "types.h"
#include "asm.h"
#include "rnc/ui/vendor/vendor_capture.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/vendor/render_vendor_capture_pass_sequence/FUN_00239780.s", FUN_00239780);
#else
/* Near miss, 4 rows: sched1 issues the a0 argument copy before the a2 one at the first call (an LUID tie); every other row matches. The loop locals sit at function scope with the ids pointer set before the loop, which gives retail's spill-slot order. */
/* Ported from rac1-decomp (src/game/vendor.c: func_0023AA38). */
#include "types.h"
#include "sda.h"
#include "qcopy.h"

typedef int u128_3AA38 __attribute__((mode(TI)));
extern float D_001E6218[][4];
extern int D_00161298 MACRO_ADDR;
extern short D_001612A0;
extern short D_0016129C;
extern short D_00161294;
extern int D_0015ED80 MACRO_ADDR;
extern long D_0015EED0 MACRO_ADDR;
extern void func_0020CD48(char *, int, int *, char *);
extern void func_001F9A28(void *, void *, void *);
extern void func_001F9A10(void *, void *, void *);
extern float func_001F9AF0(void *);
extern void func_001F9BF8(void *, void *, float);
extern float func_001FA6C0(int);
extern int func_001FA6D0(float);
extern void func_00237C80(void *, void *, float *, float *, float *, float *);
extern void func_00239690(int, int, float);
extern void func_001FB740(int, int);
extern void func_00233980(int, long);
extern void func_00238520(char *);
extern void func_002389E0(char *);
extern void func_00238630(char *);
extern void func_00238F08(char *, int, int);
extern void func_00239160(char *);
extern void func_002386E8(char *);
extern void func_00239328(int, float, float);
extern void func_00239750(void);
extern void func_00233B68(void);
extern void func_001F55D8(float, float, float, float, int, int, int, int, long, long);

/* Renders the six vendor monitor screens: projects each screen's joint box, renders its view off-screen and draws it back. */
void FUN_00239780(char *m) {
    u128_3AA38 d1[1];
    u128_3AA38 d2[1];
    u128_3AA38 base[1];
    u128_3AA38 out[3];
    int ids[3];
    u128_3AA38 tmp[1];
    u128_3AA38 c[4];
    u128_3AA38 pad[5];
    u128_3AA38 a[1];
    u128_3AA38 b[1];
    float sx, sy, sw, sh;
    float x, y, w, h;
    float len;
    float f;
    float g;
    int i;
    long tex;
    int *idp = ids;

    for (i = 0; i < 6; i++) {
        ids[0] = i * 4;
        ids[1] = i * 4 + 1;
        ids[2] = i * 4 + 2;
        func_0020CD48(m, 3, idp, (char *)out);
        qcopy(base, out);
        func_001F9A28(d1, &out[1], out);
        func_001F9A28(d2, &out[2], out);
        len = func_001F9AF0(d1);
        func_001F9BF8(tmp, d1, D_001E6218[i][0]);
        func_001F9A10(base, base, tmp);
        func_001F9BF8(d1, d1, len - 2.0f * D_001E6218[i][2]);
        len = func_001F9AF0(d2);
        func_001F9BF8(tmp, d2, D_001E6218[i][1]);
        func_001F9A10(base, base, tmp);
        func_001F9BF8(d2, d2, len - 2.0f * D_001E6218[i][3]);
        func_001F9A10(tmp, base, d1);
        func_001F9A10(tmp, tmp, d2);
        func_00237C80(base, tmp, &sx, &sy, &sw, &sh);
        if (D_00161298 != 0 || *(int *)&D_001612A0 != 0) {
            f = 1.0f;
            if (*(int *)&D_001612A0 != 0) {
                f = func_001FA6C0(8 - *(int *)&D_0016129C) * 0.125f;
            }
            if (D_00161298 != 0) {
                f = func_001FA6C0(*(int *)&D_00161294) * 0.125f;
            }
            len = func_001F9AF0(d1);
            g = 1.0f - f;
            func_001F9BF8(tmp, d1, len * g * 0.5f);
            func_001F9BF8(d1, d1, len * f);
            func_001F9A10(base, base, tmp);
            len = func_001F9AF0(d2);
            func_001F9BF8(tmp, d2, len * g * 0.5f);
            func_001F9BF8(d2, d2, len * f);
            func_001F9A10(base, base, tmp);
        }
        qcopy(&c[0], base);
        func_001F9A10(&c[1], base, d1);
        func_001F9A10(&c[2], base, d2);
        func_001F9A10(&c[3], &c[2], d1);
        a[0] = c[0];
        b[0] = c[3];
        func_00237C80(a, b, &x, &y, &w, &h);
        func_00239690(9, 7, 1.0f);
        func_001FB740(0x200, 0x200);
        func_00233980(0x42, 0x8000000064L);
        switch (i) {
        case 0:
            func_00238520(m);
            break;
        case 1:
            func_002389E0(m);
            break;
        case 2:
            func_00238630(m);
            break;
        case 3:
            func_00238F08(m, (int)x, (int)y);
            break;
        case 4:
            func_00239160(m);
            break;
        case 5:
            func_002386E8(m);
            break;
        }
        func_00239328(i, x, y);
        func_00239750();
        func_001FB740(0x200, 0x200);
        func_00233980(0x42, 0x8000000064L);
        func_00233980(8, 5);
        func_00233B68();
        tex = D_0015EED0;
        if (D_0015ED80 != 0) {
            func_001F55D8(w, h, x, y, 0, 0, (int)(sx - 1.0f),
                          func_001FA6D0(sy * 0.92857146f - 1.0f),
                          0x80808080, tex);
        } else {
            func_001F55D8(w, h, x, y, 0, 0, (int)(sx - 1.0f), (int)(sy - 1.0f),
                          0x80808080, tex);
        }
        func_00233980(8, 0);
    }
}
#endif /* NON_MATCHING */
