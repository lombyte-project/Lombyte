#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_H

#include "types.h"

struct Moby;

void           buried_bolts_update(struct Moby *) __asm__("FUN_L01_002f2eb8");
void           codebot_update(struct Moby *) __asm__("FUN_L15_002ebd78");
void           crate_update(struct Moby *) __asm__("FUN_L00_002cf218");
void           nanotech_crate_fragment_update() __asm__("FUN_L00_002afa48");
unsigned char *spawn_ship_pickup(char *, int) __asm__("FUN_L13_002e1c50");
void           ammocrate_update(char *) __asm__("FUN_L13_002e1cc8");
void           tresspasser_update(struct Moby *) __asm__("FUN_L02_002ea210");
void           initialize_collectible_sprite_state(char *) __asm__("FUN_L02_002ea7d0");
void           update_collectible_sprite_effect(char *) __asm__("FUN_L02_002ea8b0");
void           nanotech_ball_update() __asm__("FUN_L00_002d89f0");
void           pilots_helmet_update() __asm__("FUN_L00_002e03a0");
void           devestator_ammo_update(struct Moby *) __asm__("FUN_L00_002c53c0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PICKUPS_H */
