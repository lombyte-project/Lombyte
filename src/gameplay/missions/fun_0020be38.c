/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CC88). */
#include "rnc/gameplay/state/item_state.h"
int FUN_0020be38(void) {
    if (item_available[0x21] != 0 && item_available[0x1F] != 0)
        return 1;
    return 0;
}

extern __typeof__(FUN_0020be38) func_0020BE38 __attribute__((alias("FUN_0020be38")));
