#ifndef LOMBYTE_RNC_OVERLAY_L14_PATH_KEY_CAMERA_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_PATH_KEY_CAMERA_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/path_key_camera.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float path_key_camera_heading_offset_spring_gain __asm__("D_L14_00162490")
    __attribute__((sda));
extern float path_key_camera_placement_elevation_spring_gain __asm__("D_L14_00162494")
    __attribute__((sda));
extern float path_key_camera_placement_distance_spring_gain __asm__("D_L14_00162498")
    __attribute__((sda));
extern float path_key_camera_lateral_axis_rotation_spring_gain __asm__("D_L14_0016249C")
    __attribute__((sda));
extern float path_key_camera_up_axis_rotation_spring_gain __asm__("D_L14_001624A0")
    __attribute__((sda));
extern float path_key_camera_heading_blend_step_factor __asm__("D_L14_001624A4")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L14_PATH_KEY_CAMERA_GLOBALS_H */
