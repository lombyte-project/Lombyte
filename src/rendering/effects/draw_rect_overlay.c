/* Ported from rac1-decomp, the PAL decompilation (src/game/draw.c, func_001F5650). */

#include "sda.h"
#include "qcopy.h"

struct PacketCursor { int *p; };
extern struct PacketCursor D_00160F00_s __asm__("D_00160F00") MACRO_ADDR;
#define D_00160F00 (D_00160F00_s.p)
extern int D_0013E500[];
extern char D_00160820[];
extern char D_00160830[];

/* Append a GIF packet drawing the rectangle x0..x1, y0..y1 (in 16ths,
   offset by the viewport origin D_0013E500[4]/[5] - 8) as a PRIM 0x144
   sprite pair in colour rgba: the tag, the D_00160820 and D_00160830
   register descriptors (ids 0x8001/0x8004), then four XYZ values at Z
   0xFFFFF0. */
void draw_rect_overlay(int y0, int y1, int x0, int x1, unsigned long rgba) __asm__("FUN_001f52a0");

void draw_rect_overlay(int y0, int y1, int x0, int x1, unsigned long rgba) {
    D_00160F00[0] = 0x10000005;
    D_00160F00[1] = 0;
    D_00160F00[2] = 0;
    D_00160F00[3] = 0x50000005;
    {
        int *base = D_00160F00;
        D_00160F00 = base + 4;
        qcopy(D_00160F00, D_00160820);
        *(short *)(base + 4) = 0x8001;
    }
    {
        int *base = D_00160F00;
        long *p;
        D_00160F00 = base + 4;
        p = (long *)D_00160F00;
        p[0] = 0x144;
        p[1] = rgba;
    }
    {
        int *base = D_00160F00;
        D_00160F00 = base + 4;
        qcopy(D_00160F00, D_00160830);
        *(short *)(base + 4) = 0x8004;
    }
    {
        int *base = D_00160F00;
        long *p;
        D_00160F00 = base + 4;
        p = (long *)D_00160F00;
        p[0] = (x0 * 16 + D_0013E500[4] - 8)
             | ((long)(y0 * 16 + D_0013E500[5] - 8) << 16)
             | ((long)0xFFFFF0 << 32);
        p[1] = (x1 * 16 + D_0013E500[4] - 8)
             | ((long)(y0 * 16 + D_0013E500[5] - 8) << 16)
             | ((long)0xFFFFF0 << 32);
        p[2] = (x0 * 16 + D_0013E500[4] - 8)
             | ((long)(y1 * 16 + D_0013E500[5] - 8) << 16)
             | ((long)0xFFFFF0 << 32);
        p[3] = (x1 * 16 + D_0013E500[4] - 8)
             | ((long)(y1 * 16 + D_0013E500[5] - 8) << 16)
             | ((long)0xFFFFF0 << 32);
    }
    D_00160F00 = (int *)((char *)D_00160F00 + 0x20);
}

extern __typeof__(draw_rect_overlay) func_001F52A0 __attribute__((alias("FUN_001f52a0")));
