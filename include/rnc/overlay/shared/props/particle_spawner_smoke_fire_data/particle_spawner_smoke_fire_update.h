#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_PARTICLE_SPAWNER_SMOKE_FIRE_DATA_PARTICLE_SPAWNER_SMOKE_FIRE_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_PARTICLE_SPAWNER_SMOKE_FIRE_DATA_PARTICLE_SPAWNER_SMOKE_FIRE_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/shared/props/particle_spawner_smoke_fire.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int smoke_fire_emitted_particle_sign_randomized_sample_min_gp __asm__("D_L00_001619C0")
    __attribute__((sda));
extern int smoke_fire_emitted_particle_sign_randomized_sample_max_gp __asm__("D_L00_001619C4")
    __attribute__((sda));
extern int smoke_fire_emitted_particle_base_color_gp __asm__("D_L00_001619C8") __attribute__((sda));
extern float smoke_fire_far_culling_distance_gp __asm__("D_L00_001619E0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_PARTICLE_SPAWNER_SMOKE_FIRE_DATA_PARTICLE_SPAWNER_SMOKE_FIRE_UPDATE_H */
