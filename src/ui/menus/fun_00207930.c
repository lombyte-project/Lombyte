/* Ported from rac1-decomp (src/game/menu.c, func_00208160). */
#include "sda.h"
extern unsigned char D_0013D3E3 NOT_SDA;
int FUN_00207930(int x, float unused1, float unused2, float y) {
    if (x < 0x15F) {
        return (y >= 228.0f && y <= 230.0f) ? 1 : 0;
    }
    return y >= 233.0f && y <= 235.0f && D_0013D3E3 != 0;
}

extern __typeof__(FUN_00207930) func_00207930 __attribute__((alias("FUN_00207930")));
