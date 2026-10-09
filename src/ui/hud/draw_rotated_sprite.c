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

extern void FUN_001f9a10(void *, void *, void *);
extern void FUN_001f9a28(void *, void *, void *);
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
    u64 *q;

    pos.x = x;
    pos.y = y;
    sz.x = cy * fast_sin(ang);
    sz.y = cy * fast_cos(ang);
    off.x = -cx * fast_cos(ang);
    off.y = cx * fast_sin(ang);
    FUN_001f9a10(&p0, &pos, &sz);
    FUN_001f9a28(&p0, &p0, &off);
    FUN_001f9a10(&p1, &pos, &sz);
    FUN_001f9a10(&p1, &p1, &off);
    FUN_001f9a28(&p2, &pos, &sz);
    FUN_001f9a28(&p2, &p2, &off);
    FUN_001f9a28(&p3, &pos, &sz);
    FUN_001f9a10(&p3, &p3, &off);
    render_packet_cursor.tag->tag = 0x10000007;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000007;
    tag = render_packet_cursor.tag;
    q = (u64 *)(tag + 1);
    render_packet_cursor.tag = tag + 1;
    q[0] = 0xB400000000008001;
    q[1] = 0x53535353106;
    q[2] = buf;
    q[3] = 0x154;
    q[4] = 0x807F7F7F;
    q[5] = w << 4;
    q[6] = (((s32)p0.x + screen_extent.left) - 8) | ((u64)(((s32)p0.y + screen_extent.top) - 8) << 16) |
           ((u64)hud_state.z << 32);
    q[7] = (h << 20) + (w << 4);
    q[8] = (((s32)p1.x + screen_extent.left) - 8) | ((u64)(((s32)p1.y + screen_extent.top) - 8) << 16) |
           ((u64)hud_state.z << 32);
    q[9] = 0;
    q[10] = (((s32)p2.x + screen_extent.left) - 8) | ((u64)(((s32)p2.y + screen_extent.top) - 8) << 16) |
            ((u64)hud_state.z << 32);
    q[11] = h << 20;
    q[12] = (((s32)p3.x + screen_extent.left) - 8) | ((u64)(((s32)p3.y + screen_extent.top) - 8) << 16) |
            ((u64)hud_state.z << 32);
    q[13] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x70);
}

extern __typeof__(draw_rotated_sprite) func_00200600 __attribute__((alias("FUN_00200600")));
