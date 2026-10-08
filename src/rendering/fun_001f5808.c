#include "types.h"
#include "qcopy.h"

#include "rnc/rendering/dma_tag.h"
extern s32 D_0013E500[];
extern char D_00160860[];
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");

void FUN_001f5808(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh, u64 rgba, u64 tex) {
    s32 x0 = truncate_float_to_s32(x * 16.0f) + D_0013E500[4] - 8;
    s32 x1 = truncate_float_to_s32((x + w) * 16.0f) + D_0013E500[4] - 8;
    s32 y0 = truncate_float_to_s32(y * 16.0f) + D_0013E500[5] - 8;
    s32 y1 = truncate_float_to_s32((y + h) * 16.0f) + D_0013E500[5] - 8;
    s32 s1;
    s32 s0;
    s32 vb;
    s32 ub;
    long *p;
    struct DmaTag *base;

    if (x0 > 0x9000 || x1 < 0x7000 || y0 > 0x9000 || y1 < 0x7000) {
        return;
    }
    render_packet_cursor.tag->tag = 0x10000008;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000008;
    ub = u + uw;
    vb = v + vh;
    s0 = u * 16;
    s1 = ub * 16;
    base = render_packet_cursor.tag;
    render_packet_cursor.tag = base + 1;
    qcopy(base + 1, D_00160860);
    p = (long *)(base + 2);
    render_packet_cursor.tag = base + 2;
    p[0] = tex;
    p[1] = 0x154;
    p[2] = (u64)(0xA | ((long)u << 4) | ((long)ub << 14)) | ((long)v << 24) | ((long)vb << 34);
    p[3] = rgba;
    p[4] = (v << 20) + s0;
    p[5] = x0 | ((long)y0 << 16) | ((long)0xFFFFF0 << 32);
    p[6] = (v << 20) + s1;
    p[7] = x1 | ((long)y0 << 16) | ((long)0xFFFFF0 << 32);
    p[8] = (vb << 20) + s0;
    p[9] = x0 | ((long)y1 << 16) | ((long)0xFFFFF0 << 32);
    p[10] = (vb << 20) + s1;
    p[11] = x1 | ((long)y1 << 16) | ((long)0xFFFFF0 << 32);
    p[12] = 5;
    p[13] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x70);
}

extern __typeof__(FUN_001f5808) func_001F5808 __attribute__((alias("FUN_001f5808")));
