#include "types.h"
extern s16 D_00199710[150][2];
s32 find_help_entry(s16 id, s32 which, u16 *other) __asm__("FUN_001fecc8");

s32 find_help_entry(s16 id, s32 which, u16 *other) {
    s32 i;

    for (i = 0; i < 150; i++) {
        if (D_00199710[i][which] == id) {
            if (other != 0) {
                *other = D_00199710[i][which ? 0 : 1];
            }
            return i;
        }
    }
    return -1;
}

extern __typeof__(find_help_entry) func_001FECC8 __attribute__((alias("FUN_001fecc8")));
