/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CDB8). */
#include "rnc/game/item_state.h"

int mission_get_hologuise_or_map_status(void) __asm__("FUN_0020bf68");

int mission_get_hologuise_or_map_status(void) {
    if (item_available[0x1F] != 0)
        return 2;
    return item_available[0x21] != 0;
}

extern __typeof__(mission_get_hologuise_or_map_status) func_0020BF68
    __attribute__((alias("FUN_0020bf68")));
