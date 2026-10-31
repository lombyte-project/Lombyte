#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TURRET_ROCKET_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TURRET_ROCKET_H

#include "types.h"

void  turret_rocket_update(char *) __asm__("FUN_L08_002e8210");
char *spawn_turret_rocket(int, float *, float *, int) __asm__("FUN_L08_002e7eb0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TURRET_ROCKET_H */
