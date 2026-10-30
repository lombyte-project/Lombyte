#ifndef LOMBYTE_RNC_OVERLAY_L17_MOBYS_FIGHTER_JET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L17_MOBYS_FIGHTER_JET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l17/mobys/fighter_jet.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float fighter_jet_boarding_distance_limit __asm__("D_L17_00161FE0");
extern float fighter_jet_initial_flight_speed __asm__("D_L17_00161FE4");
extern float fighter_jet_starting_hp __asm__("D_L17_00162058");
extern float fighter_jet_max_hp __asm__("D_L17_00162084");

#endif /* LOMBYTE_RNC_OVERLAY_L17_MOBYS_FIGHTER_JET_GLOBALS_H */
