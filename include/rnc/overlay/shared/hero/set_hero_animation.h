#ifndef LOMBYTE_RNC_OVERLAY_SHARED_HERO_SET_HERO_ANIMATION_H
#define LOMBYTE_RNC_OVERLAY_SHARED_HERO_SET_HERO_ANIMATION_H

#include "types.h"

void  set_hero_animation(int   requested_animation_id,
                         int   starting_frame_index,
                         float blend_frames_or_selector) __asm__("FUN_L00_002323b8");
void  gravity_set_z(void *out, void *in, f32 z) __asm__("FUN_L00_00233b20");
float gravity_z(void *v) __asm__("FUN_L00_00233a78");
void  show_health_hud(void) __asm__("FUN_L00_00234e00");

s32  get_ammo(s32) __asm__("FUN_L00_00233e98");
s32  get_hero_target_pose_status(void) __asm__("FUN_L00_00233f38");
void FUN_L00_00232fe8(void);
void FUN_L00_002330d0(void);
s32  use_ammo(s32, s32) __asm__("FUN_L00_00233db8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_HERO_SET_HERO_ANIMATION_H */
