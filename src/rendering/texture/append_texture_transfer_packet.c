#include "types.h"
extern u8 D_00151900[];
extern u8 *D_00160F00;
void append_texture_transfer_packet(void) __asm__("FUN_001fb680");

void append_texture_transfer_packet(void) {
    *(u32 *)(D_00160F00 + 0) = 0x30000026;
    *(u32 *)(D_00160F00 + 4) = (u32)D_00151900;
    *(u32 *)(D_00160F00 + 8) = 0;
    *(u32 *)(D_00160F00 + 12) = 0x50000026;
    D_00160F00 += 0x10;
}

extern __typeof__(append_texture_transfer_packet) func_001FB680
    __attribute__((alias("FUN_001fb680")));
