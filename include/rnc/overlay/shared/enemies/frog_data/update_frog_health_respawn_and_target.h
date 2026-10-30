#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_FROG_DATA_UPDATE_FROG_HEALTH_RESPAWN_AND_TARGET_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_FROG_DATA_UPDATE_FROG_HEALTH_RESPAWN_AND_TARGET_H

#include "types.h"

/* Scalar data for src/overlays/shared/enemies/frog.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 frog_glow_phase_rate_degrees_gp __asm__("D_L06_00161C04") __attribute__((sda));
extern int frog_glow_color_start_gp __asm__("D_L06_00161C08") __attribute__((sda));
extern int frog_glow_color_end_gp __asm__("D_L06_00161C0C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_FROG_DATA_UPDATE_FROG_HEALTH_RESPAWN_AND_TARGET_H */
