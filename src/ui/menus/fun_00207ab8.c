/* Ported from rac1-decomp (src/game/menu.c, func_002082E8). */
#include "sda.h"
extern unsigned char D_0013D3FA NOT_SDA;
int FUN_00207ab8(void) {
    return D_0013D3FA != 0;
}

extern __typeof__(FUN_00207ab8) func_00207AB8 __attribute__((alias("FUN_00207ab8")));
