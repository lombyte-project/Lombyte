/* Ported from rac1-decomp (src/game/menu.c, func_00208030). */
#include "sda.h"
extern unsigned char D_0013D3E0 NOT_SDA;
int FUN_00207800(int arg0, float unused1, float unused2, float arg1) {
    if (arg0 < 0xDD) {
        return (arg1 >= 238.0f && arg1 <= 241.0f) ? 1 : 0;
    }
    return arg1 <= 180.0f && D_0013D3E0 != 0;
}

extern __typeof__(FUN_00207800) func_00207800 __attribute__((alias("FUN_00207800")));
