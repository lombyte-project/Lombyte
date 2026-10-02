#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f92b0/FUN_001f92b0.s", FUN_001f92b0);
#else
#include "types.h"
#include "qcopy.h"

typedef struct { f32 x, y, z, w; } Vec4;

typedef struct {
    Vec4 pos;
    s16 life;
    s16 alpha;
    u8 pad14[4];
    f32 angle;
    f32 size;
} Spark;

typedef struct {
    u8 pad0[0x1A8];
    f32 zofs;
    u8 pad1AC[0x64];
    f32 proj;
} Camera;

struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct DmaTag *p; };

extern struct TagPtr D_00160F00;
extern char D_001608E0[];
extern u8 D_00187080[];
extern Camera D_0018CD00;
extern s32 D_0018CE80;
extern Spark D_0018ED00[];
extern void FillTransferWords(void *, s32, s32);
extern long func_001F44B8(s32);
extern s32 func_001F9958(Vec4 *);
extern void func_001F9A28(Vec4 *, void *, void *);
extern void func_001F9A68(Vec4 *, Vec4 *, f32);
extern void func_001F9A98(Vec4 *, Vec4 *, void *);
extern f32 func_001F9AF0(Vec4 *);
extern void func_001F9D20(Vec4 *, Vec4 *, void *);
extern f32 func_001F9DC8(f32);
extern f32 func_001F9DE0(f32);
extern f32 func_001FA6C0(s32);
extern s32 func_001FA6D0(f32);
extern void func_00233980(s32, u64);

void FUN_001f92b0(void)
{
    Vec4 v;
    Vec4 w;
    f32 dist;
    f32 r;
    s32 i;
    Spark *pt;
    long col;
    long x;
    s32 y;
    long xy;
    s32 s;
    s32 c;
    struct DmaTag *base;
    long *p;
    Spark *arr;

    func_00233980(0x42, 0x8000000048);
    for (i = 0; i < 16; i++) {
        arr = D_0018ED00;
        pt = arr + i;
        if (pt->life <= 0) {
            continue;
        }
        func_001F9A28(&v, pt, D_00187080);
        v.w = 1.0f;
        dist = func_001F9AF0(&v);
        func_001F9A68(&v, &v, 1024.0f);
        func_001F9D20(&v, &v, D_00187080 - 0x100);
        func_001F9A98(&w, &v, &D_0018CE80);
        if (func_001F9958(&w) != 0) {
            FillTransferWords(pt, 0, 0x20);
            continue;
        }
        func_001F9A68(&v, &v, D_0018CD00.proj / v.w);
        col = (pt->alpha << 24) | 0x808080;
        x = func_001FA6D0(v.x * 16.0f) + 0x8000;
        y = func_001FA6D0(v.y * 16.0f) + 0x8000;
        xy = ((long)func_001FA6D0(v.z * 0.9997f + D_0018CD00.zofs) << 32) | ((long)y << 16) | x;
        if (dist > 18.0f) {
            dist = 18.0f;
        } else if (dist < 2.0f) {
            dist = 2.0f;
        }
        r = pt->size * (func_001FA6C0(pt->alpha + 16) * 0.015625f) * ((24.0f - dist) * 16.0f);
        s = func_001FA6D0(r * func_001F9DE0(pt->angle));
        c = func_001FA6D0(r * func_001F9DC8(pt->angle));
        D_00160F00.p->w0 = 0x10000009;
        D_00160F00.p->addr = 0;
        D_00160F00.p->w2 = 0;
        D_00160F00.p->w3 = 0x50000009;
        base = D_00160F00.p;
        D_00160F00.p = base + 1;
        qcopy(base + 1, D_001608E0);
        p = (long *)(base + 2);
        D_00160F00.p = base + 2;
        p[0] = 5;
        p[1] = func_001F44B8(0x13);
        p[2] = 0x154;
        p[3] = col;
        p[4] = 0;
        p[5] = xy + (c << 16) + s;
        p[6] = col;
        p[7] = 0x200;
        p[8] = xy + (-s << 16) + c;
        p[9] = col;
        p[10] = 0x2000000;
        p[11] = xy + (s << 16) - c;
        p[12] = col;
        p[13] = 0x2000200;
        p[14] = xy + (-c << 16) - s;
        p[15] = 0;
        D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x80);
    }
    func_00233980(0x42, 0x8000000044);
}
#endif /* NON_MATCHING */
