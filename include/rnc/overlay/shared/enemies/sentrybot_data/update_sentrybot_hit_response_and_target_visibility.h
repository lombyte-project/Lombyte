#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SENTRYBOT_DATA_UPDATE_SENTRYBOT_HIT_RESPONSE_AND_TARGET_VISIBILITY_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SENTRYBOT_DATA_UPDATE_SENTRYBOT_HIT_RESPONSE_AND_TARGET_VISIBILITY_H

#include "types.h"

/* Scalar data for src/overlays/shared/enemies/sentrybot.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float sentrybot_knockback_gravity_gp __asm__("D_L15_001613F0") __attribute__((sda));
extern float sentrybot_death_knockback_gravity_gp __asm__("D_L15_001613F4") __attribute__((sda));
extern float sentrybot_recoverable_knockback_vertical_speed_gp __asm__("D_L15_001613F8")
    __attribute__((sda));
extern float sentrybot_recoverable_knockback_horizontal_speed_gp __asm__("D_L15_001613FC")
    __attribute__((sda));
extern float
    sentrybot_recoverable_knockback_horizontal_speed_approach_rate_gp __asm__("D_L15_00161400")
        __attribute__((sda));
extern float sentrybot_death_knockback_vertical_speed_gp __asm__("D_L15_00161404")
    __attribute__((sda));
extern float sentrybot_death_knockback_horizontal_speed_gp __asm__("D_L15_00161408")
    __attribute__((sda));
extern float sentrybot_death_knockback_horizontal_speed_approach_rate_gp __asm__("D_L15_0016140C")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_SENTRYBOT_DATA_UPDATE_SENTRYBOT_HIT_RESPONSE_AND_TARGET_VISIBILITY_H */
