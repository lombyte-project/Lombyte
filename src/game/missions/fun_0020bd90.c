/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CBE0). */
#include "sda.h"
#include "rnc/game/level_object_states.h"

int FUN_0020bd90(void) {
    char *base = (char *)level_object_states;
    if (*(int *)(base + 0x40C) != 0 && *(int *)(base + 0x3FC) != 0)
        return 1;
    return 0;
}

extern __typeof__(FUN_0020bd90) func_0020BD90 __attribute__((alias("FUN_0020bd90")));
