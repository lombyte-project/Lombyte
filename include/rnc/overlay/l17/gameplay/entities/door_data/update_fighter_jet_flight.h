#ifndef LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DOOR_DATA_UPDATE_FIGHTER_JET_FLIGHT_H
#define LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DOOR_DATA_UPDATE_FIGHTER_JET_FLIGHT_H

#include "types.h"

/* Scalar data for src/overlays/l17/gameplay/entities/door.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float fighter_jet_cruise_fov_gp __asm__("D_L17_00162144") __attribute__((sda));
extern float fighter_jet_boost_fov_gp __asm__("D_L17_00162148") __attribute__((sda));
extern float fighter_jet_fov_error_gain_gp __asm__("D_L17_0016214C") __attribute__((sda));
extern float fighter_jet_fov_velocity_damping_gp __asm__("D_L17_00162150") __attribute__((sda));
extern float fighter_jet_fov_settle_threshold_scale_gp __asm__("D_L17_00162154") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DOOR_DATA_UPDATE_FIGHTER_JET_FLIGHT_H */
