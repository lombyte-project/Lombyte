#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_GROUND_PARTICLE_SCATTER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_GROUND_PARTICLE_SCATTER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/rendering/effects/ground_particle_scatter.c (the spawn vector bias also by radial_particle_emitter.c). */
extern float ground_particle_scatter_refresh_countdown __asm__("D_L08_00162370");
extern int   ground_particle_scatter_base_attempt_count __asm__("D_L08_00162374");
extern float ground_particle_scatter_spawn_vector_y_bias __asm__("D_L08_00162364");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_GROUND_PARTICLE_SCATTER_GLOBALS_H */
