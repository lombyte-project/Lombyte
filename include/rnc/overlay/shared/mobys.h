#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_H

#include "types.h"

struct Moby;

void add_moby_frames_for_group_proximity(char *) __asm__("FUN_L05_00307010");
void offset_moby_class_point_pairs(unsigned char *, int, int) __asm__("FUN_L00_0024efb0");
void camera_object_update(char *) __asm__("FUN_L00_002a4e48");
s32  compute_advanced_moby_path_position(struct Moby *, void *, void *, s32, s32, f32) __asm__("FUN_L09_00294ae0");
void  draw_only_moby_update(struct Moby *) __asm__("FUN_L16_002e9df0");
void  falling_moby_update(char *) __asm__("FUN_L03_002de370");
int   find_nearest_animation_sequence_entry(char *, float) __asm__("FUN_L00_0024ed18");
int   find_point_ahead(char *, char *, float) __asm__("FUN_L01_00274b78");
char *get_moby_pvars_link(struct Moby *) __asm__("FUN_L00_002603d0");
int   is_moby_class_23e_in_state_14(char *) __asm__("FUN_L03_002c6c60");
void  set_moby_state_14(unsigned char *) __asm__("FUN_L03_002c6c90");
void  moby_draw_callback_update(struct Moby *) __asm__("FUN_L03_002dc9b0");
void  moby_draw_registration_update(unsigned char *) __asm__("FUN_L07_002f8058");
void  remove_when_near_hero_update(char *) __asm__("FUN_L11_002cb790");
void  show_tracked_moby(void) __asm__("FUN_L01_002a2480");
void  spawn_nearby_effect_without_direction(unsigned char *, int, int, void *, float) __asm__("FUN_L10_0024c7f0");
int  test_linked_moby_turn_trigger(struct Moby *) __asm__("FUN_L11_0030f270");
void update_moby_proximity_interaction(unsigned char *) __asm__("FUN_L00_0028e180");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_H */
