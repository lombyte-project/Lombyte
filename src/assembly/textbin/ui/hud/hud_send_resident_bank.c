#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/hud/hud_send_resident_bank/FUN_001ff128.s",
            FUN_001ff128);
#else
#include "types.h"
#include "rnc/ui/hud/hud_state.h"
#define depth_buffer_address (*(s32 *)0x0015EE88)
extern void link_hud_bank(s32, s32) __asm__("FUN_001fefc0");
extern void hud_send_texture(u32, s32, s32, s32, s32, s32) __asm__("FUN_00200b10");
void hud_send_resident_bank(s32 bank, s32 base, s32 immediate) __asm__("FUN_001ff128");

void hud_send_resident_bank(s32 bank, s32 base, s32 immediate) {
    s32 addr;
    s32 i;
    s32 end;
    s32 page;
    s32 w;
    s32 h;
    s32 size;
    struct HudTexCounts *previous_counts;

    if (hud_state.header.counts->loaded[bank] == 0) {
        link_hud_bank(0, base);
    }
    addr = depth_buffer_address;
    previous_counts = hud_state.header.counts;
    i = bank == 0 ? 0 : previous_counts->ends[bank - 1];
    end = *(s32 *)((u8 *)hud_state.header.counts + 0x34 + (bank << 2));
    for (; i < end; i++) {
        page = addr >> 8;
        w = hud_state.image_pages[i].width_log2;
        h = hud_state.image_pages[i].height_log2;
        size = 1 << (w + h);
        hud_send_texture(hud_state.image_pages[i].source_address, page, 0x1B, w, h, immediate);
        addr += size * 4;
        hud_state.image_pages[i].gs_block_offset = page;
    }
}
#endif /* NON_MATCHING */
