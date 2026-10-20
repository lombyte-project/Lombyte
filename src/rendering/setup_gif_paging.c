/* Ported from rac1-decomp (src/game/draw.c, func_001F4630). */
#include "sda.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"
#include "types.h"
extern int *gif_page_marker_a __asm__("D_0015F450") MACRO_ADDR;
extern int effect_tex_cnt __asm__("D_0015F45C") MACRO_ADDR;
extern s32 pending_texture_upload_count __asm__("D_0015F458");
typedef struct {
    long unk0;
    long unk8;
} PageSlot;
extern char D_0019A3E8[];
static inline char *PagingArena(void) {
    return D_0019A3E8;
}
/* SetupGifPaging(int): marks the render_packet_cursor.words packet in D_0015F450 and
   reserves 0x10 bytes, copies D_0015EE78 to D_0015EE74, clears
   pending_texture_upload_count and the first dword of the D_0015F45C paging slots, then,
   when arg0 is 0, clears the +4 half of the arena's 8-byte list entries:
   the list at D_0019A3E8 + 0x24 (count at +0x44 of the record at +0x18)
   where it is at least D_0015EE8C >> 8, and all of the list at + 0x28
   (count at +0x24). The slot loop has its own counter, which loop
   reversal copies from the count (retail's $v1). The first list reads
   the arena through a static inline accessor (a fresh pseudo per read
   gives retail's copy for the loop) with the element offset first
   (`i * 8 + base`); the second through a block-local pointer, whose
   %hi retail keeps. */
void setup_gif_paging(int arg0) __asm__("FUN_001f4280");

extern PageSlot effect_texture_definitions[] __asm__("D_0018D440");
extern s32 gs_texture_allocation_start __asm__("D_0015EE78") MACRO_ADDR;
extern s32 gs_texture_allocation_cursor __asm__("D_0015EE74") MACRO_ADDR;
extern s32 gs_texture_allocation_base_macro __asm__("D_0015EE8C") MACRO_ADDR;
void setup_gif_paging(int skip_paging_entry_reset) {
    int *packet_cursor = render_packet_cursor.words;
    int i;

    gif_page_marker_a = packet_cursor;
    packet_cursor += 4;
    render_packet_cursor.words = packet_cursor;
    gs_texture_allocation_cursor = gs_texture_allocation_start;
    *(int *)&pending_texture_upload_count = 0;
    {
        int k;

        for (k = 0; k < effect_tex_cnt; k++) {
            effect_texture_definitions[k].unk0 = 0;
        }
    }
    if (skip_paging_entry_reset == 0) {
        for (i = 0; i < *(int *)(*(char **)(PagingArena() + 0x18) + 0x44); i++) {
            unsigned short *el = (unsigned short *)(i * 8 + *(int *)(PagingArena() + 0x24) + 4);

            if (*el >= (gs_texture_allocation_base_macro >> 8)) {
                *el = 0;
            }
        }
        {
            char *b = D_0019A3E8;
            int j;

            for (j = 0; j < *(int *)(*(char **)(b + 0x18) + 0x24); j++) {
                *(short *)(*(char **)(b + 0x28) + j * 8 + 4) = 0;
            }
        }
    }
}

extern __typeof__(setup_gif_paging) func_001F4280 __attribute__((alias("FUN_001f4280")));
