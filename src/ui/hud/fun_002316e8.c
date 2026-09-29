/* Ported from rac1-decomp, the PAL decompilation (src/game/space.c, func_00232A00). */
#include "sda.h"
#include "qcopy.h"

struct TagPtr { int *p; };
extern struct TagPtr D_00160F00;
extern int D_0013E500[];
extern char D_00160850[];

typedef union {
    struct {
        float u;
        float v;
    } f;
    long bits;
} SpaceUvPair;

/* Append a textured four-corner GIF packet. Screen coordinates use 12.4
   fixed point relative to the viewport origin; UV pairs stay as floats. */
void FUN_002316e8(int x, int y, int w, int h, unsigned long rgba,
                  unsigned long tex, float u0, float u1, float v0, float v1) __asm__("FUN_002316e8");

void FUN_002316e8(int x, int y, int w, int h, unsigned long rgba,
                  unsigned long tex, float u0, float u1, float v0, float v1) {
    int x0 = x * 16 + D_0013E500[4] - 8;
    int x1 = (x + w) * 16 + D_0013E500[4] - 8;
    int y0 = y * 16 + D_0013E500[5] - 8;
    int y1 = (y + h) * 16 + D_0013E500[5] - 8;
    SpaceUvPair uv[4];
    long *p;
    int *base;

    uv[0].f.u = u0;
    uv[0].f.v = v0;
    uv[1].f.u = u1;
    uv[1].f.v = v0;
    uv[2].f.u = u0;
    uv[2].f.v = v1;
    uv[3].f.u = u1;
    uv[3].f.v = v1;

    D_00160F00.p[0] = 0x10000007;
    D_00160F00.p[1] = 0;
    D_00160F00.p[2] = 0;
    D_00160F00.p[3] = 0x50000007;
    base = D_00160F00.p;
    D_00160F00.p = base + 4;
    qcopy(base + 4, D_00160850);
    p = (long *)(base + 8);
    D_00160F00.p = base + 8;
    p[0] = tex;
    p[1] = 0x54;
    p[2] = (rgba & 0xFFFFFFFFL) | (0xFE00L << 46);
    p[3] = uv[0].bits;
    p[4] = x0 | ((long)y0 << 16) | ((long)0xFFFFF0 << 32);
    p[5] = uv[1].bits;
    p[6] = x1 | ((long)y0 << 16) | ((long)0xFFFFF0 << 32);
    p[7] = uv[2].bits;
    p[8] = x0 | ((long)y1 << 16) | ((long)0xFFFFF0 << 32);
    p[9] = uv[3].bits;
    p[10] = x1 | ((long)y1 << 16) | ((long)0xFFFFF0 << 32);
    p[11] = 0;
    D_00160F00.p = (int *)((char *)D_00160F00.p + 0x60);
}

extern __typeof__(FUN_002316e8) func_002316E8 __attribute__((alias("FUN_002316e8")));
