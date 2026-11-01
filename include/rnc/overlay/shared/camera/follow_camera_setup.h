#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_H

#include "types.h"

void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void reset_follow_camera_state_from_position(char *) __asm__("FUN_L00_002e6bf8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SETUP_H */
