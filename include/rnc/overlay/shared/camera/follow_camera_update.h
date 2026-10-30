#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_UPDATE_H

#include "types.h"

void jet_mount(void *, void *, s32, s32, s32) __asm__("FUN_L00_002eaaa0");
void jet_end_mount(s32) __asm__("FUN_L00_002eac18");
void set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
void initialize_target_tracking_camera(void *) __asm__("FUN_L00_002ebbd8");
s32  camera_transition_update(u8 *, s8 *) __asm__("FUN_L00_002ea488");
void conditionally_set_camera_state_three(s8 *) __asm__("FUN_L00_002ec7b0");
void follow_camera_update(u8 *) __asm__("FUN_L00_002e9c28");
void initialize_camera_pose_springs(s8 *) __asm__("FUN_L00_002ead60");
void initialize_orbit_camera_from_hero(void *) __asm__("FUN_L00_002ebb00");
void select_camera_mode_by_hero_vector_alignment(void *) __asm__("FUN_L00_002ea810");
void update_camera_pose_with_optional_collision(s8 *) __asm__("FUN_L00_002eb218");
void update_hero_aligned_camera_from_input(u8 *) __asm__("FUN_L00_002e9f38");
void update_target_tracking_camera(s8 *) __asm__("FUN_L00_002ec6c0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_FOLLOW_CAMERA_UPDATE_H */
