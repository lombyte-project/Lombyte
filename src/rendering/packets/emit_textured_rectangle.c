#include "types.h"
#include "qcopy.h"

#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/screen.h"
#include "rnc/math/asm_helpers.h"

extern char textured_rectangle_packet_header_template[] __asm__("D_00160860");

void emit_textured_rectangle(f32 x,
                             f32 y,
                             f32 w,
                             f32 h,
                             s32 u,
                             s32 v,
                             s32 uw,
                             s32 vh,
                             u64 rgba,
                             u64 tex) __asm__("FUN_001f5808");

void emit_textured_rectangle(f32 x,
                             f32 y,
                             f32 screen_width,
                             f32 screen_height,
                             s32 u,
                             s32 v,
                             s32 uw,
                             s32 vh,
                             u64 rgba,

                             u64 texture_packet_word) {
    s32            x0 = truncate_float_to_s32(x * 16.0f) + screen_extent.left - 8;
    s32            x1 = truncate_float_to_s32((x + screen_width) * 16.0f) + screen_extent.left - 8;
    s32            y0 = truncate_float_to_s32(y * 16.0f) + screen_extent.top - 8;
    s32            y1 = truncate_float_to_s32((y + screen_height) * 16.0f) + screen_extent.top - 8;
    s32            u1_fixed;
    s32            u0_fixed;
    s32            vb;
    s32            ub;
    long          *packet_words;
    struct DmaTag *rectangle_dma_tag;

    if (x0 > 0x9000 || x1 < 0x7000 || y0 > 0x9000 || y1 < 0x7000) {
        return;
    }
    render_packet_cursor.tag->tag = 0x10000008;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000008;
    ub = u + uw;
    vb = v + vh;
    u0_fixed = u * 16;
    u1_fixed = ub * 16;
    rectangle_dma_tag = render_packet_cursor.tag;
    render_packet_cursor.tag = rectangle_dma_tag + 1;
    qcopy(rectangle_dma_tag + 1, textured_rectangle_packet_header_template);
    packet_words = (long *)(rectangle_dma_tag + 2);
    render_packet_cursor.tag = rectangle_dma_tag + 2;
    packet_words[0] = texture_packet_word;
    packet_words[1] = 0x154;
    packet_words[2] =
        (u64)(0xA | ((long)u << 4) | ((long)ub << 14)) | ((long)v << 24) | ((long)vb << 34);
    packet_words[3] = rgba;
    packet_words[4] = (v << 20) + u0_fixed;
    packet_words[5] = x0 | ((long)y0 << 16) | ((long)0xFFFFF0 << 32);
    packet_words[6] = (v << 20) + u1_fixed;
    packet_words[7] = x1 | ((long)y0 << 16) | ((long)0xFFFFF0 << 32);
    packet_words[8] = (vb << 20) + u0_fixed;
    packet_words[9] = x0 | ((long)y1 << 16) | ((long)0xFFFFF0 << 32);
    packet_words[10] = (vb << 20) + u1_fixed;
    packet_words[11] = x1 | ((long)y1 << 16) | ((long)0xFFFFF0 << 32);
    packet_words[12] = 5;
    packet_words[13] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x70);
}

extern __typeof__(emit_textured_rectangle) func_001F5808 __attribute__((alias("FUN_001f5808")));
