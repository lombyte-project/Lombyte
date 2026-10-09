/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CD58). */
#include "sda.h"
extern unsigned char D_0013D3E9 NOT_SDA;
#include "rnc/gameplay/state/level_state.h"
int FUN_0020bf08(void) {
    if (D_0013D3E9 != 0 && level_available[13] != 0)
        return 1;
    return 0;
}

extern __typeof__(FUN_0020bf08) func_0020BF08 __attribute__((alias("FUN_0020bf08")));
