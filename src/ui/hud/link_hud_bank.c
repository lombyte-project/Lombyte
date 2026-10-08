#include "types.h"
#include "rnc/ui/hud/hud_state.h"
void link_hud_bank(s32 bank, u32 base) __asm__("FUN_001fefc0");

void link_hud_bank(s32 bank, u32 base) {
    s32 i;
    s32 end;
    s32 start;

    if (hud_state.header.counts->loaded[bank] != 0) {
        return;
    }
    base = (base + 15) & ~15;
    hud_state.header.counts->loaded[bank] = base;
    start = bank != 0 ? hud_state.header.counts->mid_ends[bank - 1] : 0;
    end = hud_state.header.counts->mid_ends[bank];
    for (i = start; i < end; i++) {
        hud_state.palette_pages[i].source_address &= 0x7FFFFFFF;
        hud_state.palette_pages[i].source_address += base;
    }
    start = bank != 0 ? hud_state.header.counts->ends[bank - 1] : 0;
    end = hud_state.header.counts->ends[bank];
    for (i = start; i < end; i++) {
        hud_state.image_pages[i].source_address &= 0x7FFFFFFF;
        hud_state.image_pages[i].source_address += base;
    }
}

extern __typeof__(link_hud_bank) func_001FEFC0 __attribute__((alias("FUN_001fefc0")));
