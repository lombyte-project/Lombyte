#include "types.h"
#include "rnc/rendering/fs_aa_buffer.h"

struct ClearStripPacket {
    s32 dma_control;
    s32 address;
    s32 vif_command;
    s32 gif_control;
    s64 gif_tag;
    s64 gif_registers;
    s64 test_value;
    s64 test_register;
    s64 primitive_tag;
    s64 primitive_registers;
    s64 primitive;
    s64 color;
    s64 vertex_tag;
    s64 vertex_registers;
};

extern struct ClearStripPacket *render_packet_cursor[1] __asm__("D_00160F00");

void append_fullscreen_clear_strips(s64 color) __asm__("FUN_00227378");

void append_fullscreen_clear_strips(s64 color) {
    struct ClearStripPacket *base;
    struct ClearStripPacket *packet_cursor;
    s64 *commands;
    volatile s64 *vertices; /* GIF packet words, written in order */
    struct FsAaBuf *dimensions;
    s32 strip_count;
    s32 display_height;
    s32 display_width;
    s32 strip_index;
    s32 left_x;
    s32 top_y;
    s32 right_x;
    s32 bottom_y;
    s32 negative_half_width;
    s32 dividend;
    s64 packed_top_y;
    s64 packed_bottom_y;

    dimensions = &fs_aa_buffer;
    display_width = dimensions->display_width;
    display_height = dimensions->display_height;
    /* Retail truncates signed display width toward zero before packing strips. */
    dividend = (display_width > -1) ? display_width : (display_width + 0x1F);
    strip_count = dividend >> 5;
    strip_index = 0;
    render_packet_cursor[0]->dma_control = (strip_count + 5) | 0x10000000;
    render_packet_cursor[0]->address = 0;
    render_packet_cursor[0]->vif_command = 0;
    render_packet_cursor[0]->gif_control = (strip_count + 5) | 0x50000000;
    base = render_packet_cursor[0];
    packet_cursor = (struct ClearStripPacket *)((u8 *)base + 0x10);
    render_packet_cursor[0] = packet_cursor;
    base->gif_tag = 0x1000000000000001;
    commands = (s64 *)packet_cursor;
    commands[1] = 0xE;
    commands[2] = 0x3D801;
    commands[3] = 0x47;
    commands[4] = 0x2400000000000001;
    commands[5] = 0x10;
    commands[6] = 0x146;
    commands[7] = color;
    commands[8] = (s64)(strip_count | 0x8000) | 0x2400000000000000;
    commands[9] = 0x44;
    if (strip_index < strip_count) {
        bottom_y = display_height * 8 + 0x7FF0;
        top_y = 0x8000 - display_height * 8;
        negative_half_width = -(display_width * 8);
        packed_top_y = (s64)top_y << 16;
        left_x = negative_half_width + 0x8000;
        right_x = negative_half_width + 0x8200;
        packed_bottom_y = (s64)bottom_y << 16;
        vertices = (volatile s64 *)((u8 *)base + 0x60);
        do {
            *vertices++ = (s64)left_x | packed_top_y;
            *vertices++ = (s64)right_x | packed_bottom_y;
            strip_index++;
            right_x += 0x200;
            left_x += 0x200;
        } while (strip_index < strip_count);
    }
    packet_cursor = render_packet_cursor[0] =
        (struct ClearStripPacket *)((u8 *)render_packet_cursor[0] + (strip_count * 0x10 + 0x50));
    packet_cursor->dma_control = 0x10000000;
    render_packet_cursor[0]->address = 0;
    render_packet_cursor[0]->vif_command = 0x13000000;
    render_packet_cursor[0]->gif_control = 0;
    render_packet_cursor[0] = (struct ClearStripPacket *)((u8 *)render_packet_cursor[0] + 0x10);
}
extern __typeof__(append_fullscreen_clear_strips) func_00227378
    __attribute__((alias("FUN_00227378")));

