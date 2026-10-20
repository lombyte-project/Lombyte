#include "types.h"
#include "rnc/rendering/screen.h"
#include "rnc/ui/hud/hud_state.h"

#include "rnc/rendering/dma_tag.h"

extern u64 get_frame_texture(s32) __asm__("func_001FFA10");

void draw_hud_sprite(s32 id, s32 x, s32 y, s32 w, s32 h, s32 alpha) __asm__("FUN_001ffc30");

void draw_hud_sprite(s32 id, s32 x, s32 y, s32 w, s32 h, s32 alpha) {
    struct HudTexPage *image_page;
    struct DmaTag *tag;
    u64 *sprite_packet_words;
    s32 tw;
    s32 th;

    image_page = &hud_state.image_pages[hud_state.frame_refs[id].image_index];
    tw = 1 << image_page->width_log2;
    th = 1 << image_page->height_log2;
    render_packet_cursor.tag->tag = 0x10000005;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000005;
    tag = render_packet_cursor.tag;
    sprite_packet_words = (u64 *)(tag + 1);
    render_packet_cursor.tag = tag + 1;
    sprite_packet_words[0] = 0x7400000000008001;
    sprite_packet_words[1] = 0x5353106;
    sprite_packet_words[2] = get_frame_texture(id);
    sprite_packet_words[3] = 0x156;
    sprite_packet_words[4] = ((u64)alpha << 24) | 0x7F7F7F;
    sprite_packet_words[5] = 0;
    sprite_packet_words[6] = (((x << 4) + screen_extent.left) - 8) | ((u64)(((y << 4) + screen_extent.top) - 8) << 16) |
           ((u64)hud_state.z << 32);
    sprite_packet_words[7] = (th << 20) + (tw << 4);
    sprite_packet_words[8] = ((((x + w) << 4) + screen_extent.left) - 8) |
           ((u64)((((y + h) << 4) + screen_extent.top) - 8) << 16) | ((u64)hud_state.z << 32);
    sprite_packet_words[9] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x50);
}

extern __typeof__(draw_hud_sprite) func_001FFC30 __attribute__((alias("FUN_001ffc30")));
