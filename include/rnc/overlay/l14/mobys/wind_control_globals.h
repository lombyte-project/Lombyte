#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_WIND_CONTROL_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_WIND_CONTROL_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/mobys/wind_control.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32   wind_effect_strength __asm__("D_L14_00161C08");
extern s32   wind_push_decay_frames __asm__("D_L14_00161C04");
extern int   wind_spark_speed_range_start __asm__("D_L14_00161C30");
extern int   wind_spark_speed_range_end __asm__("D_L14_00161C34");
extern float wind_spark_size_range_start __asm__("D_L14_00161C38");
extern float wind_spark_size_range_end __asm__("D_L14_00161C3C");
extern int   wind_spark_peak_alpha __asm__("D_L14_00161C10");
extern int   wind_spark_fade_in_frames __asm__("D_L14_00161C2C");
extern int   wind_spark_base_color __asm__("D_L14_00161C0C");

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_WIND_CONTROL_GLOBALS_H */
