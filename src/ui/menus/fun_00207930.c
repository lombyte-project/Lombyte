/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00208160). */
#include "sda.h"
extern unsigned char D_0013D3E3 NOT_SDA;
int FUN_00207930(int arg0, float unused1, float unused2, float arg1) {
    if (arg0 < 0x15F) {
        return (arg1 >= 228.0f && arg1 <= 230.0f) ? 1 : 0;
    }
    return arg1 >= 233.0f && arg1 <= 235.0f && D_0013D3E3 != 0;
}

extern __typeof__(FUN_00207930) func_00207930 __attribute__((alias("FUN_00207930")));
