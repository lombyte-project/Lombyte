#ifndef LOMBYTE_RNC_OVERLAY_L11_MOBYS_JET_FIGHTER_DATA_UPDATE_PLAYER_SHIP_FLIGHT_14_H
#define LOMBYTE_RNC_OVERLAY_L11_MOBYS_JET_FIGHTER_DATA_UPDATE_PLAYER_SHIP_FLIGHT_14_H

#include "types.h"

/* Scalar data for src/overlays/l11/mobys/jet_fighter.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float jet_fighter_boost_speed_limit_gp __asm__("D_L11_00161FF8") __attribute__((sda));
extern float jet_fighter_boost_speed_increment_gp __asm__("D_L11_00161FFC") __attribute__((sda));
extern float jet_fighter_cruise_speed_relaxation_factor_gp __asm__("D_L11_00162000")
    __attribute__((sda));
extern float jet_fighter_yaw_deflection_scale_gp __asm__("D_L11_00162008") __attribute__((sda));
extern float jet_fighter_pitch_deflection_scale_gp __asm__("D_L11_0016200C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L11_MOBYS_JET_FIGHTER_DATA_UPDATE_PLAYER_SHIP_FLIGHT_14_H */
