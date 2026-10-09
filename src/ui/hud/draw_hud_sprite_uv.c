#include "types.h"
#include "rnc/ui/hud/hud_state.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/screen.h"
extern u64 get_frame_texture(s32) __asm__("func_001FFA10");

void draw_hud_sprite_uv(s32 id, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v,
                        s32 alpha) __asm__("FUN_00200258");

void draw_hud_sprite_uv(s32 id, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 alpha) {
    struct HudState *hud = &hud_state;
    struct HudTexPage *t;
    s32 tw;
    s32 th;
    u64 *q;

    t = &hud->image_pages[hud->frame_refs[id].image_index];
    tw = 1 << (t->width_log2 + 4);
    th = 1 << (t->height_log2 + 4);
    render_packet_cursor.tag->tag = 0x10000005;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000005;
    q = (u64 *)++render_packet_cursor.tag;
    q[0] = (u64)0xE800 << 47 | 0x8001;
    q[1] = 0x5353106;
    q[2] = get_frame_texture(id);
    q[3] = 0x156;
    q[4] = (u64)alpha << 24 | 0x7F7F7F;
    q[5] = u | ((u64)v << 16);
    q[6] = (x + screen_extent.left - 8) | ((u64)(y + screen_extent.top - 8) << 16) | ((u64)hud->z << 32);
    q[7] = (u + tw) | ((u64)(v + th) << 16);
    q[8] = (x + w + screen_extent.left - 8) | ((u64)(y + h + screen_extent.top - 8) << 16) |
           ((u64)hud->z << 32);
    q[9] = 0;
    render_packet_cursor.tag += 5;
}

extern __typeof__(draw_hud_sprite_uv) func_00200258 __attribute__((alias("FUN_00200258")));
