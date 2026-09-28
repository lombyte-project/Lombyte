/* Ported from rac1-decomp, the PAL decompilation (src/game/missionfunc.c, func_0020CD28). */
#include "sda.h"
extern int D_0013D8AC NOT_SDA;
extern unsigned char D_0013D3A8 NOT_SDA;
int FUN_0020bed8(void) {
    if (D_0013D8AC != 0) {
        return D_0013D3A8 ? 2 : 1;
    }
    return 0;
}

extern __typeof__(FUN_0020bed8) func_0020BED8 __attribute__((alias("FUN_0020bed8")));
