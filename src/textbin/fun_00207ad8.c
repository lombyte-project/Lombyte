/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00208308). */
#include "sda.h"
extern unsigned char D_0013D3FC NOT_SDA;
int FUN_00207ad8(void) {
    return D_0013D3FC != 0;
}

extern __typeof__(FUN_00207ad8) func_00207AD8 __attribute__((alias("FUN_00207ad8")));
