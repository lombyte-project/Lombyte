#include "types.h"

extern f32 D_00160EA0[3];
extern s32 tfrag_lod_dists_raw[3] __asm__("D_00160EB0");
extern f32 fog_params[] __asm__("D_0018CF20");
extern f32 D_001DE7F0[4][4];
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void fast_mem_zero16(void *, s32) __asm__("FUN_001f9810");

void set_tfrag_dists(void) __asm__("FUN_00233068");

void set_tfrag_dists(void) {
    float a, b, c, ab, bc;

    tfrag_lod_dists_raw[0] = truncate_float_to_s32(D_00160EA0[0] * 1024.0f);
    tfrag_lod_dists_raw[1] = truncate_float_to_s32(D_00160EA0[1] * 1024.0f);
    tfrag_lod_dists_raw[2] = truncate_float_to_s32(D_00160EA0[2] * 1024.0f);
    a = D_00160EA0[0] * fog_params[0];
    b = D_00160EA0[1] * fog_params[0];
    c = D_00160EA0[2] * fog_params[0];
    ab = 1.0f / (a - b);
    bc = 1.0f / (b - c);
    fast_mem_zero16(D_001DE7F0, 0x40);
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
