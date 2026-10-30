#include "types.h"
#include "rnc/rendering/screen.h"
#include "rnc/ui/hud/hud_state.h"

#include "rnc/rendering/dma_tag.h"
#include "rnc/math/fast_math.h"

struct Vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");

void draw_rotated_sprite(s32 w, s32 h, s64 buf, f32 x, f32 y, f32 cx, f32 cy, f32 ang) __asm__("FUN_00200600");

void draw_rotated_sprite(s32 texture_width,
                         s32 texture_height,
                         s64 texture_register_value,
                         f32 x,
                         f32 y,
                         f32 cx,
                         f32 cy,

                         f32 rotation_angle) {
    struct Vec4    sz;
    struct Vec4    rotated_half_width_offset;
    struct Vec4    sprite_center;
    struct Vec4    uv_u_extent_vertex;
    struct Vec4    uv_extent_vertex;
    struct Vec4    uv_origin_vertex;
    struct Vec4    uv_v_extent_vertex;
    struct DmaTag *tag;
    u64           *packet_payload;

    sprite_center.x = x;
    sprite_center.y = y;
    sz.x = cy * fast_sin(rotation_angle);
    sz.y = cy * fast_cos(rotation_angle);
    rotated_half_width_offset.x = -cx * fast_cos(rotation_angle);
    rotated_half_width_offset.y = cx * fast_sin(rotation_angle);
    fast_vec_add(&uv_u_extent_vertex, &sprite_center, &sz);
    fast_vec_sub(&uv_u_extent_vertex, &uv_u_extent_vertex, &rotated_half_width_offset);
    fast_vec_add(&uv_extent_vertex, &sprite_center, &sz);
    fast_vec_add(&uv_extent_vertex, &uv_extent_vertex, &rotated_half_width_offset);
    fast_vec_sub(&uv_origin_vertex, &sprite_center, &sz);
    fast_vec_sub(&uv_origin_vertex, &uv_origin_vertex, &rotated_half_width_offset);
    fast_vec_sub(&uv_v_extent_vertex, &sprite_center, &sz);
    fast_vec_add(&uv_v_extent_vertex, &uv_v_extent_vertex, &rotated_half_width_offset);
    render_packet_cursor.tag->tag = 0x10000007;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000007;
    tag = render_packet_cursor.tag;
    packet_payload = (u64 *)(tag + 1);
    render_packet_cursor.tag = tag + 1;
    packet_payload[0] = 0xB400000000008001;
    packet_payload[1] = 0x53535353106;
    packet_payload[2] = texture_register_value;
    packet_payload[3] = 0x154;
    packet_payload[4] = 0x807F7F7F;
    packet_payload[5] = texture_width << 4;
    packet_payload[6] = (((s32)uv_u_extent_vertex.x + screen_extent.left) - 8) |
                        ((u64)(((s32)uv_u_extent_vertex.y + screen_extent.top) - 8) << 16) |
                        ((u64)hud_state.z << 32);
    packet_payload[7] = (texture_height << 20) + (texture_width << 4);
    packet_payload[8] = (((s32)uv_extent_vertex.x + screen_extent.left) - 8) |
                        ((u64)(((s32)uv_extent_vertex.y + screen_extent.top) - 8) << 16) |
                        ((u64)hud_state.z << 32);
    packet_payload[9] = 0;
    packet_payload[10] = (((s32)uv_origin_vertex.x + screen_extent.left) - 8) |
                         ((u64)(((s32)uv_origin_vertex.y + screen_extent.top) - 8) << 16) |
                         ((u64)hud_state.z << 32);
    packet_payload[11] = texture_height << 20;
    packet_payload[12] = (((s32)uv_v_extent_vertex.x + screen_extent.left) - 8) |
                         ((u64)(((s32)uv_v_extent_vertex.y + screen_extent.top) - 8) << 16) |
                         ((u64)hud_state.z << 32);
    packet_payload[13] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x70);
}

extern __typeof__(draw_rotated_sprite) func_00200600 __attribute__((alias("FUN_00200600")));
