/* Ported from rac1-decomp (src/game/draw.c, func_001F5148). */

#include "sda.h"
#include "qcopy.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"

#include "rnc/rendering/screen.h"
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
    render_packet_cursor.words[0] = 0x10000007;
    render_packet_cursor.words[1] = 0;
    render_packet_cursor.words[2] = 0;
    render_packet_cursor.words[3] = 0x50000007;
    {
        int *base = render_packet_cursor.words;
        render_packet_cursor.words = base + 4;
        qcopy(render_packet_cursor.words, D_00160820);
        *(short *)(base + 4) = 0x8001;
    }
    {
        int *base = render_packet_cursor.words;
        long *p;
        render_packet_cursor.words = base + 4;
        p = (long *)render_packet_cursor.words;
        p[0] = 0x104;
        p[1] = 0x80000000;
    }
    {
        int *base = render_packet_cursor.words;
        render_packet_cursor.words = base + 4;
        qcopy(render_packet_cursor.words, D_00160830);
        *(short *)(base + 4) = 0x8008;
    }
    {
        int *base = render_packet_cursor.words;
        long *p;
        render_packet_cursor.words = base + 4;
        p = (long *)render_packet_cursor.words;
        p[0] = screen_extent.left | ((long)screen_extent.top << 16) | ((long)0xFFFFF3 << 32);
        p[1] = screen_extent.left | ((long)(screen_extent.top + h) << 16) | ((long)0xFFFFF3 << 32);
        p[2] = screen_extent.right | ((long)screen_extent.top << 16) | ((long)0xFFFFF3 << 32);
        p[3] = screen_extent.right | ((long)(screen_extent.top + h) << 16) | ((long)0xFFFFF3 << 32);
        p[4] = screen_extent.right | ((long)screen_extent.bottom << 16) | ((long)0xFFFFF3 << 32);
        p[5] = screen_extent.right | ((long)(screen_extent.bottom - h) << 16) | ((long)0xFFFFF3 << 32);
        p[6] = screen_extent.left | ((long)screen_extent.bottom << 16) | ((long)0xFFFFF3 << 32);
        p[7] = screen_extent.left | ((long)(screen_extent.bottom - h) << 16) | ((long)0xFFFFF3 << 32);
    }
    render_packet_cursor.words = (int *)((char *)render_packet_cursor.words + 0x40);
}

extern __typeof__(draw_letterbox_bars) func_001F4D98 __attribute__((alias("FUN_001f4d98")));
