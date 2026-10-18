#ifndef LOMBYTE_RNC_RENDERING_SKY_PARTICLES_H
#define LOMBYTE_RNC_RENDERING_SKY_PARTICLES_H

#include "types.h"

/* Fill the particle list: randomized_count particles with a random color, then
 * orbiting_count particles that circle the sky. Sky programs call it while the
 * particle count is still zero. */
void sky_particles_init(s32 randomized_count, s32 orbiting_count) __asm__("FUN_L00_00288ec0");
/* Move the orbiting particles and refresh every particle color; called each frame. */
void sky_particles_update(void) __asm__("FUN_L00_00289108");

#endif
