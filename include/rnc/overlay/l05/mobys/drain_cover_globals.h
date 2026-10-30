#ifndef LOMBYTE_RNC_OVERLAY_L05_MOBYS_DRAIN_COVER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L05_MOBYS_DRAIN_COVER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l05/mobys/drain_cover.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float drain_cover_tilt_acceleration __asm__("D_L05_00161DDC");
extern float drain_cover_tilt_deceleration __asm__("D_L05_00161DE0");
extern float drain_cover_tilt_max_speed __asm__("D_L05_00161DE4");

#endif /* LOMBYTE_RNC_OVERLAY_L05_MOBYS_DRAIN_COVER_GLOBALS_H */
