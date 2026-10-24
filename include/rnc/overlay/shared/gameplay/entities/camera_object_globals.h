#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_CAMERA_OBJECT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_CAMERA_OBJECT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/camera_object.c. */
extern int configured_unscaled_frame_count __asm__("D_L00_00161368") __attribute__((sda));
extern int configured_reciprocal_frame_count __asm__("D_L00_0016136C") __attribute__((sda));
extern float wrapped_value_decrement_rate __asm__("D_L00_00161378") __attribute__((sda));
extern int last_bolt_attraction_sound_frame __asm__("D_L00_001613AC") __attribute__((sda));
extern float bolt_spin_angle_limit_degrees __asm__("D_L00_001613A8") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_CAMERA_OBJECT_GLOBALS_H */
