/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00208318). */
#include "sda.h"
extern unsigned char D_0013D3FD NOT_SDA;
int FUN_00207ae8(void) {
    return D_0013D3FD != 0;
}

extern __typeof__(FUN_00207ae8) func_00207AE8 __attribute__((alias("FUN_00207ae8")));
