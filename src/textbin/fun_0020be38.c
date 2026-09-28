/* Ported from rac1-decomp, the PAL decompilation (src/game/missionfunc.c, func_0020CC88). */
#include "sda.h"
extern int D_0013D4C0 NOT_SDA;
int FUN_0020be38(void) {
    unsigned char *base = (unsigned char *)&D_0013D4C0;
    if (base[0x21] != 0 && base[0x1F] != 0) return 1;
    return 0;
}

extern __typeof__(FUN_0020be38) func_0020BE38 __attribute__((alias("FUN_0020be38")));
