#include "sda.h"
extern int D_0015EEB0 MACRO_ADDR;
extern int D_0015EEB4 MACRO_ADDR;

void FUN_00208e90(void) {
    if (D_0015EEB4 & 0x40) {
        return;
    }
    D_0015EEB0 = 3;
}

extern __typeof__(FUN_00208e90) func_00208E90 __attribute__((alias("FUN_00208e90")));
