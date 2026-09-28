/* Ported from rac1-decomp, the PAL decompilation (src/game/missionfunc.c, func_0020CC38). */
#include "sda.h"
extern int D_0013DA1C NOT_SDA;
extern unsigned char D_0013D3E9 NOT_SDA;
int FUN_0020bde8(void) {
    if (D_0013DA1C != 0 && D_0013D3E9 != 0) return 1;
    return 0;
}

extern __typeof__(FUN_0020bde8) func_0020BDE8 __attribute__((alias("FUN_0020bde8")));
