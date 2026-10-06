/* Ported from rac1-decomp (src/game/draw.c, func_001F5148). */

#include "sda.h"
#include "qcopy.h"

struct PacketCursor {
    int *p;
};
extern struct PacketCursor D_00160F00_s __asm__("D_00160F00") MACRO_ADDR;
#define D_00160F00 (D_00160F00_s.p)
extern int D_0013E500[];
extern int D_0015F444 MACRO_ADDR;
extern int D_0015F448 MACRO_ADDR;
extern char D_00160820[];
extern char D_00160830[];

/* Letterbox bars: while D_0015F444 is set the bar height D_0015F448
   grows to 24, otherwise it shrinks to 0. While it is non-zero, append
   a GIF packet (the D_00160820/D_00160830 register descriptors, PRIM
   0x104) drawing two full-width strips, the height in 16ths reaching in
   from the top and bottom of the D_0013E500 viewport, the same packet
   steps as FUN_001f52a0. */
void draw_letterbox_bars(void) __asm__("FUN_001f4d98");

void draw_letterbox_bars(void) {
    int h;

    if (D_0015F444 != 0) {
        if (D_0015F448 < 24) {
            D_0015F448++;
        }
    } else {
        if (D_0015F448 == 0) {
            return;
        }
        D_0015F448--;
    }
    h = D_0015F448;
    if (h == 0) {
        return;
    }
    h <<= 4;
    D_00160F00[0] = 0x10000007;
    D_00160F00[1] = 0;
    D_00160F00[2] = 0;
    D_00160F00[3] = 0x50000007;
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
        p[0] = 0x104;
        p[1] = 0x80000000;
    }
    {
        int *base = D_00160F00;
        D_00160F00 = base + 4;
        qcopy(D_00160F00, D_00160830);
        *(short *)(base + 4) = 0x8008;
    }
    {
        int *base = D_00160F00;
        long *p;
        D_00160F00 = base + 4;
        p = (long *)D_00160F00;
        p[0] = D_0013E500[4] | ((long)D_0013E500[5] << 16) | ((long)0xFFFFF3 << 32);
        p[1] = D_0013E500[4] | ((long)(D_0013E500[5] + h) << 16) | ((long)0xFFFFF3 << 32);
        p[2] = D_0013E500[6] | ((long)D_0013E500[5] << 16) | ((long)0xFFFFF3 << 32);
        p[3] = D_0013E500[6] | ((long)(D_0013E500[5] + h) << 16) | ((long)0xFFFFF3 << 32);
        p[4] = D_0013E500[6] | ((long)D_0013E500[7] << 16) | ((long)0xFFFFF3 << 32);
        p[5] = D_0013E500[6] | ((long)(D_0013E500[7] - h) << 16) | ((long)0xFFFFF3 << 32);
        p[6] = D_0013E500[4] | ((long)D_0013E500[7] << 16) | ((long)0xFFFFF3 << 32);
        p[7] = D_0013E500[4] | ((long)(D_0013E500[7] - h) << 16) | ((long)0xFFFFF3 << 32);
    }
    D_00160F00 = (int *)((char *)D_00160F00 + 0x40);
}

extern __typeof__(draw_letterbox_bars) func_001F4D98 __attribute__((alias("FUN_001f4d98")));
