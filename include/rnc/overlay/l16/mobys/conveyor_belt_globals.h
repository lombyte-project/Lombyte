#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_CONVEYOR_BELT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_CONVEYOR_BELT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l16/mobys/conveyor_belt.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float conveyor_belt_base_speed __asm__("D_L16_0016198C");
extern int   conveyor_belt_pulse_texture_id __asm__("D_L16_00161990");
extern int   conveyor_belt_scrolling_texture_id __asm__("D_L16_00161994");
extern int   conveyor_belt_pulse_color_start __asm__("D_L16_001619A8");
extern int   conveyor_belt_pulse_color_end __asm__("D_L16_001619AC");
extern int   conveyor_belt_scrolling_vertex_color __asm__("D_L16_001619B0");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_CONVEYOR_BELT_GLOBALS_H */
