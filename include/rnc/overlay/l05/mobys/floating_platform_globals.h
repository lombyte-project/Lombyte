#ifndef LOMBYTE_RNC_OVERLAY_L05_MOBYS_FLOATING_PLATFORM_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L05_MOBYS_FLOATING_PLATFORM_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l05/mobys/floating_platform.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float floating_platform_rotation_y_phase_rate_degrees __asm__("D_L05_00161CD4");
extern float floating_platform_rotation_x_phase_rate_degrees __asm__("D_L05_00161CD8");
extern float floating_platform_rotation_y_amplitude_degrees __asm__("D_L05_00161CDC");
extern float floating_platform_rotation_x_amplitude_degrees __asm__("D_L05_00161CE0");
extern int   floating_platform_voice_interval_min __asm__("D_L05_00161CE4");
extern int   floating_platform_voice_interval_max __asm__("D_L05_00161CE8");

#endif /* LOMBYTE_RNC_OVERLAY_L05_MOBYS_FLOATING_PLATFORM_GLOBALS_H */
