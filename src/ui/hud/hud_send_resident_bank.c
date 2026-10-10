#include "types.h"
#include "rnc/globals.h"
#include "rnc/ui/hud/hud_state.h"

extern void link_hud_bank(s32 bank, s32 base) __asm__("FUN_001fefc0");
extern void hud_send_texture(u32 data, s32 dbp, s32 psm, s32 wlog, s32 hlog,
                             s32 immediate) __asm__("FUN_00200b10");

void hud_send_resident_bank(s32 bank, s32 base, s32 immediate) __asm__("FUN_001ff128");

/* Uploads the image pages of one HUD bank to GS memory, from the depth buffer
   address up, and records each page's block offset. Bank 0's pages are
   linked first when the bank file is not linked yet. */
void hud_send_resident_bank(s32 bank, s32 base, s32 immediate) {
    struct HudTexCounts *counts;
    s32 address;
    s32 bank_offset;
    s32 first;
    s32 end;
    s32 i;
    s32 page;
    s32 width_log2;
    s32 height_log2;
    s32 size;

    if (hud_state.header.counts->loaded[bank] == 0) {
        link_hud_bank(0, base);
        address = depth_buffer_address;
    } else {
        address = depth_buffer_address;
    }
    counts = hud_state.header.counts;
    bank_offset = bank << 2;
    first = bank == 0 ? 0 : counts->ends[bank - 1];
    i = first;
    end = *(s32 *)((u8 *)hud_state.header.counts + bank_offset + 0x34); /* ends[bank] */
    for (; i < end; i++) {
        page = address >> 8;
        width_log2 = hud_state.image_pages[i].width_log2;
        height_log2 = hud_state.image_pages[i].height_log2;
        size = 1 << (width_log2 + height_log2);
        hud_send_texture(hud_state.image_pages[i].source_address, page, 0x1B, width_log2,
                         height_log2, immediate);
        address += size * 4;
        hud_state.image_pages[i].gs_block_offset = page;
    }
}

extern __typeof__(hud_send_resident_bank) func_001FF128 __attribute__((alias("FUN_001ff128")));
