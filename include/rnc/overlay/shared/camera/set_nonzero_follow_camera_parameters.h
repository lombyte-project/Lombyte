#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SET_NONZERO_FOLLOW_CAMERA_PARAMETERS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SET_NONZERO_FOLLOW_CAMERA_PARAMETERS_H

#include "types.h"

void set_follow_camera_control_word(int value) __asm__("FUN_L02_002f6570");
void set_nonzero_follow_camera_parameters(float x, float y, float z) __asm__("FUN_L02_002f6598");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SET_NONZERO_FOLLOW_CAMERA_PARAMETERS_H */
