#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TURRET_ROCKET_DATA_SPAWN_TURRET_ROCKET_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TURRET_ROCKET_DATA_SPAWN_TURRET_ROCKET_H

#include "types.h"

/* Scalar data for src/overlays/shared/weapons/turret_rocket.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float moby_creation_particle_primary_motion_w_gp __asm__("D_L08_00161D14")
    __attribute__((sda));
extern float moby_creation_particle_secondary_motion_w_gp __asm__("D_L08_00161D18")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TURRET_ROCKET_DATA_SPAWN_TURRET_ROCKET_H */
