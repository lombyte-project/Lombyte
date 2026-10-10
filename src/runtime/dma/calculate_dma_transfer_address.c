#include "types.h"
#include "rnc/globals.h"
extern s32 D_001DDFB8[];
extern s32 D_00160F0C;

void calculate_dma_transfer_address(void) __asm__("CalculateDmaTransferAddress");

void calculate_dma_transfer_address(void) {
    s32 i;

    i = current_level_index;
    if (i >= 0x13) {
        i = 0;
    }
    D_00160F0C = D_001DDFB8[i];
}
