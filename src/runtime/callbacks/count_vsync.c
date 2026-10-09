/* Ported from rac1-decomp (src/core/permcb.c, func_0012F308). */
#include "sda.h"
extern long D_0015ED40 MACRO_ADDR;
extern long D_0015ED48 MACRO_ADDR;
extern long D_0015ED50 MACRO_ADDR;
int count_vsync(void) __asm__("FUN_0012f1c8");

int count_vsync(void) {
    D_0015ED48++;
    D_0015ED50 = D_0015ED40 + (unsigned int)*(volatile int *)0x10000800;
    return 0;
}

extern __typeof__(count_vsync) func_0012F1C8 __attribute__((alias("FUN_0012f1c8")));

/* Defined below their only user: Ps2EeAs does not know their size at the use,
   so retail reaches them with lui + %lo. */
long D_0015ED40 MACRO_ADDR = 0;
long D_0015ED48 MACRO_ADDR = 0;
long D_0015ED50 MACRO_ADDR = 0;
