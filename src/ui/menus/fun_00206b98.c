/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_002073C8). */
#include "sda.h"
extern unsigned char D_0013D3A5 NOT_SDA;
int FUN_00206b98(void) {
    return D_0013D3A5 != 0;
}

extern __typeof__(FUN_00206b98) func_00206B98 __attribute__((alias("FUN_00206b98")));
