#include "types.h"
#include "rnc/globals.h"
extern s32 vu_chain_table[] __asm__("D_001DDFB8");
extern s32 current_vu_chain __asm__("D_00160F0C");

void calculate_dma_transfer_address(void) __asm__("CalculateDmaTransferAddress");

void calculate_dma_transfer_address(void) {
    s32 i;

    i = current_level_index;
    if (i >= 0x13) {
        i = 0;
    }
    current_vu_chain = vu_chain_table[i];
}
