#ifndef LOMBYTE_RNC_OVERLAY_L17_MOBYS_FIGHTER_JET_DATA_UPDATE_FIGHTER_JET_FLIGHT_7_H
#define LOMBYTE_RNC_OVERLAY_L17_MOBYS_FIGHTER_JET_DATA_UPDATE_FIGHTER_JET_FLIGHT_7_H

#include "types.h"

/* Scalar data for src/overlays/l17/mobys/fighter_jet.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float fighter_jet_cruise_speed_gp __asm__("D_L17_00162004") __attribute__((sda));
extern float fighter_jet_boost_speed_limit_gp __asm__("D_L17_00162008") __attribute__((sda));
extern float fighter_jet_boost_speed_step_gp __asm__("D_L17_0016200C") __attribute__((sda));
extern float fighter_jet_cruise_speed_return_factor_gp __asm__("D_L17_00162010")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L17_MOBYS_FIGHTER_JET_DATA_UPDATE_FIGHTER_JET_FLIGHT_7_H */
