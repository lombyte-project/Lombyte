#include "types.h"
#include "rnc/data_labels.h"

s32 count_gold_bolts(void) __asm__("FUN_00215290");

s32 count_gold_bolts(void) {
    s32 count = 0;
    s32 row;
    s32 k;

    for (row = 0; row < 0x14; row++) {
        for (k = 0; k < 4; k++) {
            if (D_0014BEC0[row * 4 + k] != 0) {
                count++;
            }
        }
    }
    if (count < 0) {
        count = 0;
    }
    return (count < 0x29) ? count : 0x28;
}

extern __typeof__(count_gold_bolts) func_00215290 __attribute__((alias("FUN_00215290")));
