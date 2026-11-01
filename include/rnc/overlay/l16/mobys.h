#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_H

#include "types.h"

struct Moby;

void bolt_thief_update() __asm__("FUN_L16_002e37a0");
void thrown_bomb_update() __asm__("FUN_L16_002c5eb0");
void breakable_prop_update(struct Moby *) __asm__("FUN_L16_002d6740");
void chicken_relocator_update(struct Moby *) __asm__("FUN_L16_002e8970");
void chomper_update(struct Moby *) __asm__("FUN_L16_002e5e08");
void clip_volume_dialogue_trigger_update(struct Moby *) __asm__("FUN_L16_002e7208");
void conveyor_belt_update(struct Moby *) __asm__("FUN_L16_002c9878");
void cutscene_joint_trails_update(struct Moby *) __asm__("FUN_L16_002e7e00");
void elevator_update() __asm__("FUN_L16_002e7f80");
void gadgetron_ceo_update() __asm__("FUN_L16_002e6808");
void gadgetron_sign_update(struct Moby *) __asm__("FUN_L16_002e88d0");
void grindrail_button_update(struct Moby *) __asm__("FUN_L16_002e54a0");
void grindrail_gate_update(struct Moby *) __asm__("FUN_L16_002e5010");
void help_desk_lady_update() __asm__("FUN_L16_002e3190");
void linked_pair_ribbons_update(struct Moby *) __asm__("FUN_L16_002cf4a8");
void local_bounds_command_trigger_update(struct Moby *) __asm__("FUN_L16_002e5258");
void path_following_rocket_update(struct Moby *) __asm__("FUN_L16_002c3d38");
void path_walker_update(unsigned char *) __asm__("FUN_L16_002e76b8");
void reflection_mesh_update(struct Moby *) __asm__("FUN_L16_002e93b0");
void reflection_mesh_pair_update(struct Moby *) __asm__("FUN_L16_002e98e0");
void draw_level16_reflection_mesh_batch(void) __asm__("FUN_L16_002e9ce0");
void reflection_mesh_trio_b_update(struct Moby *) __asm__("FUN_L16_002ea128");
void homing_projectile_update(unsigned char *) __asm__("FUN_L16_002a3f38");
void spinning_sprite_effect_update(struct Moby *) __asm__("FUN_L16_002c9480");
void taxi_update(struct Moby *) __asm__("FUN_L16_002e43e0");
void test_dummy_challenge_update(struct Moby *) __asm__("FUN_L16_002d5ef8");
void test_dummy_on_hoverboard_update(unsigned char *) __asm__("FUN_L16_002d04a0");
void wall_button_update(struct Moby *) __asm__("FUN_L16_002cf198");
void wall_button_blocker_update(struct Moby *) __asm__("FUN_L16_002e36e8");
void set_wall_button_blocker_collision(void *, int) __asm__("FUN_L16_002e3770");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_H */
