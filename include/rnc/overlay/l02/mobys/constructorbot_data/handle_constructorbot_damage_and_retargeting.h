#ifndef LOMBYTE_RNC_OVERLAY_L02_MOBYS_CONSTRUCTORBOT_DATA_HANDLE_CONSTRUCTORBOT_DAMAGE_AND_RETARGETING_H
#define LOMBYTE_RNC_OVERLAY_L02_MOBYS_CONSTRUCTORBOT_DATA_HANDLE_CONSTRUCTORBOT_DAMAGE_AND_RETARGETING_H

#include "types.h"

/* Scalar data for src/overlays/l02/mobys/constructorbot.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float
    constructorbot_death_knockback_horizontal_speed_approach_rate_gp __asm__("D_L02_00161A74")
        __attribute__((sda));
extern float
    constructorbot_hit_knockback_horizontal_speed_approach_rate_gp __asm__("D_L02_00161A78")
        __attribute__((sda));
extern float constructorbot_knockback_horizontal_target_speed_gp __asm__("D_L02_00161A9C")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L02_MOBYS_CONSTRUCTORBOT_DATA_HANDLE_CONSTRUCTORBOT_DAMAGE_AND_RETARGETING_H */
