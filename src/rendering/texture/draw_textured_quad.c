#include "types.h"
#include "rnc/rendering/screen.h"
#include "qcopy.h"

#include "rnc/rendering/dma_tag.h"
extern char D_00160840[];

void draw_textured_quad(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s64 color,
                        s64 extra) __asm__("FUN_001f5450");

void draw_textured_quad(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s64 color,
                        s64 extra) {
    struct DmaTag *tag;
    u64 *q;
    s32 sx;
    s32 sy;
    s32 x1;
    s32 y1;
    s32 x0;
    s32 y0;
    s32 u1;
    s32 u0;
    s32 v1;

    sx = D_0013E500.left;
    sy = D_0013E500.top;
    x0 = (x << 4) + sx - 8;
    x1 = ((x + w) << 4) + sx - 8;
    y0 = (y << 4) + sy - 8;
    y1 = ((y + h) << 4) + sy - 8;
    u1 = (u + uw) << 4;
    u0 = u << 4;
    v1 = v + vh;
    render_packet_cursor.tag->tag = 0x10000007;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000007;
    tag = render_packet_cursor.tag;
    render_packet_cursor.tag = tag + 1;
    qcopy(tag + 1, D_00160840);
    q = (u64 *)(tag + 2);
    render_packet_cursor.tag = tag + 2;
    q[0] = extra;
    q[1] = 0x154;
    q[2] = color;
    q[3] = (v << 20) + u0;
    q[4] = x0 | ((u64)y0 << 16) | 0xFFFFF000000000;
    q[5] = (v << 20) + u1;
    q[6] = x1 | ((u64)y0 << 16) | 0xFFFFF000000000;
    q[7] = (v1 << 20) + u0;
    q[8] = x0 | ((u64)y1 << 16) | 0xFFFFF000000000;
    q[9] = (v1 << 20) + u1;
    q[10] = x1 | ((u64)y1 << 16) | 0xFFFFF000000000;
    q[11] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x60);
}

extern __typeof__(draw_textured_quad) func_001F5450 __attribute__((alias("FUN_001f5450")));
