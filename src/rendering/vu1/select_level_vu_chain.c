#include "types.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"

extern s32 vu_chain_table[] __asm__("D_001DDFB8");

void select_level_vu_chain(void) __asm__("CalculateDmaTransferAddress");

void select_level_vu_chain(void) {
    s32 i;

    i = current_level_index;
    if (i >= 0x13) {
        i = 0;
    }
    current_vu_chain = vu_chain_table[i];
}
