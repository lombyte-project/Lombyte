#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00237c80/FUN_00237c80.s", FUN_00237c80);
#else
#include "types.h"
typedef unsigned int u128 __attribute__((mode(TI)));

struct Display {
    u8 pad_0[0x190];
    f32 sx;
    f32 sy;
};

struct ScreenOfs {
    u8 pad_0[8];
    s32 x;
    s32 y;
};

extern u8 D_00187080[];
extern struct Display D_0018CD00;
extern struct ScreenOfs D_0013E500;

extern void func_001F9A28(void *, void *, void *);
extern void func_001F9A68(void *, void *, f32);
extern void func_001F9D20(void *, void *, void *);

void fun_00237c80(u128 *a, u128 *b, f32 *w, f32 *h, f32 *x, f32 *y) __asm__("FUN_00237c80");

void fun_00237c80(u128 *a, u128 *b, f32 *w, f32 *h, f32 *x, f32 *y) {
    f32 p0[4] __attribute__((aligned(16)));
    f32 p1[4] __attribute__((aligned(16)));
    f32 *q;
    f32 sx;
    f32 sy;

    q = p1;
    *(u128 *)p0 = *a;
    *(u128 *)q = *b;
    func_001F9A28(p0, p0, D_00187080);
    func_001F9A28(q, q, D_00187080);
    func_001F9A68(p0, p0, 1024.0f);
    func_001F9A68(q, q, 1024.0f);
    q[3] = 1.0f;
    p0[3] = 1.0f;
    func_001F9D20(p0, p0, D_00187080 - 0x40);
    func_001F9D20(q, q, D_00187080 - 0x40);
    p0[0] *= 1.0f / p0[3];
    p0[1] *= 1.0f / p0[3];
    q[0] *= 1.0f / q[3];
    q[1] *= 1.0f / q[3];
    sx = D_0018CD00.sx;
    sy = D_0018CD00.sy;
    p0[0] *= sx;
    p0[1] *= sy;
    q[0] *= sx;
    q[1] *= sy;
    *x = p0[0] * 0.25f + (f32)D_0013E500.x;
    *y = p0[1] * 0.25f + (f32)D_0013E500.y;
    *w = (q[0] - p0[0]) * 0.25f;
    *h = (p1[1] - p0[1]) * 0.25f;
}
#endif /* NON_MATCHING */
