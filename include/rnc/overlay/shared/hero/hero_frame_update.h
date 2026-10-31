#ifndef LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_FRAME_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_FRAME_UPDATE_H

#include "types.h"

s32  get_hero_idle_animation_index(s32) __asm__("FUN_L01_00226f10");
void request_hero_idle_state_for_vehicle_mode(void) __asm__("FUN_L01_002405a0");

void enter_hero_vehicle_mode_and_set_moby(int, int, int) __asm__("FUN_L01_00231348");
void hero_frame_update(void) __asm__("FUN_L01_00228000");
void update_hero_blink_and_gold_weapon_tint(void) __asm__("FUN_L01_00228e38");
void update_hero_ground_material_effects(void) __asm__("FUN_L01_0022cd48");
void update_hero_ledge_probe(void) __asm__("FUN_L01_0022d090");
void exit_hero_vehicle_mode_and_restore_moby(void) __asm__("FUN_L01_00231450");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_FRAME_UPDATE_H */
