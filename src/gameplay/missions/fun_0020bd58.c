/* Ported from rac1-decomp, the PAL decompilation (src/game/missionfunc.c, func_0020CBA8). */
#include "sda.h"
extern int D_0013D8AC NOT_SDA;
extern unsigned char D_0013D388[];
int FUN_0020bd58(void) {
    if (D_0013D8AC != 0 && D_0013D388[0x20] != 0 && D_0013D388[0x21] != 0) return 1;
    return 0;
}

extern __typeof__(FUN_0020bd58) func_0020BD58 __attribute__((alias("FUN_0020bd58")));
