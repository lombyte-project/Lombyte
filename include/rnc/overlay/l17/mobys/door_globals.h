#ifndef LOMBYTE_RNC_OVERLAY_L17_MOBYS_DOOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L17_MOBYS_DOOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l17/mobys/door.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float fighter_jet_boarding_distance_limit __asm__("D_L17_00161FE0");
extern float fighter_jet_initial_flight_speed __asm__("D_L17_00161FE4");
extern float fighter_jet_starting_hp __asm__("D_L17_00162058");
extern float fighter_jet_max_hp __asm__("D_L17_00162084");
extern float ripple_pair_radial_phase_distance_scale __asm__("D_L17_00162254");
extern float ripple_pair_phase_period __asm__("D_L17_00162258");
extern float ripple_pair_z_amplitude __asm__("D_L17_0016225C");
extern float ripple_pair_uv_scroll_rate __asm__("D_L17_00162260");
extern float ripple_uv_scroll_rate __asm__("D_L17_0016238C");
extern float ripple_radial_phase_distance_scale __asm__("D_L17_00162380");
extern float ripple_phase_period __asm__("D_L17_00162384");
extern float ripple_z_amplitude __asm__("D_L17_00162388");

#endif /* LOMBYTE_RNC_OVERLAY_L17_MOBYS_DOOR_GLOBALS_H */
