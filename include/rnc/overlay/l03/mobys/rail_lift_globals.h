#ifndef LOMBYTE_RNC_OVERLAY_L03_MOBYS_RAIL_LIFT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L03_MOBYS_RAIL_LIFT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l03/mobys/rail_lift.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float moby_vertical_oscillation_amplitude __asm__("D_L03_00161BD8");
extern float moby_vertical_oscillation_phase_rate __asm__("D_L03_00161BDC");
extern float moby_rotation_oscillation_amplitude __asm__("D_L03_00161BE0");
extern float moby_rotation_oscillation_first_phase_rate __asm__("D_L03_00161BE4");
extern float moby_rotation_oscillation_second_phase_rate __asm__("D_L03_00161BE8");

#endif /* LOMBYTE_RNC_OVERLAY_L03_MOBYS_RAIL_LIFT_GLOBALS_H */
