#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_NANOTECH_BALL_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_NANOTECH_BALL_H

#include "types.h"

void  nanotech_ball_update(void *) __asm__("FUN_L00_002d89f0");
void  suckcannon_update(void *) __asm__("FUN_L00_002da790");
void  update_linked_moby_transform_and_lifetime(void *) __asm__("FUN_L00_002d7f88");
void *get_moby_suck_cannon_record(void *) __asm__("FUN_L00_002db890");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_NANOTECH_BALL_H */
