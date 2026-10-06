#include "types.h"
extern u8 D_00151900[];
extern u8 *D_00160F00;
void aa_blur_pass(void) __asm__("FUN_001fb680");

void aa_blur_pass(void) {
    *(u32 *)(D_00160F00 + 0) = 0x30000026;
    *(u32 *)(D_00160F00 + 4) = (u32)D_00151900;
    *(u32 *)(D_00160F00 + 8) = 0;
    *(u32 *)(D_00160F00 + 12) = 0x50000026;
    D_00160F00 += 0x10;
}

extern __typeof__(aa_blur_pass) func_001FB680
    __attribute__((alias("FUN_001fb680")));
