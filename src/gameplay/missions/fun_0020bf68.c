/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CDB8). */
#include "sda.h"
extern int D_0013D4C0 NOT_SDA;
int FUN_0020bf68(void) {
    unsigned char *base = (unsigned char *)&D_0013D4C0;
    if (base[0x1F] != 0)
        return 2;
    return base[0x21] != 0;
}

extern __typeof__(FUN_0020bf68) func_0020BF68 __attribute__((alias("FUN_0020bf68")));
