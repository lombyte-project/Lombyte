/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_002073F8). */
#include "sda.h"
extern unsigned char D_0013D3AD NOT_SDA;
int FUN_00206bc8(void) {
    return D_0013D3AD != 0;
}

extern __typeof__(FUN_00206bc8) func_00206BC8 __attribute__((alias("FUN_00206bc8")));
