#ifndef LOMBYTE_RNC_OVERLAY_L10_MOBYS_DIRECTIONAL_PARTICLE_EMITTER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L10_MOBYS_DIRECTIONAL_PARTICLE_EMITTER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l10/mobys/directional_particle_emitter.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern s32 directional_particle_color_range_start __asm__("D_L10_00161EF0");
extern s32 directional_particle_color_range_end __asm__("D_L10_00161EF4");
extern f32 directional_particle_speed_range_end __asm__("D_L10_00161EF8");
extern f32 directional_particle_speed_range_start __asm__("D_L10_00161EFC");
extern s32 directional_particle_variant_threshold_percent __asm__("D_L10_00161F04");

#endif /* LOMBYTE_RNC_OVERLAY_L10_MOBYS_DIRECTIONAL_PARTICLE_EMITTER_GLOBALS_H */
