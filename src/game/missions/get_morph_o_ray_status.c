/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CD80). */
#include "sda.h"

extern int D_0013DB2C           NOT_SDA;
extern unsigned char D_0013D4D5 NOT_SDA;
int                             get_morph_o_ray_status(void) __asm__("FUN_0020bf30");

int get_morph_o_ray_status(void) {
    if (D_0013D4D5 != 0)
        return 2;
    return D_0013DB2C != 0;
}

extern __typeof__(get_morph_o_ray_status) func_0020BF30 __attribute__((alias("FUN_0020bf30")));
