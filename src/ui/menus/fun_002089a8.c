#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0 MACRO_ADDR;

void FUN_002089a8(void) {
    char *s = D_0013D290;
    *(int *)(s + 0xDC) = -1;
    D_0015EEB0 = 4;
    *(int *)(s + 0xE0) = -1;
}

extern __typeof__(FUN_002089a8) func_002089A8 __attribute__((alias("FUN_002089a8")));
