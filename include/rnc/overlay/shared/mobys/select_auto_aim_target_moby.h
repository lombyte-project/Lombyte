#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_SELECT_AUTO_AIM_TARGET_MOBY_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_SELECT_AUTO_AIM_TARGET_MOBY_H

#include "types.h"

f32  ground_height_at(void *) __asm__("FUN_L00_00263468");
void spawn_ground_contact_effect_particle(void *) __asm__("FUN_L00_002634f8");
void cutscene_set_camera_target(void *) __asm__("FUN_L00_002637f8");
void manipulator_set_scale(void *, f32) __asm__("FUN_L00_00263b38");
void draw_trail(void *, void *, void *, f32, f32) __asm__("FUN_L00_00263b70");
void queue_dialogue_message(int a, int b) __asm__("FUN_L00_00263d40");
void spawn_moby_break_fragments(void *, s32, s32, s32, s32, s32, s32) __asm__("FUN_L00_00263e30");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_SELECT_AUTO_AIM_TARGET_MOBY_H */
