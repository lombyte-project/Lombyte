#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_TURN_AND_PITCH_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_TURN_AND_PITCH_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/camera/follow_camera_turn_and_pitch.c. */
extern float follow_camera_input_stiffness __asm__("D_L00_00161CAC") __attribute__((sda));
extern float follow_camera_input_damping __asm__("D_L00_00161CB0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_TURN_AND_PITCH_GLOBALS_H */
