/* Ported from rac1-decomp, the PAL decompilation (src/game/missionfunc.c, func_0020CCD0). */
#include "sda.h"
typedef struct { int a, b, c, d; } Rec16_C940;
extern Rec16_C940 D_0013D5B0[];
extern unsigned char D_0013D4C2 NOT_SDA;
/* Two flat `&&` returns over D_0013D5B0[20/24/22].d; retail's reuse of
   the %hi register comes out by itself. */
int FUN_0020be80(void) {
    if (D_0013D5B0[20].d != 0 && D_0013D5B0[24].d == 0) {
        return 1;
    }
    if (D_0013D5B0[24].d != 0 && D_0013D4C2 != 0 && D_0013D5B0[22].d == 0) {
        return 2;
    }
    return 0;
}

extern __typeof__(FUN_0020be80) func_0020BE80 __attribute__((alias("FUN_0020be80")));
