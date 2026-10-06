/* Ported from rac1-decomp (src/game/menu.c, func_002081F8). */
#include "sda.h"
extern unsigned char D_0013D3E1 NOT_SDA;
int FUN_002079c8(void) {
    return D_0013D3E1 != 0;
}

extern __typeof__(FUN_002079c8) func_002079C8 __attribute__((alias("FUN_002079c8")));
