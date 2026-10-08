#include "types.h"

#include "rnc/rendering/dma_tag.h"

extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

/* Appends a GIF packet to render_packet_cursor: a fixed 10-quad header (the last
   quads carry n = w / 32), then n pairs of sprite corner registers stepping
   0x200 (in 1/16 pixels) per column across a w x h area centred on 0x8000.
   c is volatile so the two pointer increments per pair stay separate, and
   zero / step are shared constants as in retail. */
void FUN_001fb740(s32 w, s32 h) {
    struct DmaTag *tag;
    u64 *q;
    volatile u64 *c;
    s32 n;
    s32 i;
    s32 zero;
    s32 step;
    s32 x;
    s32 y;
    u64 lo;
    u64 hi;

    n = w / 32;
    vu1_add_g_sregister(0x42, 0x800000004AULL);
    render_packet_cursor.tag->tag = (n + 5) | 0x10000000;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = (n + 5) | 0x50000000;
    tag = render_packet_cursor.tag;
    q = (u64 *)(tag + 1);
    render_packet_cursor.tag = tag + 1;
    zero = 0;
    q[zero] = 0x1000000000000001;
    q[1] = 0xE;
    q[2] = 0x32003;
    q[3] = 0x47;
    q[4] = 0x2400000000000001;
    q[5] = 0x10;
    q[6] = 0x146;
    q[7] = 0x80008080;
    q[8] = (n | 0x8000) | 0x2400000000000000;
    q[9] = 0x44;
    i = zero;
    if (n > zero) {
        lo = (u64)(0x8000 - h * 8) << 16;
        x = 0x8000 + -(w * 8);
        c = (volatile u64 *)((u8 *)tag + 0x60);
        y = 0x8200 + -(w * 8);
        hi = (u64)(h * 8 + 0x7FF0) << 16;
        do {
            step = 0x200;
            *c++ = x | lo;
            *c++ = y | hi;
            y += step;
            x += step;
            i++;
        } while (i < n);
    }
    render_packet_cursor.tag = render_packet_cursor.tag + (n + 5);
}

extern __typeof__(FUN_001fb740) func_001FB740 __attribute__((alias("FUN_001fb740")));
