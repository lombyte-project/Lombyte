/* Ported from rac1-decomp (src/game/menu.c, func_002073D8). */
#include "sda.h"
extern unsigned char D_0013D3A6 NOT_SDA;
int FUN_00206ba8(void) {
    return D_0013D3A6 != 0;
}

extern __typeof__(FUN_00206ba8) func_00206BA8 __attribute__((alias("FUN_00206ba8")));
