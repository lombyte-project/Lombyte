#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_PLATFORM_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_PLATFORM_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l12/mobys/platform.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float platform_tilt_approach_divisor __asm__("D_L12_00161918");
extern float platform_tilt_offset_divisor __asm__("D_L12_00161914");

#endif /* LOMBYTE_RNC_OVERLAY_L12_MOBYS_PLATFORM_GLOBALS_H */
