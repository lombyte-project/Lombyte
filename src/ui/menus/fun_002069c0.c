/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_002071F0). */
#include "sda.h"
extern unsigned char D_0013D39D NOT_SDA;
int FUN_002069c0(void) {
    return D_0013D39D != 0;
}

extern __typeof__(FUN_002069c0) func_002069C0 __attribute__((alias("FUN_002069c0")));
