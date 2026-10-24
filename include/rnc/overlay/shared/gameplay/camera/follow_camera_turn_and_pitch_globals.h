#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_TURN_AND_PITCH_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_TURN_AND_PITCH_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/camera/follow_camera_turn_and_pitch.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float follow_camera_input_stiffness __asm__("D_L00_00161CAC");
extern float follow_camera_input_damping __asm__("D_L00_00161CB0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_CAMERA_FOLLOW_CAMERA_TURN_AND_PITCH_GLOBALS_H */
