#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f5ab0/FUN_001f5ab0.s", FUN_001f5ab0);
#else
#include "types.h"

struct DmaTag {
    u32 w0;
    u32 addr;
    u32 w2;
    u32 w3;
};

struct TagPtr {
    struct DmaTag *p;
};

struct Vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

struct ScreenOfs {
    u8 pad0[0x10];
    s32 x;
    s32 y;
};

extern struct TagPtr D_00160F00;
extern struct ScreenOfs D_0013E500;
extern void func_001F9A10(void *, void *, void *);
extern void func_001F9A28(void *, void *, void *);
extern void func_001F9A68(void *, void *, f32);
extern f32 func_001F9DC8(f32);
extern f32 func_001F9DE0(f32);
extern s32 func_001FA6D0(f32);

void FUN_001f5ab0(s32 x, s32 y, s64 buf, s64 z, s32 clr, u8 flipx, u8 flipy, f32 px, f32 py, f32 sy, f32 sx, f32 ang, f32 u, f32 v)
{
    struct Vec4 sz;
    struct Vec4 off;
    struct Vec4 pos;
    struct Vec4 t;
    struct Vec4 p0;
    struct Vec4 p1;
    struct Vec4 p2;
    struct Vec4 p3;
    struct DmaTag *tag;
    u64 *q;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    f32 nu;
    f32 nv;

    if (flipx != 0) {
        a = x << 4;
        b = 0x10;
    } else {
        b = x << 4;
        a = 0x10;
    }
    if (flipy != 0) {
        c = y << 20;
        d = 0x100000;
    } else {
        d = y << 20;
        c = 0x100000;
    }
    pos.x = px;
    pos.y = py;
    nu = 1.0f - u;
    sz.x = sx * func_001F9DE0(ang);
    nv = 1.0f - v;
    sz.y = sx * func_001F9DC8(ang);
    off.x = sy * func_001F9DC8(ang);
    off.y = -sy * func_001F9DE0(ang);
    func_001F9A68(&t, &sz, nv);
    func_001F9A10(&p0, &pos, &t);
    func_001F9A68(&t, &off, nu);
    func_001F9A28(&p0, &p0, &t);
    func_001F9A68(&t, &sz, nv);
    func_001F9A10(&p1, &pos, &t);
    func_001F9A68(&t, &off, u);
    func_001F9A10(&p1, &p1, &t);
    func_001F9A68(&t, &sz, v);
    func_001F9A28(&p2, &pos, &t);
    func_001F9A68(&t, &off, nu);
    func_001F9A28(&p2, &p2, &t);
    func_001F9A68(&t, &sz, v);
    func_001F9A28(&p3, &pos, &t);
    func_001F9A68(&t, &off, u);
    func_001F9A10(&p3, &p3, &t);
    D_00160F00.p->w0 = 0x10000007;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000007;
    tag = D_00160F00.p;
    q = (u64 *)(tag + 1);
    D_00160F00.p = tag + 1;
    q[0] = 0xB400000000008001;
    q[1] = 0x53535353106;
    q[2] = buf;
    q[3] = 0x154;
    q[4] = clr;
    q[5] = a | c;
    q[6] = (func_001FA6D0(p0.x * 16.0f) + D_0013E500.x - 8) | ((u64)(func_001FA6D0(p0.y * 16.0f) + D_0013E500.y - 8) << 16) | ((u64)z << 32);
    q[7] = b | c;
    q[8] = (func_001FA6D0(p1.x * 16.0f) + D_0013E500.x - 8) | ((u64)(func_001FA6D0(p1.y * 16.0f) + D_0013E500.y - 8) << 16) | ((u64)z << 32);
    q[9] = a | d;
    q[10] = (func_001FA6D0(p2.x * 16.0f) + D_0013E500.x - 8) | ((u64)(func_001FA6D0(p2.y * 16.0f) + D_0013E500.y - 8) << 16) | ((u64)z << 32);
    q[11] = b | d;
    q[12] = (func_001FA6D0(p3.x * 16.0f) + D_0013E500.x - 8) | ((u64)(func_001FA6D0(p3.y * 16.0f) + D_0013E500.y - 8) << 16) | ((u64)z << 32);
    q[13] = 0;
    D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x70);
}
#endif /* NON_MATCHING */
