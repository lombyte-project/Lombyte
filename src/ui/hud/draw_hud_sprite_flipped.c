#include "types.h"
#include "rnc/rendering/screen.h"
#include "rnc/ui/hud/hud_state.h"

#include "rnc/rendering/dma_tag.h"

extern u64 get_frame_texture(s32) __asm__("func_001FFA10");

void draw_hud_sprite_flipped(s32 id, s32 x, s32 y, s32 w, s32 h, s32 alpha) __asm__("FUN_001ffe18");

void draw_hud_sprite_flipped(s32 id, s32 x, s32 y, s32 w, s32 h, s32 alpha) {
    struct HudTexPage *t;
    struct DmaTag *tag;
    u64 *q;
    s32 tw;
    s32 th;

    t = &hud_state.image_pages[hud_state.frame_refs[id].image_index];
    tw = 1 << t->width_log2;
    th = 1 << t->height_log2;
    render_packet_cursor.tag->tag = 0x10000007;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000007;
    tag = render_packet_cursor.tag;
    q = (u64 *)(tag + 1);
    render_packet_cursor.tag = tag + 1;
    q[0] = 0xB400000000008001;
    q[1] = 0x53535353106;
    q[2] = get_frame_texture(id);
    q[3] = 0x154;
    q[4] = ((u64)alpha << 24) | 0x7F7F7F;
    q[5] = tw << 4;
    q[6] = (((x << 4) + D_0013E500.left) - 8) | ((u64)((((y + h) << 4) + D_0013E500.top) - 8) << 16) |
           ((u64)hud_state.z << 32);
    q[7] = (th << 20) + (tw << 4);
    q[8] = (((x << 4) + D_0013E500.left) - 8) | ((u64)(((y << 4) + D_0013E500.top) - 8) << 16) |
           ((u64)hud_state.z << 32);
    q[9] = 0;
    q[10] = ((((x + w) << 4) + D_0013E500.left) - 8) |
            ((u64)((((y + h) << 4) + D_0013E500.top) - 8) << 16) | ((u64)hud_state.z << 32);
    q[11] = th << 20;
    q[12] = ((((x + w) << 4) + D_0013E500.left) - 8) | ((u64)(((y << 4) + D_0013E500.top) - 8) << 16) |
            ((u64)hud_state.z << 32);
    q[13] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x70);
}
