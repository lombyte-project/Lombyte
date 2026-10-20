#include "types.h"
#include "rnc/rendering/screen.h"
#include "rnc/ui/hud/hud_state.h"

#include "rnc/rendering/dma_tag.h"

struct Vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");

void draw_rotated_sprite(s32 w, s32 h, s64 buf, f32 x, f32 y, f32 cx, f32 cy,
                         f32 ang) __asm__("FUN_00200600");

void draw_rotated_sprite(s32 w, s32 h, s64 buf, f32 x, f32 y, f32 cx, f32 cy, f32 ang) {
    struct Vec4 sz;
    struct Vec4 off;
    struct Vec4 pos;
    struct Vec4 p0;
    struct Vec4 p1;
    struct Vec4 p2;
    struct Vec4 p3;
    struct DmaTag *tag;
    u64 *packet_payload;

    pos.x = x;
    pos.y = y;
    sz.x = cy * fast_sin(ang);
    sz.y = cy * fast_cos(ang);
    off.x = -cx * fast_cos(ang);
    off.y = cx * fast_sin(ang);
    add_vector_xyz(&p0, &pos, &sz);
    subtract_vector_xyz(&p0, &p0, &off);
    add_vector_xyz(&p1, &pos, &sz);
    add_vector_xyz(&p1, &p1, &off);
    subtract_vector_xyz(&p2, &pos, &sz);
    subtract_vector_xyz(&p2, &p2, &off);
    subtract_vector_xyz(&p3, &pos, &sz);
    add_vector_xyz(&p3, &p3, &off);
    render_packet_cursor.tag->tag = 0x10000007;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000007;
    tag = render_packet_cursor.tag;
    packet_payload = (u64 *)(tag + 1);
    render_packet_cursor.tag = tag + 1;
    packet_payload[0] = 0xB400000000008001;
    packet_payload[1] = 0x53535353106;
    packet_payload[2] = buf;
    packet_payload[3] = 0x154;
    packet_payload[4] = 0x807F7F7F;
    packet_payload[5] = w << 4;
    packet_payload[6] = (((s32)p0.x + screen_extent.left) - 8) | ((u64)(((s32)p0.y + screen_extent.top) - 8) << 16) |
           ((u64)hud_state.z << 32);
    packet_payload[7] = (h << 20) + (w << 4);
    packet_payload[8] = (((s32)p1.x + screen_extent.left) - 8) | ((u64)(((s32)p1.y + screen_extent.top) - 8) << 16) |
           ((u64)hud_state.z << 32);
    packet_payload[9] = 0;
    packet_payload[10] = (((s32)p2.x + screen_extent.left) - 8) | ((u64)(((s32)p2.y + screen_extent.top) - 8) << 16) |
            ((u64)hud_state.z << 32);
    packet_payload[11] = h << 20;
    packet_payload[12] = (((s32)p3.x + screen_extent.left) - 8) | ((u64)(((s32)p3.y + screen_extent.top) - 8) << 16) |
            ((u64)hud_state.z << 32);
    packet_payload[13] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x70);
}

extern __typeof__(draw_rotated_sprite) func_00200600 __attribute__((alias("FUN_00200600")));
