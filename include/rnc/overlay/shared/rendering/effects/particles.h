#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_PARTICLES_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_PARTICLES_H

#include "types.h"

void *spawn_random_alpha_rotated_particle(void *, void *, f32) __asm__("FUN_L00_00271b18");
void *spawn_mode_controlled_rotating_alpha_fade_particle(void *,
                                                         void *,
                                                         s32,
                                                         void *) __asm__("FUN_L00_00274948");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_PARTICLES_H */
