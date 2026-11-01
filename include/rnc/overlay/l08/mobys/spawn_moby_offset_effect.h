#ifndef LOMBYTE_RNC_OVERLAY_L08_MOBYS_SPAWN_MOBY_OFFSET_EFFECT_H
#define LOMBYTE_RNC_OVERLAY_L08_MOBYS_SPAWN_MOBY_OFFSET_EFFECT_H

#include "types.h"
#include "rnc/sdk/eetypes.h"

unsigned char *spawn_moby_offset_effect(char *) __asm__("FUN_L08_00279f00");
void spawn_effect_with_initial_motion(u128 *, u128 *, int, float) __asm__("FUN_L08_0027fe00");

#endif /* LOMBYTE_RNC_OVERLAY_L08_MOBYS_SPAWN_MOBY_OFFSET_EFFECT_H */
