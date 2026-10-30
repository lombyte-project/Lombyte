#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_ORB_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_ORB_H

#include "types.h"

void hydrodisplacer_update(void *) __asm__("FUN_L00_002df9a0");
void pilots_helmet_update(void *) __asm__("FUN_L00_002e03a0");
void robot_arm_update(s8 *) __asm__("FUN_L00_002e3d88");
void vendor_orb_update(u8 *) __asm__("FUN_L00_002df730");
void set_follow_camera_angle_blend_from_position(void *, f32, void *) __asm__("FUN_L00_002e8970");
void pterodactyl_update(u8 *) __asm__("FUN_L00_002e3a88");
void red_plant_update(u8 *) __asm__("FUN_L00_002e3ca8");
void set_follow_camera_angle_blend_from_direction(void *, f32, void *) __asm__("FUN_L00_002e8918");
s32  register_beam_slot(void *, s32, s32) __asm__("FUN_L00_002e1f28");
s32  set_beam_origin_transform_and_endpoint(void *, void *, void *) __asm__("FUN_L00_002e2190");
void set_follow_camera_orbit_clamp_mode(s32) __asm__("FUN_L00_002e8568");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_ORB_H */
