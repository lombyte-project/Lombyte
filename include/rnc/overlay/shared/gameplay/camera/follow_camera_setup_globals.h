#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_SETUP_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_SETUP_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/camera/follow_camera_setup.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float follow_camera_orbit_distance_damping __asm__("D_L00_00161D04");
extern float follow_camera_orbit_height_damping __asm__("D_L00_00161D08");
extern f32 follow_camera_vehicle_offset_scale __asm__("D_L00_00161D98");
extern f32 follow_camera_vehicle_orbit_distance __asm__("D_L00_00161D9C");
extern f32 follow_camera_vehicle_orbit_height __asm__("D_L00_00161DA0");
extern float follow_camera_collision_retraction_rate __asm__("D_L00_00161CB8");
extern f32 follow_camera_minimum_retracted_orbit_distance __asm__("D_L00_00161CD0");

/* Additional scalar data read by this unit. */
extern s32 follow_camera_rotation_slide_enabled_gp __asm__("D_L00_00161D94") __attribute__((sda));
extern f32 follow_camera_rotation_slide_trigger_threshold_gp __asm__("D_L00_00161D84") __attribute__((sda));
extern f32 follow_camera_rotation_slide_lateral_spring_target_gp __asm__("D_L00_00161D88") __attribute__((sda));
extern f32 follow_camera_vehicle_collision_probe_step_growth_factor_gp __asm__("D_L00_00161DA4") __attribute__((sda));
extern f32 follow_camera_vehicle_collision_probe_scale_gp __asm__("D_L00_00161DA8") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_SETUP_GLOBALS_H */
