#ifndef LOMBYTE_RNC_OVERLAY_L02_MOBYS_ELEVATOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L02_MOBYS_ELEVATOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l02/mobys/elevator.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float level02_elevator_stop_height_offset __asm__("D_L02_00161BD0");
extern float level02_elevator_max_vertical_speed __asm__("D_L02_00161BDC");
extern float level02_elevator_vertical_acceleration __asm__("D_L02_00161BE0");
extern float level02_elevator_ring_outer_radius __asm__("D_L02_00161BEC");
extern float level02_elevator_ring_vertical_spacing __asm__("D_L02_00161BE8");
extern float level02_elevator_ring_segment_half_height __asm__("D_L02_00161BF0");
extern float level02_elevator_ring_segment_angle_degrees __asm__("D_L02_00161C10");

#endif /* LOMBYTE_RNC_OVERLAY_L02_MOBYS_ELEVATOR_GLOBALS_H */
