#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_UPDATE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_UPDATE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/camera/follow_camera_update.c. */
extern float orbit_distance_default_stiffness __asm__("D_L00_00161CEC") __attribute__((sda));
extern float orbit_distance_default_damping __asm__("D_L00_00161CF0") __attribute__((sda));
extern float orbit_distance_pull_back_stiffness __asm__("D_L00_00161D14") __attribute__((sda));
extern float orbit_distance_pull_back_damping __asm__("D_L00_00161D18") __attribute__((sda));
extern float hero_aligned_camera_rotation_gain_at_zero_input __asm__("D_L00_00161DC0") __attribute__((sda));
extern float hero_aligned_camera_rotation_gain_at_unit_input __asm__("D_L00_00161DC4") __attribute__((sda));
extern float hero_aligned_camera_input_stiffness __asm__("D_L00_00161DC8") __attribute__((sda));
extern float hero_aligned_camera_input_damping __asm__("D_L00_00161DCC") __attribute__((sda));
extern float hero_aligned_camera_input_step_limit __asm__("D_L00_00161DD0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_UPDATE_GLOBALS_H */
