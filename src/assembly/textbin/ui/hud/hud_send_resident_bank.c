#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/hud/hud_send_resident_bank/FUN_001ff128.s", FUN_001ff128);
#else
#include "types.h"
struct TexEntry { u32 data; s16 page; u8 log_w; u8 log_h; };
struct TexCounts { u8 pad0[0x14]; s32 mid_ends[8]; s32 ends[16]; s32 loaded[16]; };
struct TexTable { u8 pad0[0x18]; struct TexCounts *counts; u8 pad1C[8]; struct TexEntry *entries; struct TexEntry *mids; };
extern struct TexTable D_0019A3E8;
extern s32 D_0015EE88[];
extern void link_hud_bank(s32, s32) __asm__("FUN_001fefc0");
extern void hud_send_texture(u32, s32, s32, s32, s32, s32) __asm__("FUN_00200b10");
void hud_send_resident_bank(s32 bank, s32 base, s32 immediate) __asm__("FUN_001ff128");

extern struct TexTable D_0019A3E8_far __asm__("D_0019A3E8") __attribute__((section(".data")));
void hud_send_resident_bank(s32 bank, s32 base, s32 immediate) {
    s32 addr;
    s32 i;
    s32 end;
    s32 page;
    s32 start;
    s32 w;
    s32 h;
    s32 size;

    if (D_0019A3E8.counts->loaded[bank] == 0) {
        link_hud_bank(0, base);
    }
    addr = D_0015EE88[0];
    start = bank != 0 ? D_0019A3E8_far.counts->ends[bank - 1] : 0;
    end = D_0019A3E8.counts->ends[bank];
    for (i = start; i < end; i++) {
        page = addr >> 8;
        w = D_0019A3E8.entries[i].log_w;
        h = D_0019A3E8.entries[i].log_h;
        size = 1 << (w + h);
        hud_send_texture(D_0019A3E8.entries[i].data, page, 0x1B, w, h, immediate);
        addr += size * 4;
        D_0019A3E8.entries[i].page = page;
    }
}
#endif /* NON_MATCHING */
