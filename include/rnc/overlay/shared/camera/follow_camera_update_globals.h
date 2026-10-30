#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_UPDATE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_UPDATE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/camera/follow_camera_update.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float orbit_distance_default_stiffness __asm__("D_L00_00161CEC");
extern float orbit_distance_default_damping __asm__("D_L00_00161CF0");
extern float orbit_distance_pull_back_stiffness __asm__("D_L00_00161D14");
extern float orbit_distance_pull_back_damping __asm__("D_L00_00161D18");
extern float hero_aligned_camera_rotation_gain_at_zero_input __asm__("D_L00_00161DC0");
extern float hero_aligned_camera_rotation_gain_at_unit_input __asm__("D_L00_00161DC4");
extern float hero_aligned_camera_input_stiffness __asm__("D_L00_00161DC8");
extern float hero_aligned_camera_input_damping __asm__("D_L00_00161DCC");
extern float hero_aligned_camera_input_step_limit __asm__("D_L00_00161DD0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_UPDATE_GLOBALS_H */
