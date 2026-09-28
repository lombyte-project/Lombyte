/* Ported from rac1-decomp, the PAL decompilation (src/game/missionfunc.c, func_0020CDA8). */
#include "sda.h"
extern unsigned char D_0013D4DF NOT_SDA;
int FUN_0020bf58(void) {
    return D_0013D4DF != 0;
}

extern __typeof__(FUN_0020bf58) func_0020BF58 __attribute__((alias("FUN_0020bf58")));
