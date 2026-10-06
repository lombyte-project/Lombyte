#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f5ab0/FUN_001f5ab0.s", FUN_001f5ab0);
#else
#include "types.h"

#include "rnc/rendering/dma_tag.h"

struct Vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

struct ScreenOfs {
    u8 pad0[0x10];
    s32 x;
    s32 y;
};

extern struct TagPtr render_packet_cursor __asm__("D_00160F00");
extern struct ScreenOfs screen_offsets __asm__("D_0013E500");
extern void fast_vec_add(void *, void *, void *) __asm__("func_001F9A10");
extern void fast_vec_sub(void *, void *, void *) __asm__("func_001F9A28");
extern void fast_vec_scale(void *, void *, f32) __asm__("func_001F9A68");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern s32 convert_float_to_integer(f32) __asm__("func_001FA6D0");

void append_rotated_sprite_quad(s32 texture_width, s32 texture_height, s64 texture_tex0,
                                s64 z_and_fog, s32 color, u8 flip_u, u8 flip_v, f32 center_x,
                                f32 center_y, f32 quad_width, f32 quad_height, f32 angle,
                                f32 pivot_u, f32 pivot_v) __asm__("FUN_001f5ab0");

void append_rotated_sprite_quad(s32 texture_width, s32 texture_height, s64 texture_tex0,
                                s64 z_and_fog, s32 color, u8 flip_u, u8 flip_v, f32 center_x,
                                f32 center_y, f32 quad_width, f32 quad_height, f32 angle,
                                f32 pivot_u, f32 pivot_v) {
    struct Vec4 vertical_edge;
    struct Vec4 horizontal_edge;
    struct Vec4 center;
    struct Vec4 temporary;
    struct Vec4 top_left;
    struct Vec4 top_right;
    struct Vec4 bottom_left;
    struct Vec4 bottom_right;
    struct DmaTag *tag;
    u64 *packet_words;
    s32 texture_left;
    s32 texture_right;
    s32 texture_top;
    s32 texture_bottom;
    f32 inverse_pivot_u;
    f32 inverse_pivot_v;

    if (flip_u != 0) {
        texture_left = ((u32)texture_width << 4);
        texture_right = 0x10;
    } else {
        texture_right = ((u32)texture_width << 4);
        texture_left = 0x10;
    }
    if (flip_v != 0) {
        texture_top = ((u32)texture_height << 20);
        texture_bottom = 0x100000;
    } else {
        texture_bottom = ((u32)texture_height << 20);
        texture_top = 0x100000;
    }
    center.x = center_x;
    center.y = center_y;
    vertical_edge.x = quad_height * fast_sin(angle);
    inverse_pivot_v = 1.0f - pivot_v;
    vertical_edge.y = quad_height * fast_cos(angle);
    horizontal_edge.x = quad_width * fast_cos(angle);
    horizontal_edge.y = -quad_width * fast_sin(angle);
    inverse_pivot_u = 1.0f - pivot_u;
    fast_vec_scale(&temporary, &vertical_edge, inverse_pivot_v);
    fast_vec_add(&top_left, &center, &temporary);
    fast_vec_scale(&temporary, &horizontal_edge, inverse_pivot_u);
    fast_vec_sub(&top_left, &top_left, &temporary);
    fast_vec_scale(&temporary, &vertical_edge, inverse_pivot_v);
    fast_vec_add(&top_right, &center, &temporary);
    fast_vec_scale(&temporary, &horizontal_edge, pivot_u);
    fast_vec_add(&top_right, &top_right, &temporary);
    fast_vec_scale(&temporary, &vertical_edge, pivot_v);
    fast_vec_sub(&bottom_left, &center, &temporary);
    fast_vec_scale(&temporary, &horizontal_edge, inverse_pivot_u);
    fast_vec_sub(&bottom_left, &bottom_left, &temporary);
    fast_vec_scale(&temporary, &vertical_edge, pivot_v);
    fast_vec_sub(&bottom_right, &center, &temporary);
    fast_vec_scale(&temporary, &horizontal_edge, pivot_u);
    fast_vec_add(&bottom_right, &bottom_right, &temporary);
    render_packet_cursor.p->tag = 0x10000007;
    render_packet_cursor.p->addr = 0;
    render_packet_cursor.p->vif0 = 0;
    render_packet_cursor.p->vif1 = 0x50000007;
    tag = render_packet_cursor.p;
    packet_words = (u64 *)(tag + 1);
    render_packet_cursor.p = tag + 1;
    packet_words[0] = 0xB400000000008001;
    packet_words[1] = 0x53535353106;
    packet_words[2] = texture_tex0;
    packet_words[3] = 0x154;
    packet_words[4] = color;
    packet_words[5] = texture_left | texture_top;
    packet_words[6] =
        (convert_float_to_integer(top_left.x * 16.0f) + screen_offsets.x - 8) |
        ((u64)(convert_float_to_integer(top_left.y * 16.0f) + screen_offsets.y - 8) << 16) |
        ((u64)z_and_fog << 32);
    packet_words[7] = texture_right | texture_top;
    packet_words[8] =
        (convert_float_to_integer(top_right.x * 16.0f) + screen_offsets.x - 8) |
        ((u64)(convert_float_to_integer(top_right.y * 16.0f) + screen_offsets.y - 8) << 16) |
        ((u64)z_and_fog << 32);
    packet_words[9] = texture_left | texture_bottom;
    packet_words[10] =
        (convert_float_to_integer(bottom_left.x * 16.0f) + screen_offsets.x - 8) |
        ((u64)(convert_float_to_integer(bottom_left.y * 16.0f) + screen_offsets.y - 8) << 16) |
        ((u64)z_and_fog << 32);
    packet_words[11] = texture_right | texture_bottom;
    packet_words[12] =
        (convert_float_to_integer(bottom_right.x * 16.0f) + screen_offsets.x - 8) |
        ((u64)(convert_float_to_integer(bottom_right.y * 16.0f) + screen_offsets.y - 8) << 16) |
        ((u64)z_and_fog << 32);
    packet_words[13] = 0;
    render_packet_cursor.p = (struct DmaTag *)((u8 *)render_packet_cursor.p + 0x70);
}

extern __typeof__(append_rotated_sprite_quad) func_001F5AB0 __attribute__((alias("FUN_001f5ab0")));

#endif /* NON_MATCHING */
