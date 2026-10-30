/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CDA8). */
#include "sda.h"

extern unsigned char D_0013D4DF NOT_SDA;
int                             has_hologuise(void) __asm__("FUN_0020bf58");

int has_hologuise(void) {
    return D_0013D4DF != 0;
}

extern __typeof__(has_hologuise) func_0020BF58 __attribute__((alias("FUN_0020bf58")));
