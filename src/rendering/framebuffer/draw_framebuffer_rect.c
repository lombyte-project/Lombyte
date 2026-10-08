#include "types.h"

#include "rnc/rendering/dma_tag.h"

extern void vu1_add_vif_code(s32) __asm__("func_00233938");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

void draw_framebuffer_rect(s32 x0, s32 y0, s32 x1, s32 y1, s32 ox, s32 oy,
                           u32 color) __asm__("FUN_001fb8f0");

void draw_framebuffer_rect(s32 x0, s32 y0, s32 x1, s32 y1, s32 ox, s32 oy, u32 color) {
    struct DmaTag *tag;
    u64 *q;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;

    vu1_add_vif_code(0x13000000);
    vu1_add_g_sregister(0x42, 0x64);
    render_packet_cursor.tag->tag = 0x10000006;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000006;
    ay = y0 << 4;
    by = y1 << 4;
    ax = x0 << 4;
    bx = x1 << 4;
    by += 0x8000;
    ay += 0x8000;
    bx += 0x8000;
    ax += 0x8000;
    by -= oy << 3;
    ay -= oy << 3;
    bx -= ox << 3;
    ax -= ox << 3;
    tag = render_packet_cursor.tag;
    q = (u64 *)(tag + 1);
    render_packet_cursor.tag = tag + 1;
    q[0] = 0x1000000000000001;
    q[1] = 0xE;
    q[2] = 0x33003;
    q[3] = 0x47;
    q[4] = 0x2400000000000001;
    q[5] = 0x10;
    q[6] = 0x106;
    q[7] = color;
    q[8] = 0x2400000000008001;
    q[9] = 0x44;
    q[10] = ax | ((u64)ay << 16);
    q[11] = bx | ((u64)by << 16);
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x60);
    vu1_add_g_sregister(0x42, 0x8000000044ULL);
    vu1_add_vif_code(0x13000000);
}

extern __typeof__(draw_framebuffer_rect) func_001FB8F0 __attribute__((alias("FUN_001fb8f0")));
