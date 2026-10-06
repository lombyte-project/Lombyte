#include "types.h"
extern u8 D_0013E520[];
s32 count_nonzero_entries_up_to_10(void) __asm__("FUN_00215300");

s32 count_nonzero_entries_up_to_10(void) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < 0x25; i++) {
        if (D_0013E520[i] != 0) {
            count++;
        }
    }
    if (count < 0) {
        count = 0;
    }
    return (count < 0xB) ? count : 0xA;
}

extern __typeof__(count_nonzero_entries_up_to_10) func_00215300
    __attribute__((alias("FUN_00215300")));
