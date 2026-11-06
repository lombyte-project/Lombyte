#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_H

#include "types.h"

struct UpdateCam;

s32  volume_camera_check(struct UpdateCam *) __asm__("FUN_L00_002ed8b0");
int  evaluate_clip_volume_candidate(char *, char *) __asm__("FUN_L03_002e8870");
void set_transform_state_three_when_outside_clip_volume(char *) __asm__("FUN_L03_002e8980");
void try_select_camera_offset_effect_moby(char *) __asm__("FUN_L02_002fa7c8");
void set_follow_camera_orbit_clamp_mode(int) __asm__("FUN_L00_002e8568");
void set_follow_camera_blend_output_flags(void) __asm__("FUN_L00_002e8680");
void lock_follow_camera_run_toward(void) __asm__("FUN_L00_002e8620");
void set_follow_camera_pull_back_max(float) __asm__("FUN_L00_002e8490");
void follow_camera_setup() __asm__("FUN_L00_002e6d60");
void reset_follow_camera_state_from_position(char *) __asm__("FUN_L00_002e6bf8");
void follow_camera_auto_turn(int) __asm__("FUN_L00_002e8f40");
void follow_camera_update(u8 *) __asm__("FUN_L00_002e9c28");
void initialize_camera_transform_and_paths() __asm__("FUN_L01_00315de0");
void update_camera_mode_from_preset() __asm__("FUN_L01_00316030");
void update_camera_position_and_orientation_from_paths() __asm__("FUN_L01_00315358");
void initialize_hero_aligned_camera_mode(void) __asm__("FUN_L00_002ea7a0");
int  camera_transition_update(u8 *, char *) __asm__("FUN_L00_002ea488");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void update_hero_aligned_camera_from_input(u8 *) __asm__("FUN_L00_002e9f38");
void reset_camera_from_source_transform(char *) __asm__("FUN_L05_0032a6e0");
int  update_moby_camera_control(char *) __asm__("FUN_L02_002fc298");
void release_moby_camera_control(char *) __asm__("FUN_L06_00316330");
void initialize_orbit_camera_from_hero() __asm__("FUN_L00_002ebb00");
void register_camera_update_callback(int) __asm__("FUN_L02_001fc008");
void conditionally_set_camera_state_three(char *) __asm__("FUN_L00_002ec7b0");
void initialize_target_tracking_camera(void *) __asm__("FUN_L00_002ebbd8");
void update_target_tracking_camera(char *) __asm__("FUN_L00_002ec6c0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_H */
