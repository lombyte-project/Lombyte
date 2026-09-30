#include "sda.h"
extern int D_0015EE90 MACRO_ADDR;
extern int D_001D4810[];
extern int D_001D4840[];

int FUN_0021a1b0(int *p) {
    if (D_0015EE90 != 0) {
        p[0xD] = (int)D_001D4810;
    } else {
        p[0xD] = (int)D_001D4840;
    }
    return 0;
}

extern __typeof__(FUN_0021a1b0) func_0021A1B0 __attribute__((alias("FUN_0021a1b0")));
