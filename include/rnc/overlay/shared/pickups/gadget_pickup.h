#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_GADGET_PICKUP_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_GADGET_PICKUP_H

#include "types.h"

struct Moby;

void tresspasser_update(struct Moby *) __asm__("FUN_L02_002ea210");
void initialize_collectible_sprite_state(char *) __asm__("FUN_L02_002ea7d0");
void update_collectible_sprite_effect(char *) __asm__("FUN_L02_002ea8b0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_GADGET_PICKUP_H */
