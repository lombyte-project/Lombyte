/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CDB8). */
#include "rnc/gameplay/state/item_state.h"
int FUN_0020bf68(void) {
    if (item_available[0x1F] != 0)
        return 2;
    return item_available[0x21] != 0;
}

extern __typeof__(FUN_0020bf68) func_0020BF68 __attribute__((alias("FUN_0020bf68")));
