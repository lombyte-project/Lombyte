#ifndef LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_IDLE_SEQUENCE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_IDLE_SEQUENCE_H

#include "types.h"

void release_and_clear_hero_voice_slots(void) __asm__("FUN_L00_00206c08");
void emit_hero_surface_effect_burst(s32 puffs, s32 debris, s32 flash) __asm__("FUN_L00_00209ca8");
s32  return_hero_animation_id_unchanged(s32 input_value) __asm__("FUN_L00_002050b8");

void set_first_hero_easing_parameters(float, float) __asm__("FUN_L00_0020a1e0");
void update_hero_moby(void) __asm__("FUN_L00_00208820");
void update_hero_world_matrices(s32) __asm__("FUN_L00_00209240");
void update_hero_easing_records(void) __asm__("FUN_L00_002054e8");
void update_hero_rotation_targets_and_random_manipulators(void) __asm__("FUN_L00_002056a8");
void update_hero_glow_and_extra_manipulators(void) __asm__("FUN_L00_002059d8");
void update_hero_bolt_exclamation_hud(void) __asm__("FUN_L00_00205ea8");
void dispatch_hero_animation_timed_events(void) __asm__("FUN_L00_00206080");
void check_hero_item_reward_conditions(void) __asm__("FUN_L00_00206cf8");
void scan_hero_input_sequences(void) __asm__("FUN_L00_00206e00");
void apply_hero_and_item_scale_sequence(void) __asm__("FUN_L00_00207a58");
void update_hero_scalar_pulse(void) __asm__("FUN_L00_00207c48");
void update_hero_moby_probed_height_bounds(void) __asm__("FUN_L00_00208b60");
void update_hero_collision_ease_target(void) __asm__("FUN_L00_0020a318");
void update_hero_pitch_and_yaw_easing_targets(void) __asm__("FUN_L00_0020a540");
void update_hero_idle_random_rotation_targets(void) __asm__("FUN_L00_0020a9e8");
void update_hero_target_tracking(void) __asm__("FUN_L00_0020ac98");
void update_hero_easing_targets_and_point_effects(void) __asm__("FUN_L00_0020b1d8");
void jet_camera_mode(void) __asm__("FUN_L00_00203b18");
void reset_hero_to_idle(void) __asm__("FUN_L00_00204f80");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_IDLE_SEQUENCE_H */
