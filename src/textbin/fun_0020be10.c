/* Ported from rac1-decomp, the PAL decompilation (src/game/missionfunc.c, FUN_0020cc60). */
#include "sda.h"
extern int D_0013DB2C NOT_SDA;
extern unsigned char D_0013D4FD NOT_SDA;
int FUN_0020be10(void) {
    if (D_0013DB2C != 0 && D_0013D4FD != 0) return 1;
    return 0;
}

extern __typeof__(FUN_0020be10) func_0020BE10 __attribute__((alias("FUN_0020be10")));
