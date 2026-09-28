/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00207F50). */
#include "sda.h"
extern unsigned char D_0013D3DE NOT_SDA;
int FUN_00207720(int arg0, float unused1, float unused2, float arg1) {
    if (arg0 < 0xDD) {
        return arg1 >= 232.0f && arg1 <= 235.0f && D_0013D3DE != 0;
    }
    return (arg1 <= 180.0f) ? 1 : 0;
}

extern __typeof__(FUN_00207720) func_00207720 __attribute__((alias("FUN_00207720")));
