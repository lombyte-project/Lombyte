#ifndef LOMBYTE_RNC_OVERLAY_L01_MOBYS_BURNING_DEBRIS_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L01_MOBYS_BURNING_DEBRIS_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l01/mobys/burning_debris.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32 burning_debris_gravity_coefficient __asm__("D_L01_00162058");
extern f32 burning_debris_velocity_multiplier __asm__("D_L01_0016205C");
extern f32 burning_debris_horizontal_damping_threshold __asm__("D_L01_00162060");

#endif /* LOMBYTE_RNC_OVERLAY_L01_MOBYS_BURNING_DEBRIS_GLOBALS_H */
