#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TURRET_SHOT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TURRET_SHOT_H

#include "rnc/overlay/shared/math/quad.h"
#include "types.h"

struct Moby;

void         turret_shot_update(struct Moby *) __asm__("FUN_L09_00307d68");
struct Moby *spawn_turret_shot(void *, OvlVec4 *, OvlVec4 *, s32) __asm__("FUN_L09_00307ba8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TURRET_SHOT_H */
