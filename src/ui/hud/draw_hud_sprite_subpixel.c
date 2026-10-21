#include "types.h"
#include "rnc/ui/hud/hud_state.h"
#include "rnc/rendering/dma_tag.h"
#include "rnc/rendering/screen.h"
extern u64 get_frame_texture(s32) __asm__("func_001FFA10");

void draw_hud_sprite_subpixel(s32 id, s32 x, s32 y, s32 w, s32 h,
                              s32 alpha) __asm__("FUN_00200080");

void draw_hud_sprite_subpixel(s32 id, s32 x, s32 y, s32 width, s32 height, s32 alpha) {
    struct HudState *hud = &hud_state;
    struct HudTexPage *image_page;
    s32 tw;
    s32 th;
    u64 *sprite_packet;

    image_page = &hud->image_pages[hud->frame_refs[id].image_index];
    tw = 1 << image_page->width_log2;
    th = 1 << image_page->height_log2;
    render_packet_cursor.tag->tag = 0x10000005;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000005;
    sprite_packet = (u64 *)++render_packet_cursor.tag;
    sprite_packet[0] = (u64)0xE800 << 47 | 0x8001;
    sprite_packet[1] = 0x5353106;
    sprite_packet[2] = get_frame_texture(id);
    sprite_packet[3] = 0x156;
    sprite_packet[4] = (u64)alpha << 24 | 0x7F7F7F;
    sprite_packet[5] = 0;
    sprite_packet[6] = (x + screen_extent.left - 8) | ((u64)(y + screen_extent.top - 8) << 16) | ((u64)hud->z << 32);
    sprite_packet[7] = (th << 20) + (tw << 4);
    sprite_packet[8] = (x + width + screen_extent.left - 8) | ((u64)(y + height + screen_extent.top - 8) << 16) |
           ((u64)hud->z << 32);
    sprite_packet[9] = 0;
    render_packet_cursor.tag += 5;
}

extern __typeof__(draw_hud_sprite_subpixel) func_00200080 __attribute__((alias("FUN_00200080")));
