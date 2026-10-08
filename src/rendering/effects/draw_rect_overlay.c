/* Ported from rac1-decomp (src/game/draw.c, func_001F5650). */

#include "sda.h"
#include "qcopy.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"

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
    render_packet_cursor.words[0] = 0x10000005;
    render_packet_cursor.words[1] = 0;
    render_packet_cursor.words[2] = 0;
    render_packet_cursor.words[3] = 0x50000005;
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
        p[0] = 0x144;
        p[1] = rgba;
    }
    {
        int *base = render_packet_cursor.words;
        render_packet_cursor.words = base + 4;
        qcopy(render_packet_cursor.words, D_00160830);
        *(short *)(base + 4) = 0x8004;
    }
    {
        int *base = render_packet_cursor.words;
        long *p;
        render_packet_cursor.words = base + 4;
        p = (long *)render_packet_cursor.words;
        p[0] = (x0 * 16 + D_0013E500[4] - 8) | ((long)(y0 * 16 + D_0013E500[5] - 8) << 16) |
               ((long)0xFFFFF0 << 32);
        p[1] = (x1 * 16 + D_0013E500[4] - 8) | ((long)(y0 * 16 + D_0013E500[5] - 8) << 16) |
               ((long)0xFFFFF0 << 32);
        p[2] = (x0 * 16 + D_0013E500[4] - 8) | ((long)(y1 * 16 + D_0013E500[5] - 8) << 16) |
               ((long)0xFFFFF0 << 32);
        p[3] = (x1 * 16 + D_0013E500[4] - 8) | ((long)(y1 * 16 + D_0013E500[5] - 8) << 16) |
               ((long)0xFFFFF0 << 32);
    }
    render_packet_cursor.words = (int *)((char *)render_packet_cursor.words + 0x20);
}

extern __typeof__(draw_rect_overlay) func_001F52A0 __attribute__((alias("FUN_001f52a0")));
