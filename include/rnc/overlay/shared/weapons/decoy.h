#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DECOY_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DECOY_H

#include "types.h"

void decoy_update(char *) __asm__("FUN_L00_002c4380");
void release_moby_with_collision_check(float *, char *, void *) __asm__("FUN_L00_002c3888");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DECOY_H */
