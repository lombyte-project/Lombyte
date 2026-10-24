#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VENDOR_ORB_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VENDOR_ORB_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/vendor_orb.c. */
extern float vendor_orb_rotation_rate_degrees __asm__("D_L00_00161BB0") __attribute__((sda));
extern float pterodactyl_path_speed __asm__("D_L00_00161C98") __attribute__((sda));
extern float pterodactyl_scale_multiplier __asm__("D_L00_00161C9C") __attribute__((sda));
extern f32 follow_camera_position_default_stiffness __asm__("D_L00_00161D28") __attribute__((sda));
extern f32 follow_camera_position_default_damping __asm__("D_L00_00161D2C") __attribute__((sda));
extern f32 follow_camera_position_transition_end_stiffness __asm__("D_L00_00161D30") __attribute__((sda));
extern f32 follow_camera_position_transition_end_damping __asm__("D_L00_00161D34") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VENDOR_ORB_GLOBALS_H */
