#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_MORPHORAY_CHICKEN_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_MORPHORAY_CHICKEN_H

#include "types.h"

struct Moby;

s32  FUN_L00_002caf40(s32);
int  cancel_suck_cannon_suction_and_set_moby_state_5(struct Moby *) __asm__("FUN_L00_002caf10");
int  launch_suck_cannon_projectile_and_update_moby_state(struct Moby *,
                                                         int,
                                                         int,
                                                         float) __asm__("FUN_L00_002cae60");
int  moby_set_state_from_check(struct Moby *) __asm__("FUN_L00_002cadc0");
void morphoray_chicken_update() __asm__("FUN_L00_002c97e0");
struct Moby *spawn_morphoray_chicken(void *) __asm__("FUN_L00_002c9348");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_MORPHORAY_CHICKEN_H */
