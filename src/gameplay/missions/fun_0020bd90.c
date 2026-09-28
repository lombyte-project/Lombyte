/* Ported from rac1-decomp, the PAL decompilation (src/game/missionfunc.c, func_0020CBE0). */
#include "sda.h"
extern int D_0013D5B0 NOT_SDA;
int FUN_0020bd90(void) {
    char *base = (char *)&D_0013D5B0;
    if (*(int *)(base + 0x40C) != 0 && *(int *)(base + 0x3FC) != 0) return 1;
    return 0;
}

extern __typeof__(FUN_0020bd90) func_0020BD90 __attribute__((alias("FUN_0020bd90")));
