#include "types.h"
#include "qcopy.h"

typedef struct {
    f32 v[4];
} __attribute__((aligned(16))) Vec;

extern u8 D_001871B0[];
extern char D_0013F350[];
extern char D_0013F3D0[];
extern char D_0013F5E0[];
extern char D_00187080[];
extern char D_00187290[];
extern s32 D_0018C32C[];

extern f32 FUN_002133d0(f32, f32, f32);
extern void func_001F9AD8(void *, void *, void *);
extern void func_001EC530(f32 *, void *, void *, void *, void *, void *);
extern void FUN_001f9bf8(void *, void *, f32);
extern f32 func_001FA5C8(f32, f32);
extern f32 func_001FA580(f32, f32);
extern void func_00214890(void *, void *, void *, f32);
extern void FUN_001f9a10(void *, void *, void *);
extern void FUN_001fa480(void *, void *);
extern f32 func_001F9AB0(void *, void *);
extern void FUN_001f9a68(void *, void *, f32);
extern void FUN_001f9a28(void *, void *, void *);
extern f32 FUN_001f9af0(void *);
extern f32 func_001F9DF8(f32);
extern f32 func_001F99C0(f32);
extern void FUN_00214530(void *, void *, f32);
extern void func_00214800(void *, void *, void *);
extern void func_002144D8(void *, void *);
extern void func_001F9740(void *);

int FUN_001eccd8(char *cam, f32 *b) __asm__("FUN_001eccd8");

int FUN_001eccd8(char *cam, f32 *b)
{
    Vec ang;
    Vec fwd;
    Vec side;
    Vec up;
    Vec target;
    Vec pos;
    Vec diff;
    Vec proj;
    Vec m[3];
    Vec r0;
    Vec r1;
    Vec r2;
    Vec q;
    f32 step;
    f32 dyaw;
    f32 dpitch;
    f32 yaw;
    f32 turn;
    f32 sign;
    f32 s2;
    f32 d;
    char *g;

    if (*(s32 *)(b + 3) <= 0) {
        return 1;
    }
    step = 1.0f / FUN_002133d0(1.0f, (f32)*(s32 *)(b + 5), (f32)*(s32 *)(b + 3) * b[4]);
    if (D_001871B0[2] == 2) {
        qcopy(&target, D_0013F3D0);
        qcopy(&fwd, b + 8);
        qcopy(&up, b + 12);
        func_001F9AD8(&side, &fwd, &up);
        func_001EC530(ang.v, cam + 0x30, &target, &fwd, &side, &up);
    } else {
        qcopy(&target, cam + 0x30);
        g = D_0013F350;
        FUN_001f9bf8(&fwd, *(char **)(g + 0x2080) + 0xC0, 1.0f);
        FUN_001f9bf8(&up, *(char **)(g + 0x2080) + 0xE0, 1.0f);
        ang.v[2] = 0.0f;
        ang.v[1] = 0.0f;
        ang.v[0] = 3.1415927f;
    }
    dyaw = func_001FA5C8(ang.v[0], b[0]);
    b[0] = func_001FA580(b[0], dyaw * step);
    dpitch = func_001FA5C8(ang.v[1], b[1]);
    b[1] = func_001FA580(b[1], dpitch * step);
    b[2] = b[2] + (ang.v[2] - b[2]) * step;
    FUN_001f9bf8(&pos, &fwd, b[2]);
    func_00214890(&pos, &pos, &up, b[0]);
    func_001F9AD8(&side, &pos, &up);
    FUN_001f9bf8(&side, &side, 1.0f);
    func_00214890(&pos, &pos, &side, b[1]);
    FUN_001f9a10(b + 20, &target, &pos);
    if (D_0018C32C[0] == 0) {
        qcopy(D_00187080, b + 20);
    }
    FUN_001fa480(b + 16, m);
    d = func_001F9AB0(&m[2], cam);
    FUN_001f9a68(&proj, &m[2], d);
    FUN_001f9a28(&diff, cam, &proj);
    yaw = 1.5707964f - func_001F9DF8(func_001F9AB0(&m[0], &diff) / FUN_001f9af0(&diff));
    sign = -1.0f;
    if (func_001F9AB0(&diff, &m[1]) >= 0.0f) {
        sign = 1.0f;
    }
    yaw = yaw * sign;
    if (func_001F99C0(dyaw) > 1.5707964f
        && ((dyaw >= 0.0f && sign < 0.0f) || (dyaw < 0.0f && sign >= 0.0f))) {
        if (yaw < 0.0f) {
            yaw += 6.2831855f;
        } else {
            yaw -= 6.2831855f;
        }
    }
    turn = yaw * step;
    if (func_001F99C0(turn) < 1e-5f) {
        qcopy(&r0, &m[0]);
        qcopy(&r1, &m[1]);
    } else {
        FUN_00214530(&r2, &m[2], turn);
        func_00214800(&r0, &m[0], &r2);
        func_00214800(&r1, &m[1], &r2);
    }
    if (func_001F99C0(yaw) < 1e-5f) {
        qcopy(&r2, &m[0]);
    } else {
        FUN_00214530(&q, &m[2], yaw);
        func_00214800(&r2, &m[0], &q);
    }
    yaw = 1.5707964f - func_001F9DF8(func_001F9AB0(&r2, cam));
    d = func_001F9AB0(&r2, cam + 0x20);
    s2 = -1.0f;
    if (d >= 0.0f) {
        s2 = 1.0f;
    }
    yaw *= s2;
    func_00214890(&r0, &r0, &r1, yaw * step);
    FUN_001f9bf8(D_00187290, &r0, 1.0f);
    func_001F9AD8(D_00187290 + 0x10, D_00187290, D_0013F5E0);
    FUN_001f9bf8(D_00187290 + 0x10, D_00187290 + 0x10, -1.0f);
    func_001F9AD8(D_00187290 + 0x20, D_00187290 + 0x10, D_00187290);
    func_002144D8(b + 24, D_00187290);
    func_002144D8(b + 16, D_00187290);
    func_001F9740(b + 3);
    return 0;
}

extern __typeof__(FUN_001eccd8) func_001ECCD8 __attribute__((alias("FUN_001eccd8")));
