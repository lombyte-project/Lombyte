#ifndef LOMBYTE_RNC_OVERLAY_L15_MOBYS_CONSTRUCTION_ARM_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L15_MOBYS_CONSTRUCTION_ARM_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l15/mobys/construction_arm.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   construction_arm_effect_duration_min_frames __asm__("D_L15_001619F4");
extern int   construction_arm_effect_duration_max_frames __asm__("D_L15_001619F8");
extern float construction_arm_effect_speed_max __asm__("D_L15_001619FC");
extern float construction_arm_wait_time_min __asm__("D_L15_00161A00");
extern float construction_arm_wait_time_max __asm__("D_L15_00161A04");

#endif /* LOMBYTE_RNC_OVERLAY_L15_MOBYS_CONSTRUCTION_ARM_GLOBALS_H */
