#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_FLOATING_PLATFORM_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_FLOATING_PLATFORM_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l18/mobys/floating_platform.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float floating_platform_bob_amplitude __asm__("D_L18_00161A48");
extern float floating_platform_bob_phase_rate __asm__("D_L18_00161A4C");
extern float floating_platform_sink_trigger_distance __asm__("D_L18_00161A5C");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_FLOATING_PLATFORM_GLOBALS_H */
