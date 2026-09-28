#include "types.h"
struct TexEntry { u32 data; s16 page; u8 log_w; u8 log_h; };
struct TexCounts { u8 pad0[0x14]; s32 mid_ends[8]; s32 ends[16]; s32 loaded[16]; };
struct TexTable { u8 pad0[0x18]; struct TexCounts *counts; u8 pad1C[8]; struct TexEntry *entries; struct TexEntry *mids; };
extern struct TexTable D_0019A3E8;
void link_hud_bank(s32 bank, u32 base) __asm__("FUN_001fefc0");

void link_hud_bank(s32 bank, u32 base) {
    s32 i;
    s32 end;
    s32 start;

    if (D_0019A3E8.counts->loaded[bank] != 0) {
        return;
    }
    base = (base + 15) & ~15;
    D_0019A3E8.counts->loaded[bank] = base;
    start = bank != 0 ? D_0019A3E8.counts->mid_ends[bank - 1] : 0;
    end = D_0019A3E8.counts->mid_ends[bank];
    for (i = start; i < end; i++) {
        D_0019A3E8.mids[i].data &= 0x7FFFFFFF;
        D_0019A3E8.mids[i].data += base;
    }
    start = bank != 0 ? D_0019A3E8.counts->ends[bank - 1] : 0;
    end = D_0019A3E8.counts->ends[bank];
    for (i = start; i < end; i++) {
        D_0019A3E8.entries[i].data &= 0x7FFFFFFF;
        D_0019A3E8.entries[i].data += base;
    }
}

extern __typeof__(link_hud_bank) func_001FEFC0 __attribute__((alias("FUN_001fefc0")));
