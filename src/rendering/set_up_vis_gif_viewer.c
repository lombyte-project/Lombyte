#include "types.h"

extern u64 D_0019E540[];
extern u64 D_0019E6C0[];
extern u64 D_0019E6D8[];

void set_up_vis_gif_viewer(u64 *q, s32 n, s32 prim, s32 a3, s32 t0, s32 mode) __asm__("FUN_00202fd0");

void set_up_vis_gif_viewer(u64 *q, s32 n, s32 prim, s32 a3, s32 t0, s32 mode) {
    u64 w0;
    s64 w1;
    u64 w2;
    u64 *src;

    w0 = D_0019E540[mode * 3];
    w1 = D_0019E540[mode * 3 + 1];
    w2 = D_0019E540[mode * 3 + 2];
    if (mode >= 0) {
        q[0] = (0x20 | (w1 & 0x1C)) | ((u64)prim << 6) | ((u64)(u32)n << 32);
        q += 2;
        q[0] = (a3 | ((u64)t0 << 2)) | ((u64)mode << 24);
        q += 2;
        q[0] = w0;
        q[2] = w2;
    } else if (mode < -1) {
        src = D_0019E6C0;
        if (mode == -3) {
            src = D_0019E6D8;
        }
        q[0] = (0x20 | ((u64)prim << 6)) | ((u64)(u32)n << 32);
        q += 2;
        q[0] = 5;
        q += 2;
        q[0] = src[0];
        q[2] = src[2];
    } else {
        q[0] = (0x20 | ((u64)prim << 6)) | ((u64)(u32)n << 32);
        q += 2;
        q[0] = 5;
        q += 2;
        q[0] = ((u64)0x8000 << 29 | 0x9980) << 19 | 0x7FFB;
        q[2] = 0;
    }
}

extern __typeof__(set_up_vis_gif_viewer) func_00202FD0 __attribute__((alias("FUN_00202fd0")));
