/* Ported from rac1-decomp, the PAL decompilation (src/game/missionfunc.c, func_0020CCB8). */
#include "sda.h"
extern int D_0013D4C0 NOT_SDA;
int FUN_0020be68(int arg0) {
    unsigned char *base = (unsigned char *)&D_0013D4C0;
    return base[arg0] != 0;
}

extern __typeof__(FUN_0020be68) func_0020BE68 __attribute__((alias("FUN_0020be68")));
