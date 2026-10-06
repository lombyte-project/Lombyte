/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CC10). */
#include "sda.h"
extern int D_0013D9DC NOT_SDA;
extern unsigned char D_0013D3DD NOT_SDA;
int FUN_0020bdc0(void) {
    if (D_0013D9DC != 0 && D_0013D3DD != 0)
        return 1;
    return 0;
}

extern __typeof__(FUN_0020bdc0) func_0020BDC0 __attribute__((alias("FUN_0020bdc0")));
