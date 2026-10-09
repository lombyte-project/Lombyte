#include "types.h"
#include "rnc/ui/hud/hud_state.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/screen.h"
extern u64 get_frame_texture(s32) __asm__("func_001FFA10");

void draw_hud_sprite_rect(s32 tex, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0, s32 u1, s32 v1,
                          s32 alpha) __asm__("FUN_00200958");

void draw_hud_sprite_rect(s32 tex, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0, s32 u1, s32 v1,
                          s32 alpha) {
    u64 *q;

    render_packet_cursor.tag->tag = 0x10000005;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000005;
    q = (u64 *)++render_packet_cursor.tag;
    q[0] = (u64)0xE800 << 47 | 0x8001;
    q[1] = 0x5353106;
    q[2] = get_frame_texture(tex);
    q[3] = 0x156;
    q[4] = (u64)alpha << 24 | 0x7F7F7F;
    q[5] = u0 | ((u64)v0 << 16);
    q[6] = (x0 + screen_extent.left - 8) | ((u64)(y0 + screen_extent.top - 8) << 16) |
           ((u64)hud_state.z << 32);
    q[7] = u1 | ((u64)v1 << 16);
    q[8] = (x1 + screen_extent.left - 8) | ((u64)(y1 + screen_extent.top - 8) << 16) |
           ((u64)hud_state.z << 32);
    q[9] = 0;
    render_packet_cursor.tag += 5;
}

extern __typeof__(draw_hud_sprite_rect) func_00200958 __attribute__((alias("FUN_00200958")));
