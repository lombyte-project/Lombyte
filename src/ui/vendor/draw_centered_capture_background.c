#include "types.h"

#include "rnc/rendering/dma_tag.h"

extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

/* Appends a GIF packet to render_packet_cursor: a fixed 10-quad header (the last
   quads carry n = w / 32), then n pairs of sprite corner registers stepping
   0x200 (in 1/16 pixels) per column across a w x h area centred on 0x8000.
   c is volatile so the two pointer increments per pair stay separate, and
   zero / step are shared constants as in retail. */
void draw_centered_capture_background(s32 w, s32 h) __asm__("FUN_001fb740");

void draw_centered_capture_background(s32 width_pixels, s32 height_pixels) {
    struct DmaTag *tag;
    u64           *packet_header;
    volatile u64  *sprite_corner_cursor;
    s32            column_count;
    s32            i;
    s32            zero;
    s32            step;
    s32            x;
    s32            y;
    u64            lo;
    u64            hi;

    column_count = width_pixels / 32;
    vu1_add_g_sregister(0x42, 0x800000004AULL);
    render_packet_cursor.tag->tag = (column_count + 5) | 0x10000000;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = (column_count + 5) | 0x50000000;
    tag = render_packet_cursor.tag;
    packet_header = (u64 *)(tag + 1);
    render_packet_cursor.tag = tag + 1;
    zero = 0;
    packet_header[zero] = 0x1000000000000001;
    packet_header[1] = 0xE;
    packet_header[2] = 0x32003;
    packet_header[3] = 0x47;
    packet_header[4] = 0x2400000000000001;
    packet_header[5] = 0x10;
    packet_header[6] = 0x146;
    packet_header[7] = 0x80008080;
    packet_header[8] = (column_count | 0x8000) | 0x2400000000000000;
    packet_header[9] = 0x44;
    i = zero;
    if (column_count > zero) {
        lo = (u64)(0x8000 - height_pixels * 8) << 16;
        x = 0x8000 + -(width_pixels * 8);
        sprite_corner_cursor = (volatile u64 *)((u8 *)tag + 0x60);
        y = 0x8200 + -(width_pixels * 8);
        hi = (u64)(height_pixels * 8 + 0x7FF0) << 16;
        do {
            step = 0x200;
            *sprite_corner_cursor++ = x | lo;
            *sprite_corner_cursor++ = y | hi;
            y += step;
            x += step;
            i++;
        } while (i < column_count);
    }
    render_packet_cursor.tag = render_packet_cursor.tag + (column_count + 5);
}

extern __typeof__(draw_centered_capture_background) func_001FB740
    __attribute__((alias("FUN_001fb740")));
