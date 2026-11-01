#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_VISIBOMB_MISSILE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_VISIBOMB_MISSILE_H

#include "types.h"

struct Moby;

void visibomb_missile_update(struct Moby *) __asm__("FUN_L00_002b6140");
s32  spawn_visibomb_missile(void *, void *, f32, f32) __asm__("FUN_L00_002b58d8");
void terminate_visibomb_missile(void *, char **) __asm__("FUN_L00_002b5b20");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_VISIBOMB_MISSILE_H */
