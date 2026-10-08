#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002271d0/FUN_002271d0.s", FUN_002271d0);
#else
#include "types.h"
#include "rnc/rendering/fs_aa_buffer.h"

#include "rnc/rendering/dma_tag.h"
extern void vu1_add_g_sregister(s32, s32) __asm__("func_00233980");
void append_fullscreen_setup_strips(void) __asm__("FUN_002271d0");

void append_fullscreen_setup_strips(void) {
    struct DmaTag *tag;
    u64 *packet_words;
    u64 *strip_words;
    s32 display_width;
    s32 display_height;
    s32 strip_count;
    s32 strip_index;
    s32 negative_half_width;
    s32 left_x;
    s32 right_x;
    u64 top_y;
    u64 bottom_y;

    display_width = fs_aa_buffer.display_width;
    display_height = fs_aa_buffer.display_height;
    /* Retail divides the signed display width, truncating toward zero. */
    strip_count = display_width / 32;
    vu1_add_g_sregister(0x42, 0x64);
    render_packet_cursor.tag->tag = (strip_count + 5) | 0x10000000;
    render_packet_cursor.tag->addr = 0;
    render_packet_cursor.tag->vif0 = 0;
    render_packet_cursor.tag->vif1 = (strip_count + 5) | 0x50000000;
    tag = render_packet_cursor.tag;
    packet_words = (u64 *)(tag + 1);
    render_packet_cursor.tag = tag + 1;
    packet_words[0] = 0x1000000000000001;
    packet_words[1] = 0xE;
    packet_words[2] = 0x31001;
    packet_words[3] = 0x47;
    packet_words[4] = 0x2400000000008001;
    packet_words[5] = 0x10;
    packet_words[6] = 0x146;
    packet_words[7] = 0x7F808080;
    packet_words[8] = 0x2400000000000000 | (strip_count | 0x8000);
    packet_words[9] = 0x44;
    strip_index = 0;
    /* Each strip writes two packed 64-bit XY coordinates. */
    if (strip_count > 0) {
        negative_half_width = -(display_width * 8);
        top_y = (u64)(0x8000 - display_height * 8) << 16;
        left_x = negative_half_width + 0x8000;
    strip_words = (u64 *)(tag + 6);
        right_x = negative_half_width + 0x8200;
        bottom_y = (u64)(display_height * 8 + 0x7FF0) << 16;
    loop:
        *strip_words++ = left_x | top_y;
        strip_index++;
        *strip_words++ = right_x | bottom_y;
        right_x += 0x200;
        left_x += 0x200;
        if (strip_index < strip_count) {
            goto loop;
        }
    }
    /* The cursor is already past the DMA tag at this point. */
    do {
        render_packet_cursor.tag =
            (struct DmaTag *)((u8 *)render_packet_cursor.tag + 0x50 + strip_count * 0x10);
    } while (0);
}

extern __typeof__(append_fullscreen_setup_strips) func_002271D0
    __attribute__((alias("FUN_002271d0")));

#endif /* NON_MATCHING */
