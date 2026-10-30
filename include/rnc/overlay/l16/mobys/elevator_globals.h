#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_ELEVATOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_ELEVATOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l16/mobys/elevator.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float elevator_ring_outer_radius __asm__("D_L16_00161EC4");
extern float elevator_ring_half_height __asm__("D_L16_00161EC8");
extern float elevator_ring_segment_angle_degrees __asm__("D_L16_00161EE8");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_ELEVATOR_GLOBALS_H */
