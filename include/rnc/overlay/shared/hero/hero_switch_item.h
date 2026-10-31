#ifndef LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_SWITCH_ITEM_H
#define LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_SWITCH_ITEM_H

#include "types.h"

void stick_target(float scale, int mode) __asm__("FUN_L00_002118c8");
int  hero_slot_item_id(int i) __asm__("FUN_L00_0020d498");
void cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
void hero_respawn(void) __asm__("FUN_L00_00211250");

void exit_hero_vehicle_mode(void) __asm__("FUN_L00_00210b30");
s32  hero_is_in_special_state(void) __asm__("FUN_L00_0020d568");
void update_hero_ground_contact_and_surface_slopes(void) __asm__("FUN_L00_002127b8");
void clear_hero_target_and_ground_speeds(void) __asm__("FUN_L00_002118b0");
void update_hero_item_mobys(void) __asm__("FUN_L00_0020f580");
void update_hero_offset_rail_candidate(void) __asm__("FUN_L00_0020d330");
void update_hero_equipped_items(void) __asm__("FUN_L00_00210928");
void hero_update(void) __asm__("FUN_L00_00211670");
void hero_get_collision_ray_endpoints(void *, void *, s32, f32, f32) __asm__("FUN_L00_002126b8");
void clear_hero_item_slot(int, int) __asm__("FUN_L00_0020fca8");
void start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_SWITCH_ITEM_H */
