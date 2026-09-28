/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00207F20). */
#include "sda.h"
extern unsigned char D_0013D3CC NOT_SDA;
int FUN_00207670(void) {
    return D_0013D3CC != 0;
}

extern __typeof__(FUN_00207670) func_00207670 __attribute__((alias("FUN_00207670")));
