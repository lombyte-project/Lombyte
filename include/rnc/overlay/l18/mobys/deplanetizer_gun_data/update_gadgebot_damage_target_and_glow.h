#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_DEPLANETIZER_GUN_DATA_UPDATE_GADGEBOT_DAMAGE_TARGET_AND_GLOW_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_DEPLANETIZER_GUN_DATA_UPDATE_GADGEBOT_DAMAGE_TARGET_AND_GLOW_H

#include "types.h"

/* Scalar data for src/overlays/l18/mobys/deplanetizer_gun.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float gadgebot_death_fall_gravity_gp __asm__("D_L18_0016261C") __attribute__((sda));
extern float gadgebot_death_fall_horizontal_speed_approach_rate_gp __asm__("D_L18_00162620")
    __attribute__((sda));
extern float gadgebot_death_fall_base_horizontal_speed_gp __asm__("D_L18_00162624")
    __attribute__((sda));
extern float gadgebot_death_fall_base_vertical_speed_gp __asm__("D_L18_00162628")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_DEPLANETIZER_GUN_DATA_UPDATE_GADGEBOT_DAMAGE_TARGET_AND_GLOW_H */
