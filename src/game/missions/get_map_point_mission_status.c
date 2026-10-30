/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CCD0). */
#include "sda.h"
#include "rnc/game/level_object_states.h"

extern unsigned char D_0013D4C2 NOT_SDA;
/* Two flat `&&` returns over level_object_states[20/24/22].state; retail's reuse of
   the %hi register comes out by itself. */
int get_map_point_mission_status(void) __asm__("FUN_0020be80");

int get_map_point_mission_status(void) {
    if (level_object_states[20].state != 0 && level_object_states[24].state == 0) {
        return 1;
    }
    if (level_object_states[24].state != 0 && D_0013D4C2 != 0 &&
        level_object_states[22].state == 0) {
        return 2;
    }
    return 0;
}

extern __typeof__(get_map_point_mission_status) func_0020BE80
    __attribute__((alias("FUN_0020be80")));
