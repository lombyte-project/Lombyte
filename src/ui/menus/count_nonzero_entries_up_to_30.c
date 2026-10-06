#include "types.h"
extern u8 D_0013D408[];
s32 count_nonzero_entries_up_to_30(void) __asm__("FUN_00215348");

s32 count_nonzero_entries_up_to_30(void) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if (D_0013D408[i] != 0) {
            count++;
        }
    }
    if (count < 0) {
        count = 0;
    }
    return (count < 0x1F) ? count : 0x1E;
}

extern __typeof__(count_nonzero_entries_up_to_30) func_00215348
    __attribute__((alias("FUN_00215348")));
