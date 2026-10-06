/* Ported from rac1-decomp (src/game/menu.c, func_00207F00). */
#include "sda.h"
extern unsigned char D_0013D3CB NOT_SDA;
int FUN_00207660(void) {
    return D_0013D3CB != 0;
}

extern __typeof__(FUN_00207660) func_00207660 __attribute__((alias("FUN_00207660")));
