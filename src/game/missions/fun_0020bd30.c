/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CB80). */
#include "sda.h"
extern int D_0013D73C NOT_SDA;
extern unsigned char D_0013D3A0 NOT_SDA;
int FUN_0020bd30(void) {
    if (D_0013D73C != 0 && D_0013D3A0 != 0)
        return 1;
    return 0;
}

extern __typeof__(FUN_0020bd30) func_0020BD30 __attribute__((alias("FUN_0020bd30")));
