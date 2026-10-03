#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f55d8/FUN_001f55d8.s", FUN_001f55d8);
#else
#include "types.h"
#include "qcopy.h"

struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct DmaTag *p; };
struct ScreenOfs { u8 pad0[0x10]; s32 x; s32 y; };

extern struct TagPtr render_packet_cursor __asm__("D_00160F00");
extern struct ScreenOfs screen_offsets __asm__("D_0013E500");
extern char textured_quad_header[] __asm__("D_00160840");
extern s32 convert_float_to_integer(f32) __asm__("func_001FA6D0");

void append_subpixel_textured_screen_quad(s32 texture_u, s32 texture_v, s32 texture_width, s32 texture_height, s64 color, s64 texture_tex0, f32 screen_x, f32 screen_y, f32 screen_width, f32 screen_height) __asm__("FUN_001f55d8");

void append_subpixel_textured_screen_quad(s32 texture_u, s32 texture_v, s32 texture_width, s32 texture_height, s64 color, s64 texture_tex0, f32 screen_x, f32 screen_y, f32 screen_width, f32 screen_height)
{
    struct DmaTag *tag;
    u64 *packet_words;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 texture_right;
    s32 texture_left;
    s32 texture_bottom;

    left = convert_float_to_integer(screen_x * 16.0f) + screen_offsets.x - 8;
    right = convert_float_to_integer((screen_width + screen_x) * 16.0f) + screen_offsets.x - 8;
    top = convert_float_to_integer(screen_y * 16.0f) + screen_offsets.y - 8;
    bottom = convert_float_to_integer((screen_y + screen_height) * 16.0f) + screen_offsets.y - 8;
    texture_right = (texture_u + texture_width) << 4;
    texture_left = texture_u << 4;
    texture_bottom = texture_v + texture_height;
    render_packet_cursor.p->w0 = 0x10000007;
    render_packet_cursor.p->addr = 0;
    render_packet_cursor.p->w2 = 0;
    render_packet_cursor.p->w3 = 0x50000007;
    tag = render_packet_cursor.p;
    render_packet_cursor.p = tag + 1;
    qcopy(tag + 1, textured_quad_header);
    packet_words = (u64 *)(tag + 2);
    render_packet_cursor.p = tag + 2;
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
    render_packet_cursor.p = (struct DmaTag *)((u8 *)render_packet_cursor.p + 0x60);
}

extern __typeof__(append_subpixel_textured_screen_quad) func_001F55D8 __attribute__((alias("FUN_001f55d8")));

#endif /* NON_MATCHING */
