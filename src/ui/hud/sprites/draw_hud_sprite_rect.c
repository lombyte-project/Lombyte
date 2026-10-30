#include "types.h"
#include "rnc/ui/hud/hud_state.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/screen.h"

extern u64 get_frame_texture(s32) __asm__("func_001FFA10");

void draw_hud_sprite_rect(s32 tex,
                          s32 x0,
                          s32 y0,
                          s32 x1,
                          s32 y1,
                          s32 u0,
                          s32 v0,
                          s32 u1,
                          s32 v1,
                          s32 alpha) __asm__("FUN_00200958");

void draw_hud_sprite_rect(s32 frame_id,
                          s32 x0,
                          s32 y0,
                          s32 x1,
                          s32 y1,
                          s32 texture_u0,
                          s32 texture_v0,
                          s32 texture_u1,
                          s32 texture_v1,

                          s32 alpha) {
    u64 *packet_words;

    render_packet_cursor.tag->tag = 0x10000005;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000005;
    packet_words = (u64 *)++render_packet_cursor.tag;
    packet_words[0] = (u64)0xE800 << 47 | 0x8001;
    packet_words[1] = 0x5353106;
    packet_words[2] = get_frame_texture(frame_id);
    packet_words[3] = 0x156;
    packet_words[4] = (u64)alpha << 24 | 0x7F7F7F;
    packet_words[5] = texture_u0 | ((u64)texture_v0 << 16);
    packet_words[6] = (x0 + screen_extent.left - 8) | ((u64)(y0 + screen_extent.top - 8) << 16) |
                      ((u64)hud_state.z << 32);
    packet_words[7] = texture_u1 | ((u64)texture_v1 << 16);
    packet_words[8] = (x1 + screen_extent.left - 8) | ((u64)(y1 + screen_extent.top - 8) << 16) |
                      ((u64)hud_state.z << 32);
    packet_words[9] = 0;
    render_packet_cursor.tag += 5;
}

extern __typeof__(draw_hud_sprite_rect) func_00200958 __attribute__((alias("FUN_00200958")));
