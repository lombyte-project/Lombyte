/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CBE0). */
#include "sda.h"
#include "rnc/game/level_object_states.h"

int FUN_0020bd90(void) {
    struct LevelObjectState *states = level_object_states;
    if (states[0x40].state != 0 && states[0x3F].state != 0)
        return 1;
    return 0;
}

extern __typeof__(FUN_0020bd90) func_0020BD90 __attribute__((alias("FUN_0020bd90")));
