#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_PLASMABOT_DATA_UPDATE_PLASMABOT_VISUAL_EFFECTS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_PLASMABOT_DATA_UPDATE_PLASMABOT_VISUAL_EFFECTS_H

#include "types.h"

/* Scalar data for src/overlays/shared/enemies/plasmabot.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 plasmabot_nozzle_bone_x_offset_gp __asm__("D_L14_001614A8") __attribute__((sda));
extern f32 plasmabot_beam_bone_x_offset_gp __asm__("D_L14_001614B0") __attribute__((sda));
extern f32 plasmabot_beam_endpoint_bone_y_offset_gp __asm__("D_L14_001614B4") __attribute__((sda));
extern f32 plasmabot_beam_bone_z_offset_gp __asm__("D_L14_001614B8") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_PLASMABOT_DATA_UPDATE_PLASMABOT_VISUAL_EFFECTS_H */
