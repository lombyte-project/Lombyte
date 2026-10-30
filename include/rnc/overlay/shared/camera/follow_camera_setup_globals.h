#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/camera/follow_camera_setup.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float follow_camera_orbit_distance_damping __asm__("D_L00_00161D04");
extern float follow_camera_orbit_height_damping __asm__("D_L00_00161D08");
extern f32   follow_camera_vehicle_offset_scale __asm__("D_L00_00161D98");
extern f32   follow_camera_vehicle_orbit_distance __asm__("D_L00_00161D9C");
extern f32   follow_camera_vehicle_orbit_height __asm__("D_L00_00161DA0");
extern float follow_camera_collision_retraction_rate __asm__("D_L00_00161CB8");
extern f32   follow_camera_minimum_retracted_orbit_distance __asm__("D_L00_00161CD0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_GLOBALS_H */
