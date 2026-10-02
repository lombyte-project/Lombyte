#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022e420/FUN_0022e420.s", FUN_0022e420);
#else
#include "types.h"
#include "eetypes.h"
#include "sda.h"
#include "qcopy.h"

typedef union { u128 q; f32 f[4]; } Vec4;

struct Quad {
    Vec4 v[4];         /* 0x00 */
    u32 col[4];        /* 0x40 */
    f32 uv[4][2];      /* 0x50 */
    u64 r0;            /* 0x70 */
    u64 r1;            /* 0x78 */
    u64 r2;            /* 0x80 */
    u64 r3;            /* 0x88 */
};

typedef struct {
    u8 pad00[0x20];
    s32 unk20;         /* 0x20 */
    u8 pad24[2];
    s16 unk26;         /* 0x26 */
    u8 pad28[0x50 - 0x28];
    s32 idx;           /* 0x50 */
    s32 len;           /* 0x54 */
    s32 mode;          /* 0x58 */
    s32 state;         /* 0x5C */
    u8 pad60[0xC0 - 0x60];
    Vec4 trailA[32];   /* 0xC0 */
    Vec4 trailB[32];   /* 0x2C0 */
} Scene;

typedef struct {
    u32 w0;
    u32 w1;
} Word2;

extern Scene D_0013E030;
extern Vec4 D_0013E0F0[];
extern Vec4 D_0013E2F0[];
extern s32 D_0015F604;
extern f32 D_001604D0 __attribute__((sda));
extern f32 D_001D9A10[4][2];
extern u32 D_001D9A30[];
extern u32 D_001D9A34[];

extern u64 func_001F44B8(s32);
extern void func_001F7D30(void *, s32, s32);
extern void func_001F9A10(void *, void *, void *);
extern void func_001F9A28(void *, void *, void *);
extern void func_001F9AD8(void *, void *, void *);
extern void func_001F9BF8(void *, void *, f32);
extern f32 func_001FA6C0(s32);
extern u32 func_001FA6E0(u32, u32, f32);

static inline Vec4 *trailA(s32 n) {
    return &D_0013E030.trailA[n];
}

void FUN_0022e420(void) {
    struct Quad q;
    Vec4 n[4];
    Vec4 p[4];
    s32 sel;
    s32 m;
    s32 c;
    s32 m2;
    s32 h2;
    f32 f2;
    s32 i;
    s32 j;
    s32 k;
    s32 h;
    s32 off;
    f32 f;
    Vec4 *a0;
    Vec4 *a1;
    Vec4 *a2;
    Vec4 *b0;
    Vec4 *b1;

    if (D_0015F604 == 6 && D_0013E030.unk20 == 4) {
        q.r1 = func_001F44B8(0);
    } else {
        q.r1 = func_001F44B8(0x13);
    }
    q.r2 = 0xFF9000000260;
    q.r3 = 0x8000000048;
    for (c = 0; c < 4; c++) {
        q.uv[c][0] = D_001D9A10[c][0];
        q.uv[c][1] = D_001D9A10[c][1];
    }
    for (i = 0; i < D_0013E030.len - 1; i++) {
        k = (D_0013E030.idx - i + 0x1F) & 0x1F;
        a1 = trailA((k + 1) & 0x1F);
        b1 = &D_0013E2F0[(k + 1) & 0x1F];
        a0 = trailA(k);
        b0 = &D_0013E2F0[k];
        for (j = 0; j < 2; j++) {
            q.r0 = 0;
            off = j * 32;
            a2 = trailA((k + 2) & 0x1F);
            func_001F9A28(&n[0], b0, a0);
            func_001F9A28(&n[1], a0, b0);
            func_001F9A28(&n[2], b1, a1);
            func_001F9A28(&n[3], a1, b1);
            func_001F9A28(&p[1], a1 + off, a0 + off);
            func_001F9A28(&p[3], a2 + off, a1 + off);
            if (i == 0) {
                qcopy(&p[3], &p[1]);
            }
            func_001F9AD8(&p[0], &n[0], &p[1]);
            func_001F9AD8(&p[1], &n[1], &p[1]);
            func_001F9AD8(&p[2], &n[2], &p[3]);
            func_001F9AD8(&p[3], &n[3], &p[3]);
            for (m = 0; m < 4; m++) {
                h = m >> 1;
                f = func_001FA6C0(i + 1 - h) * 0.03125f;
                q.col[m] = func_001FA6E0(D_001D9A30[D_0013E030.unk26 * 2], D_001D9A34[D_0013E030.unk26 * 2], f);
                func_001F9BF8(&q.v[m], &p[m], (1.0f - f * f) * (&D_001604D0)[D_0013E030.unk26]);
                func_001F9A10(&q.v[m], &q.v[m], trailA((k + h) & 0x1F) + off);
            }
            func_001F7D30(&q, 0, 0);
            for (m2 = 0; m2 < 4; m2++) {
                h2 = m2 >> 1;
                f2 = func_001FA6C0(i + 1 - h2) * 0.03125f;
                func_001F9BF8(&q.v[m2], &n[m2], (1.0f - f2 * f2) * (&D_001604D0)[D_0013E030.unk26]);
                func_001F9A10(&q.v[m2], &q.v[m2], trailA((k + h2) & 0x1F) + off);
            }
            func_001F7D30(&q, 0, 0);
        }
    }
}
#endif /* NON_MATCHING */
