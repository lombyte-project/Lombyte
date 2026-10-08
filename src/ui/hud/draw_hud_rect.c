#include "types.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/screen.h"

void draw_hud_rect(s32 x0, s32 y0, s32 x1, s32 y1, u64 prim, s32 pixels) __asm__("FUN_00200c80");

void draw_hud_rect(s32 x0, s32 y0, s32 x1, s32 y1, u64 prim, s32 pixels) {
    u64 *q;

    render_packet_cursor.tag->tag = 0x10000003;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000003;
    q = (u64 *)render_packet_cursor.tag++;
    q[2] = (u64)0x8800 << 47 | 1;
    q[3] = 0x4410;
    q[4] = 0x41;
    q[5] = prim;
    if (pixels != 0) {
        q[6] = (x0 + D_0013E500.left - 8) | ((u64)(y0 + D_0013E500.top - 8) << 16) |
               (u64)0xFFFFF000 << 24;
        q[7] = (x1 + D_0013E500.left - 8) | ((u64)(y1 + D_0013E500.top - 8) << 16) |
               (u64)0xFFFFF000 << 24;
    } else {
        q[6] = ((x0 << 4) + D_0013E500.left - 0x10) |
               ((u64)((y0 << 4) + D_0013E500.top - 0x10) << 16) | (u64)0xFFFFF000 << 24;
        q[7] = ((x1 << 4) + D_0013E500.left - 0x10) |
               ((u64)((y1 << 4) + D_0013E500.top - 0x10) << 16) | (u64)0xFFFFF000 << 24;
    }
    render_packet_cursor.tag += 3;
}

extern __typeof__(draw_hud_rect) func_00200C80 __attribute__((alias("FUN_00200c80")));
