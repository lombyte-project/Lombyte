/* Ported from rac1-decomp (src/game/pause.c, func_00222D70). */
#include "sda.h"
extern int D_0015ED84 MACRO_ADDR;
extern int D_001D4528[];
/* `D_001D4528[(unsigned)D_0015ED84 % 19]`; the older near-miss
   predated MACRO_ADDR. */
int FUN_00221d28(char *arg0) {
    *(int *)(arg0 + 0x34) = D_001D4528[(unsigned int)D_0015ED84 % 19];
    return 0;
}

extern __typeof__(FUN_00221d28) func_00221D28 __attribute__((alias("FUN_00221d28")));
