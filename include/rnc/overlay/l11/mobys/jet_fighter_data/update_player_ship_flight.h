#ifndef LOMBYTE_RNC_OVERLAY_L11_MOBYS_JET_FIGHTER_DATA_UPDATE_PLAYER_SHIP_FLIGHT_H
#define LOMBYTE_RNC_OVERLAY_L11_MOBYS_JET_FIGHTER_DATA_UPDATE_PLAYER_SHIP_FLIGHT_H

#include "types.h"

/* Scalar data for src/overlays/l11/mobys/jet_fighter.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float jet_fighter_cruise_camera_fov_radians_gp __asm__("D_L11_00162134")
    __attribute__((sda));
extern float jet_fighter_boost_camera_fov_radians_gp __asm__("D_L11_00162138") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L11_MOBYS_JET_FIGHTER_DATA_UPDATE_PLAYER_SHIP_FLIGHT_H */
