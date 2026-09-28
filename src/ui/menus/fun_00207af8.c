/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00208328). */
#include "sda.h"
extern unsigned char D_0013D407 NOT_SDA;
int FUN_00207af8(void) {
    return D_0013D407 != 0;
}

extern __typeof__(FUN_00207af8) func_00207AF8 __attribute__((alias("FUN_00207af8")));
