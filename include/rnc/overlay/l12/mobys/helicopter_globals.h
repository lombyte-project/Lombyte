#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_HELICOPTER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_HELICOPTER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l12/mobys/helicopter.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   helicopter_help_message_hit_count __asm__("D_0015EE04");
extern float helicopter_path_speed __asm__("D_L12_001619A8");
extern float helicopter_rotation_smoothing_divisor __asm__("D_L12_001619B0");

#endif /* LOMBYTE_RNC_OVERLAY_L12_MOBYS_HELICOPTER_GLOBALS_H */
