#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_ORB_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_ORB_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/vendor/vendor_orb.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float vendor_orb_rotation_rate_degrees __asm__("D_L00_00161BB0");
extern float pterodactyl_path_speed __asm__("D_L00_00161C98");
extern float pterodactyl_scale_multiplier __asm__("D_L00_00161C9C");
extern f32   follow_camera_position_default_stiffness __asm__("D_L00_00161D28");
extern f32   follow_camera_position_default_damping __asm__("D_L00_00161D2C");
extern f32   follow_camera_position_transition_end_stiffness __asm__("D_L00_00161D30");
extern f32   follow_camera_position_transition_end_damping __asm__("D_L00_00161D34");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_ORB_GLOBALS_H */
