#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SENTRYBOT_DATA_SENTRYBOT_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SENTRYBOT_DATA_SENTRYBOT_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/shared/enemies/sentrybot.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int   sentrybot_alert_level_frame_stamp_gp __asm__("D_L15_001613E8") __attribute__((sda));
extern float sentrybot_emission_vector_speed_gp __asm__("D_L15_00161410") __attribute__((sda));
extern int   sentrybot_update_disabled_gp __asm__("D_L15_00161414") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SENTRYBOT_DATA_SENTRYBOT_UPDATE_H */
