/* Ported from rac1-decomp (src/game/menu.c, func_002071D0). */
#include "sda.h"
extern unsigned char D_0013D394 NOT_SDA;
int FUN_002069a0(void) {
    return D_0013D394 != 0;
}

extern __typeof__(FUN_002069a0) func_002069A0 __attribute__((alias("FUN_002069a0")));
