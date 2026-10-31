#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_CUTSCENE_EFFECTS_DATA_UPDATE_DEPLANETIZER_GUN_SEQUENCE_EFFECTS_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_CUTSCENE_EFFECTS_DATA_UPDATE_DEPLANETIZER_GUN_SEQUENCE_EFFECTS_H

#include "types.h"

/* Scalar data for src/overlays/l18/mobys/cutscene_effects.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float deplanetizer_burst_local_spawn_offset_x_gp __asm__("D_L18_001624E0")
    __attribute__((sda));
extern float deplanetizer_burst_velocity_scale_gp __asm__("D_L18_001624F0") __attribute__((sda));
extern float deplanetizer_burst_particle_size_gp __asm__("D_L18_001624F4") __attribute__((sda));
extern float beam_effect_default_length __asm__("D_L18_0016174C");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_CUTSCENE_EFFECTS_DATA_UPDATE_DEPLANETIZER_GUN_SEQUENCE_EFFECTS_H */
