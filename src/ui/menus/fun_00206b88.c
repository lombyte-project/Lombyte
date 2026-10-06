/* Ported from rac1-decomp (src/game/menu.c, FUN_002073b8). */
#include "sda.h"
extern unsigned char D_0013D3A4 NOT_SDA;
int FUN_00206b88(void) {
    return D_0013D3A4 != 0;
}

extern __typeof__(FUN_00206b88) func_00206B88 __attribute__((alias("FUN_00206b88")));
