#include "types.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/world/level_id.h"

extern s32 vu_chain_table[LEVEL_COUNT] __asm__("D_001DDFB8");

void select_level_vu_chain(void) __asm__("CalculateDmaTransferAddress");

void select_level_vu_chain(void) {
    s32 i;

    i = current_level_index;
    if (i >= LEVEL_COUNT) {
        i = 0;
    }
    current_vu_chain = vu_chain_table[i];
}

/* Render buffer size in bytes per level (current_vu_chain). */
s32 vu_chain_table[LEVEL_COUNT] = {
    0x1C0000, 0x160000, 0x1A0000, 0x170000, 0x1A0000, 0x160000, 0x160000,
    0x1A0000, 0x1A0000, 0x1A0000, 0x1A0000, 0x170000, 0x170000, 0x170000,
    0x180000, 0x170000, 0x170000, 0x1A0000, 0x170000,
};
