#ifndef LOMBYTE_RNC_OVERLAY_L03_MOBYS_TAXI_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L03_MOBYS_TAXI_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l03/mobys/taxi.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float taxi_bobbing_amplitude __asm__("D_L03_00161AF0");
extern float taxi_bobbing_phase_rate_degrees __asm__("D_L03_00161AF4");
extern float taxi_rocking_amplitude __asm__("D_L03_00161AF8");
extern float taxi_rocking_first_phase_rate_degrees __asm__("D_L03_00161AFC");
extern float taxi_rocking_second_phase_rate_degrees __asm__("D_L03_00161B00");
extern s32   taxi_path_point_debug_draw_enabled __asm__("D_L03_00161B04");
extern f32   rail_flyer_max_speed __asm__("D_L03_00161B08");
extern f32   rail_flyer_pitch_acceleration_degrees __asm__("D_L03_00161B10");
extern f32   rail_flyer_roll_acceleration_degrees __asm__("D_L03_00161B18");
extern f32   rail_flyer_speed_acceleration __asm__("D_L03_00161B0C");
extern f32   rail_flyer_yaw_acceleration_degrees __asm__("D_L03_00161B14");

#endif /* LOMBYTE_RNC_OVERLAY_L03_MOBYS_TAXI_GLOBALS_H */
