#include "types.h"

extern f32 D_00160EA0[3];
extern s32 D_00160EB0[3];
extern f32 D_0018CF20[];
extern f32 D_001DE7F0[4][4];
extern s32 func_001FA6D0(f32) __asm__("FUN_001fa6d0");
extern void FUN_001f9810(void *, s32);

void set_tfrag_dists(void) __asm__("FUN_00233068");

void set_tfrag_dists(void) {
    float a, b, c, ab, bc;

    D_00160EB0[0] = func_001FA6D0(D_00160EA0[0] * 1024.0f);
    D_00160EB0[1] = func_001FA6D0(D_00160EA0[1] * 1024.0f);
    D_00160EB0[2] = func_001FA6D0(D_00160EA0[2] * 1024.0f);
    a = D_00160EA0[0] * D_0018CF20[0];
    b = D_00160EA0[1] * D_0018CF20[0];
    c = D_00160EA0[2] * D_0018CF20[0];
    ab = 1.0f / (a - b);
    bc = 1.0f / (b - c);
    FUN_001f9810(D_001DE7F0, 0x40);
    D_001DE7F0[0][0] = ab * 0.5f;
    D_001DE7F0[0][1] = -ab;
    D_001DE7F0[1][0] = bc * 0.5f;
    D_001DE7F0[1][1] = -bc;
    D_001DE7F0[2][0] = b * ab * -0.5f;
    D_001DE7F0[2][1] = a * ab;
    D_001DE7F0[3][0] = c * bc * -0.5f;
    D_001DE7F0[3][1] = b * bc;
    D_001DE7F0[1][3] = b;
    D_001DE7F0[0][3] = a;
}

extern __typeof__(set_tfrag_dists) func_00233068 __attribute__((alias("FUN_00233068")));
