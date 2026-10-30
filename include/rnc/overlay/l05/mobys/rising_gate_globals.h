#ifndef LOMBYTE_RNC_OVERLAY_L05_MOBYS_RISING_GATE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L05_MOBYS_RISING_GATE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l05/mobys/rising_gate.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float gate_rise_acceleration __asm__("D_L05_00161E80");
extern float gate_rise_deceleration __asm__("D_L05_00161E84");
extern float gate_rise_max_speed __asm__("D_L05_00161E88");
extern float gate_swing_acceleration __asm__("D_L05_00161E8C");
extern float gate_swing_deceleration __asm__("D_L05_00161E90");
extern float gate_swing_max_speed __asm__("D_L05_00161E94");

#endif /* LOMBYTE_RNC_OVERLAY_L05_MOBYS_RISING_GATE_GLOBALS_H */
