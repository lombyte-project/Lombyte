/* Ported from rac1-decomp (src/game/pause.c, func_0021DAC8). */
#include "sda.h"
extern int D_001A0318 NOT_SDA;
int FUN_0021cac8(void) {
    D_001A0318 = -1;
    return 0;
}

extern __typeof__(FUN_0021cac8) func_0021CAC8 __attribute__((alias("FUN_0021cac8")));
