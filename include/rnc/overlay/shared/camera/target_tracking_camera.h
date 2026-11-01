#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_TARGET_TRACKING_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_TARGET_TRACKING_CAMERA_H

#include "types.h"

void conditionally_set_camera_state_three(char *) __asm__("FUN_L00_002ec7b0");
void initialize_target_tracking_camera(void *) __asm__("FUN_L00_002ebbd8");
void update_target_tracking_camera(char *) __asm__("FUN_L00_002ec6c0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_TARGET_TRACKING_CAMERA_H */
