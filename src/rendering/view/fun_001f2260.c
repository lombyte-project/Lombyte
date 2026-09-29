#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

typedef union { u128 q; f32 f[4]; } Vec4;

struct G {
    f32 f[0x50];
};
extern struct G D_00186F40;
extern u8 D_00187290[];
extern s32 D_0018C32C[];
extern u8 D_0018CD00[];
extern u8 D_0018CF80[];

extern void sceVu0UnitMatrix(void *);
extern void SceVu0RotMatrixX(void *, void *, f32);
extern void SceVu0RotMatrixY(void *, void *, f32);
extern void sceVu0RotMatrixZ(void *, void *, f32);
extern void FUN_001fa378(void *, void *, void *);
extern void FUN_001f9a80(void *, void *, f32);
extern void FUN_001f9a68(void *, void *, f32);

void FUN_001f2260(void) {
    Vec4 m[4];
    struct G *g;
    u8 *x;
    f32 *xf;
    f32 a;
    f32 b;
    f32 c;

    if (D_0018C32C[0] == 0) {
        qcopy(&m[0].q, D_00187290);
        qcopy(&m[1].q, D_00187290 + 0x10);
        qcopy(&m[2].q, D_00187290 + 0x20);
    } else {
        sceVu0UnitMatrix(m);
        SceVu0RotMatrixX(m, m, D_00186F40.f[0x150 / 4]);
        SceVu0RotMatrixY(m, m, D_00186F40.f[0x154 / 4]);
        sceVu0RotMatrixZ(m, m, D_00186F40.f[0x158 / 4]);
    }
    g = &D_00186F40;
    x = D_0018CD00 + 0xC0;
    xf = (f32 *)D_0018CD00;
    g->f[0] = -m[1].f[0];
    g->f[0x3C / 4] = 1.0f;
    *(s32 *)&g->f[0x30 / 4] = 0;
    g->f[0x10 / 4] = -m[1].f[1];
    g->f[0x20 / 4] = -m[1].f[2];
    g->f[4 / 4] = -m[2].f[0];
    g->f[0x14 / 4] = -m[2].f[1];
    g->f[0x24 / 4] = -m[2].f[2];
    g->f[8 / 4] = m[0].f[0];
    g->f[0x18 / 4] = m[0].f[1];
    g->f[0x28 / 4] = m[0].f[2];
    *(s32 *)&g->f[0x34 / 4] = 0;
    *(s32 *)&g->f[0x38 / 4] = 0;
    *(s32 *)&g->f[0xC / 4] = 0;
    *(s32 *)&g->f[0x1C / 4] = 0;
    *(s32 *)&g->f[0x2C / 4] = 0;
    FUN_001fa378(&g->f[0x40 / 4], x, g);
    FUN_001fa378(&g->f[0x80 / 4], x + 0x40, g);
    a = xf[0x1A0 / 4];
    b = xf[0x1A4 / 4];
    c = xf[0x1A8 / 4];
    g->f[0x80 / 4] += g->f[0x8C / 4] * a;
    g->f[0x84 / 4] += g->f[0x8C / 4] * b;
    g->f[0x88 / 4] += g->f[0x8C / 4] * c;
    g->f[0x90 / 4] += g->f[0x9C / 4] * a;
    g->f[0x94 / 4] += g->f[0x9C / 4] * b;
    g->f[0x98 / 4] += g->f[0x9C / 4] * c;
    g->f[0xA0 / 4] += g->f[0xAC / 4] * a;
    g->f[0xA4 / 4] += g->f[0xAC / 4] * b;
    g->f[0xA8 / 4] += g->f[0xAC / 4] * c;
    g->f[0xB0 / 4] += g->f[0xBC / 4] * a;
    g->f[0xB4 / 4] += g->f[0xBC / 4] * b;
    g->f[0xB8 / 4] += g->f[0xBC / 4] * c;
    FUN_001fa378(&g->f[0xC0 / 4], x + 0x80, g);
    FUN_001f9a80(&g->f[0x100 / 4], x + 0x80, xf[0x1C0 / 4]);
    FUN_001f9a80(&g->f[0x110 / 4], x + 0x90, xf[0x1C0 / 4]);
    qcopy(&g->f[0x120 / 4], x + 0xA0);
    qcopy(&g->f[0x130 / 4], x + 0xB0);
    FUN_001fa378(&g->f[0x100 / 4], &g->f[0x100 / 4], g);
    qcopy(D_0018CF80, g);
    qcopy(D_0018CF80 + 0x10, &g->f[0x10 / 4]);
    qcopy(D_0018CF80 + 0x20, &g->f[0x20 / 4]);
    FUN_001f9a68(D_0018CF80 + 0x30, &g->f[0x140 / 4], 1024.0f);
    *(f32 *)(D_0018CF80 + 0x3C) = 1024.0f;
}

extern __typeof__(FUN_001f2260) func_001F2260 __attribute__((alias("FUN_001f2260")));
