/* Ported from rac1-decomp (src/game/menu.c, func_00207FD0). */
#include "sda.h"
extern unsigned char D_0013D3DF NOT_SDA;
int FUN_002077a0(int arg0, float unused1, float unused2, float arg1) {
    if (arg0 < 0xDD) {
        return (arg1 >= 242.0f) ? 1 : 0;
    }
    return arg1 <= 180.0f && D_0013D3DF != 0;
}

extern __typeof__(FUN_002077a0) func_002077A0 __attribute__((alias("FUN_002077a0")));
