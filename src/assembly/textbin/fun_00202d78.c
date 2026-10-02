#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00202d78/FUN_00202d78.s", FUN_00202d78);
#else
#include "types.h"

struct DrawIn {
    u8 pad_0[4];
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
};

extern s32 D_0015EE8C;
extern u64 D_0019E6C0[];
extern u64 D_0019E6D8[];
extern s32 func_001F97A0(s32);

void FUN_00202d78(u64 *q, struct DrawIn *in, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    s32 lo;
    s32 hi;
    s32 r1;
    s32 r2;
    s32 g;
    s32 x0;
    s32 x1;
    s32 y;
    u64 *src;
    u64 t;
    u64 u;
    u64 v;
    u64 w;
    s32 wd;

    hi = in->unk4 >> 6;
    lo = in->unk4 >> 7;
    if (hi <= 0) {
        hi = 1;
    }
    if (lo <= 0) {
        lo = 1;
    }
    r1 = func_001F97A0(in->unk4);
    r2 = func_001F97A0(in->unk6);
    g = D_0015EE8C >> 8;
    x1 = in->unkE + g;
    x0 = in->unkA + g;
    y = in->unkC + g;
    wd = in->unk8;
    if (a6 >= 0) {
        t = ((u64)a3 << 6) | 0x20;
        t = ((u64)(wd - 1) << 2) | t;
        t |= (u64)a2 << 32;
        q[0] = t;
        q += 2;
        q[0] = a4 | ((u64)a5 << 2) | ((u64)a6 << 24);
        q += 2;
        t = ((u64)r1 << 26) | 0x1300000;
        t = ((u64)hi << 14) | t;
        t |= (u64)r2 << 30;
        u = ((u64)x0 << 37) | ((u64)0x8000 << 19);
        t |= u;
        t |= (u64)-1 << 63;
        v = ((u64)lo << 14) | ((u64)y << 20);
        w = ((u64)x1 << 40) | ((u64)0x8000 << 19);
        v |= w;
        v |= (u64)0x8000 << 39;
        q[0] = t;
        q[2] = v;
    } else if (a6 < -1) {
        src = D_0019E6C0;
        if (a6 == -3) {
            src = D_0019E6D8;
        }
        t = ((u64)a2 << 32) | 0x20;
        t = ((u64)a3 << 6) | t;
        q[0] = t;
        q += 2;
        q[0] = 5;
        q += 2;
        q[0] = src[0];
        q[2] = src[2];
    } else {
        t = ((u64)a2 << 32) | 0x20;
        t = ((u64)a3 << 6) | t;
        q[0] = t;
        q += 2;
        q[0] = 5;
        q += 2;
        q[0] = ((((u64)0x8000 << 29) | 0x9980) << 19) | 0x7FFB;
        q[2] = 0;
    }
}
#endif /* NON_MATCHING */
