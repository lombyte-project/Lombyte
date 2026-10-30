#include "types.h"
#include "rnc/rendering/screen.h"
#include "qcopy.h"

#include "rnc/rendering/dma_tag.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"

void draw_textured_quad(s32 x,
                        s32 y,
                        s32 w,
                        s32 h,
                        s32 u,
                        s32 v,
                        s32 uw,
                        s32 vh,
                        s64 color,
                        s64 extra) __asm__("FUN_001f5450");

void draw_textured_quad(s32 x,
                        s32 y,
                        s32 quad_width,
                        s32 quad_height,
                        s32 u,
                        s32 v,
                        s32 uw,
                        s32 vh,
                        s64 color,

                        s64 extra) {
    struct DmaTag *tag;
    u64           *packet_words;
    s32            sx;
    s32            sy;
    s32            x1;
    s32            y1;
    s32            x0;
    s32            y0;
    s32            u1;
    s32            u0;
    s32            texture_v_end;

    sx = screen_extent.left;
    sy = screen_extent.top;
    x0 = (x << 4) + sx - 8;
    x1 = ((x + quad_width) << 4) + sx - 8;
    y0 = (y << 4) + sy - 8;
    y1 = ((y + quad_height) << 4) + sy - 8;
    u1 = (u + uw) << 4;
    u0 = u << 4;
    texture_v_end = v + vh;
    render_packet_cursor.tag->tag = 0x10000007;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000007;
    tag = render_packet_cursor.tag;
    render_packet_cursor.tag = tag + 1;
    qcopy(tag + 1, textured_quad_header);
    packet_words = (u64 *)(tag + 2);
    render_packet_cursor.tag = tag + 2;
    packet_words[0] = extra;
    packet_words[1] = 0x154;
    packet_words[2] = color;
    packet_words[3] = (v << 20) + u0;
    packet_words[4] = x0 | ((u64)y0 << 16) | 0xFFFFF000000000;
    packet_words[5] = (v << 20) + u1;
    packet_words[6] = x1 | ((u64)y0 << 16) | 0xFFFFF000000000;
    packet_words[7] = (texture_v_end << 20) + u0;
    packet_words[8] = x0 | ((u64)y1 << 16) | 0xFFFFF000000000;
    packet_words[9] = (texture_v_end << 20) + u1;
    packet_words[10] = x1 | ((u64)y1 << 16) | 0xFFFFF000000000;
    packet_words[11] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x60);
}

extern __typeof__(draw_textured_quad) func_001F5450 __attribute__((alias("FUN_001f5450")));
