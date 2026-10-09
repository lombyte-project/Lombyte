/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CCB8). */
#include "rnc/gameplay/state/item_state.h"
int FUN_0020be68(int arg0) {
    return item_available[arg0] != 0;
}

extern __typeof__(FUN_0020be68) func_0020BE68 __attribute__((alias("FUN_0020be68")));
