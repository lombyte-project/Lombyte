#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB4 __attribute__((sda));
extern int D_0015EEB0;

void FUN_00208c70(void) {
    char *s = D_0013D290;
    if (*(int *)(s + 0x1C) != 0) {
        D_0015EEB0 = 3;
        return;
    }
    if (D_0015EEB4 & 2) D_0015EEB0 = 0xD;
}

extern __typeof__(FUN_00208c70) func_00208C70 __attribute__((alias("FUN_00208c70")));
