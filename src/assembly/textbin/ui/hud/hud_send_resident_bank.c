#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/hud/hud_send_resident_bank/FUN_001ff128.s",
            FUN_001ff128);
#else
#include "types.h"
struct TexEntry {
    u32 data;
    s16 page;
    u8 log_w;
    u8 log_h;
};
struct TexCounts {
    u8 pad0[0x14];
    s32 mid_ends[8];
    s32 ends[16];
    s32 loaded[16];
};
struct TexTable {
    u8 pad0[0x18];
    struct TexCounts *counts;
    u8 pad1C[8];
    struct TexEntry *entries;
    struct TexEntry *mids;
};
extern struct TexTable D_0019A3E8;
extern struct TexTable hud_texture_table __asm__("D_0019A3E8");
extern struct TexCounts *hud_texture_counts __asm__("D_0019A400");
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
    struct TexCounts *previous_counts;

    if (D_0019A3E8.counts->loaded[bank] == 0) {
        link_hud_bank(0, base);
    }
    addr = depth_buffer_address;
    previous_counts = hud_texture_counts;
    i = bank == 0 ? 0 : previous_counts->ends[bank - 1];
    end = *(s32 *)((u8 *)D_0019A3E8.counts + 0x34 + (bank << 2));
    for (; i < end; i++) {
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
