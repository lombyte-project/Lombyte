/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_002082F8). */
#include "sda.h"
extern unsigned char D_0013D3FB NOT_SDA;
int FUN_00207ac8(void) {
    return D_0013D3FB != 0;
}

extern __typeof__(FUN_00207ac8) func_00207AC8 __attribute__((alias("FUN_00207ac8")));
