#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0 MACRO_ADDR;

void FUN_00208eb8(void) {
    if (*(int *)(D_0013D290 + 0x1C) != 0) {
        D_0015EEB0 = 3;
    }
}

extern __typeof__(FUN_00208eb8) func_00208EB8 __attribute__((alias("FUN_00208eb8")));
