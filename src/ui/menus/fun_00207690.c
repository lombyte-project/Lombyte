/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00207EC0). */
#include "sda.h"
extern unsigned char D_0013D3D8 NOT_SDA;
/* The recorded C was right; the two ps2eeas nops were the only
   residual. */
int FUN_00207690(int arg0, float unused1, float unused2, float arg1) {
    if (arg0 >= 0xBE) return D_0013D3D8 != 0;
    return (arg1 >= 58.5f) ? 1 : 0;
}

extern __typeof__(FUN_00207690) func_00207690 __attribute__((alias("FUN_00207690")));
