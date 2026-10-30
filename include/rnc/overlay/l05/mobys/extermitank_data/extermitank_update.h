#ifndef LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMITANK_DATA_EXTERMITANK_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMITANK_DATA_EXTERMITANK_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/l05/mobys/extermitank.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float extermitank_secondary_particle_vector_b_z_scale_gp __asm__("D_L05_00161B5C")
    __attribute__((sda));
extern float extermitank_secondary_particle_vector_a_w_gp __asm__("D_L05_00161B60")
    __attribute__((sda));
extern float extermitank_secondary_particle_vector_b_w_gp __asm__("D_L05_00161B64")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMITANK_DATA_EXTERMITANK_UPDATE_H */
