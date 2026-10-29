#include "types.h"
#include "rnc/rendering/screen.h"
#include "rnc/ui/hud/hud_state.h"

#include "rnc/rendering/dma_tag.h"

extern u64 get_frame_texture(s32) __asm__("func_001FFA10");

void draw_hud_sprite_flipped(s32 id, s32 x, s32 y, s32 w, s32 h, s32 alpha) __asm__("FUN_001ffe18");

void draw_hud_sprite_flipped(s32 id, s32 x, s32 y, s32 sprite_width, s32 sprite_height, s32 alpha) {
    struct HudTexPage *image_page;
    struct DmaTag *tag;
    u64 *packet_words;
    s32 tw;
    s32 th;

    image_page = &hud_state.image_pages[hud_state.frame_refs[id].image_index];
    tw = 1 << image_page->width_log2;
    th = 1 << image_page->height_log2;
    render_packet_cursor.tag->tag = 0x10000007;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = 0x50000007;
    tag = render_packet_cursor.tag;
    packet_words = (u64 *)(tag + 1);
    render_packet_cursor.tag = tag + 1;
    packet_words[0] = 0xB400000000008001;
    packet_words[1] = 0x53535353106;
    packet_words[2] = get_frame_texture(id);
    packet_words[3] = 0x154;
    packet_words[4] = ((u64)alpha << 24) | 0x7F7F7F;
    packet_words[5] = tw << 4;
    packet_words[6] = (((x << 4) + screen_extent.left) - 8) | ((u64)((((y + sprite_height) << 4) + screen_extent.top) - 8) << 16) |
           ((u64)hud_state.z << 32);
    packet_words[7] = (th << 20) + (tw << 4);
    packet_words[8] = (((x << 4) + screen_extent.left) - 8) | ((u64)(((y << 4) + screen_extent.top) - 8) << 16) |
           ((u64)hud_state.z << 32);
    packet_words[9] = 0;
    packet_words[10] = ((((x + sprite_width) << 4) + screen_extent.left) - 8) |
            ((u64)((((y + sprite_height) << 4) + screen_extent.top) - 8) << 16) | ((u64)hud_state.z << 32);
    packet_words[11] = th << 20;
    packet_words[12] = ((((x + sprite_width) << 4) + screen_extent.left) - 8) | ((u64)(((y << 4) + screen_extent.top) - 8) << 16) |
            ((u64)hud_state.z << 32);
    packet_words[13] = 0;
    render_packet_cursor.tag = (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x70);
}
