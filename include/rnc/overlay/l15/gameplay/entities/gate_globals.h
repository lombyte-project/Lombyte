#ifndef LOMBYTE_RNC_OVERLAY_L15_GAMEPLAY_ENTITIES_GATE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L15_GAMEPLAY_ENTITIES_GATE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l15/gameplay/entities/gate.c. */
extern float vertical_gate_travel_distance __asm__("D_L15_001614F8") __attribute__((sda));
extern f32 swinging_door_swing_angle_degrees __asm__("D_L15_001614FC") __attribute__((sda));
extern f32 swinging_door_swing_duration __asm__("D_L15_00161500") __attribute__((sda));
extern int construction_arm_effect_duration_min_frames __asm__("D_L15_001619F4") __attribute__((sda));
extern int construction_arm_effect_duration_max_frames __asm__("D_L15_001619F8") __attribute__((sda));
extern float construction_arm_effect_speed_max __asm__("D_L15_001619FC") __attribute__((sda));
extern float construction_arm_wait_time_min __asm__("D_L15_00161A00") __attribute__((sda));
extern float construction_arm_wait_time_max __asm__("D_L15_00161A04") __attribute__((sda));
extern float helicopter_bob_period __asm__("D_L15_00161A08") __attribute__((sda));
extern float helicopter_bob_amplitude __asm__("D_L15_00161A0C") __attribute__((sda));
extern float ultramech_pre_hit_scale_multiplier __asm__("D_L15_00161B2C") __attribute__((sda));
extern float ultramech_post_hit_scale_multiplier __asm__("D_L15_00161B30") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L15_GAMEPLAY_ENTITIES_GATE_GLOBALS_H */
