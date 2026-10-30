#ifndef LOMBYTE_RNC_OVERLAY_SHARED_HERO_AIR_ACCEL_H
#define LOMBYTE_RNC_OVERLAY_SHARED_HERO_AIR_ACCEL_H

#include "types.h"

s32  get_hero_idle_animation_index(s32) __asm__("FUN_L01_00226f10");
void request_hero_idle_state_for_vehicle_mode(void) __asm__("FUN_L01_002405a0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_HERO_AIR_ACCEL_H */
