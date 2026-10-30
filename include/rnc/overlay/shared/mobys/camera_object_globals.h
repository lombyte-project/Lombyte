#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_CAMERA_OBJECT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_CAMERA_OBJECT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/mobys/camera_object.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   configured_unscaled_frame_count __asm__("D_L00_00161368");
extern int   configured_reciprocal_frame_count __asm__("D_L00_0016136C");
extern float wrapped_value_decrement_rate __asm__("D_L00_00161378");
extern int   last_bolt_attraction_sound_frame __asm__("D_L00_001613AC");
extern float bolt_spin_angle_limit_degrees __asm__("D_L00_001613A8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_CAMERA_OBJECT_GLOBALS_H */
