#include "types.h"
extern s32 D_0018E740[];
void initialize_alpha_lookup_table(void) __asm__("FUN_001f7a30");

void initialize_alpha_lookup_table(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 0x100; i++) {
        j = i & 0xE7;
        if (i & 8) {
            j |= 0x10;
        }
        if (i & 0x10) {
            j |= 8;
        }
        D_0018E740[j] = (i >> 1) << 24;
    }
}

extern __typeof__(initialize_alpha_lookup_table) func_001F7A30
    __attribute__((alias("FUN_001f7a30")));
