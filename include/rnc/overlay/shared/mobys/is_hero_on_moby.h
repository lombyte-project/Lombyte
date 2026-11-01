#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_IS_HERO_ON_MOBY_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_IS_HERO_ON_MOBY_H

#include "types.h"

void spawn_proximity_gated_bolts(void *) __asm__("FUN_L01_002787a0");

struct Moby;

void spawn_moby_from_placement(struct Moby *, int) __asm__("FUN_L01_00278e20");
s32  is_hero_on_moby(struct Moby *) __asm__("FUN_L01_00277fb8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_IS_HERO_ON_MOBY_H */
