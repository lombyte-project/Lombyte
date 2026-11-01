#ifndef LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_MOVE_STATE_VELOCITY_H
#define LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_MOVE_STATE_VELOCITY_H

#include "types.h"

int  allocate_hero_voice_for_entry(int a, int b) __asm__("FUN_L00_00216de8");
void help_queue_add(s32, s32, s32) __asm__("FUN_L00_00216ec0");

void update_hero_transformed_negative_z_angle_parameters(void) __asm__("FUN_L00_00214fe8");
void update_hero_delayed_voice_requests(void) __asm__("FUN_L00_00216f10");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_MOVE_STATE_VELOCITY_H */
