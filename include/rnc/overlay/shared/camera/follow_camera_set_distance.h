#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SET_DISTANCE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SET_DISTANCE_H

#include "types.h"

void set_follow_camera_orbit_clamp_mode(int) __asm__("FUN_L00_002e8568");
void set_follow_camera_blend_output_flags(void) __asm__("FUN_L00_002e8680");
void FUN_L00_002e8620(void);
void FUN_L00_002e8490(float);

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_SET_DISTANCE_H */
