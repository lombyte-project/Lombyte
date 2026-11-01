#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SCRIPTED_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SCRIPTED_CAMERA_H

#include "types.h"

void set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
void jet_mount(void *, void *, s32, s32, s32) __asm__("FUN_L00_002eaaa0");
void jet_end_mount(s32) __asm__("FUN_L00_002eac18");

void initialize_camera_pose_springs(char *) __asm__("FUN_L00_002ead60");
void update_camera_pose_with_optional_collision(char *) __asm__("FUN_L00_002eb218");
void set_camera_5_track_acceleration_yz(float, float) __asm__("FUN_L01_00316e88");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SCRIPTED_CAMERA_H */
