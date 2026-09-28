/* Ported from rac1-decomp, the PAL decompilation (src/core/permcb.c, func_0012F308). */
#include "sda.h"
extern long D_0015ED40 MACRO_ADDR;
extern long D_0015ED48 MACRO_ADDR;
extern long D_0015ED50 MACRO_ADDR;
int FUN_0012f1c8(void) {
    D_0015ED48++;
    D_0015ED50 = D_0015ED40 + (unsigned int)*(volatile int *)0x10000800;
    return 0;
}

extern __typeof__(FUN_0012f1c8) func_0012F1C8 __attribute__((alias("FUN_0012f1c8")));
