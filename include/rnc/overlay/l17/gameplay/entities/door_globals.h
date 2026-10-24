#ifndef LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DOOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DOOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l17/gameplay/entities/door.c. */
extern float fighter_jet_boarding_distance_limit __asm__("D_L17_00161FE0") __attribute__((sda));
extern float fighter_jet_initial_flight_speed __asm__("D_L17_00161FE4") __attribute__((sda));
extern float fighter_jet_starting_hp __asm__("D_L17_00162058") __attribute__((sda));
extern float fighter_jet_max_hp __asm__("D_L17_00162084") __attribute__((sda));
extern float ripple_pair_radial_phase_distance_scale __asm__("D_L17_00162254") __attribute__((sda));
extern float ripple_pair_phase_period __asm__("D_L17_00162258") __attribute__((sda));
extern float ripple_pair_z_amplitude __asm__("D_L17_0016225C") __attribute__((sda));
extern float ripple_pair_uv_scroll_rate __asm__("D_L17_00162260") __attribute__((sda));
extern float ripple_uv_scroll_rate __asm__("D_L17_0016238C") __attribute__((sda));
extern float ripple_radial_phase_distance_scale __asm__("D_L17_00162380") __attribute__((sda));
extern float ripple_phase_period __asm__("D_L17_00162384") __attribute__((sda));
extern float ripple_z_amplitude __asm__("D_L17_00162388") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L17_GAMEPLAY_ENTITIES_DOOR_GLOBALS_H */
