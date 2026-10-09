#include "types.h"
#include "asm.h"

#include "types.h"
#include "rnc/rendering/screen.h"
#include "qcopy.h"

#include "rnc/rendering/dma_tag.h"
extern char textured_quad_header[] __asm__("D_00160840");
extern s32 convert_float_to_integer(f32) __asm__("func_001FA6D0");

void append_subpixel_textured_screen_quad(f32 screen_x, f32 screen_y, f32 screen_width,
                                          f32 screen_height, s32 texture_u, s32 texture_v,
                                          s32 texture_width, s32 texture_height, s64 color,
                                          s64 texture_tex0) __asm__("FUN_001f55d8");

void append_subpixel_textured_screen_quad(f32 screen_x, f32 screen_y, f32 screen_width,
                                          f32 screen_height, s32 texture_u, s32 texture_v,
                                          s32 texture_width, s32 texture_height, s64 color,
                                          s64 texture_tex0) {
    struct DmaTag *tag;
    u64 *packet_words;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 texture_right;
    s32 texture_left;
    s32 texture_bottom;
    f32 screen_scale = 16.0f;

    left = convert_float_to_integer(screen_x * screen_scale) + screen_extent.left - 8;
    right =
        convert_float_to_integer((screen_x + screen_width) * screen_scale) + screen_extent.left - 8;
    top = convert_float_to_integer(screen_y * screen_scale) + screen_extent.top - 8;
    bottom =
        convert_float_to_integer((screen_y + screen_height) * screen_scale) + screen_extent.top - 8;
    texture_right = (texture_u + texture_width) << 4;
    texture_left = texture_u << 4;
    texture_bottom = texture_v + texture_height;
    render_packet_cursor.tag->tag = 0x10000007;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000007;
    tag = render_packet_cursor.tag;
    render_packet_cursor.tag = tag + 1;
    qcopy(tag + 1, textured_quad_header);
    packet_words = (u64 *)(tag + 2);
    render_packet_cursor.tag = tag + 2;
    packet_words[0] = texture_tex0;
    packet_words[1] = 0x154;
    packet_words[2] = color;
    packet_words[3] = (texture_v << 20) + texture_left;
    packet_words[4] = left | ((u64)top << 16) | 0xFFFFF000000000;
    packet_words[5] = (texture_v << 20) + texture_right;
    packet_words[6] = right | ((u64)top << 16) | 0xFFFFF000000000;
    packet_words[7] = (texture_bottom << 20) + texture_left;
    packet_words[8] = left | ((u64)bottom << 16) | 0xFFFFF000000000;
    packet_words[9] = (texture_bottom << 20) + texture_right;
    packet_words[10] = right | ((u64)bottom << 16) | 0xFFFFF000000000;
    packet_words[11] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x60);
}

extern __typeof__(append_subpixel_textured_screen_quad) func_001F55D8
    __attribute__((alias("FUN_001f55d8")));
