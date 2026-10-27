#ifndef LOMBYTE_RNC_OVERLAY_L15_GAMEPLAY_ENTITIES_GATE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L15_GAMEPLAY_ENTITIES_GATE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l15/gameplay/entities/gate.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float vertical_gate_travel_distance __asm__("D_L15_001614F8");
extern f32 swinging_door_swing_angle_degrees __asm__("D_L15_001614FC");
extern f32 swinging_door_swing_duration __asm__("D_L15_00161500");
extern int construction_arm_effect_duration_min_frames __asm__("D_L15_001619F4");
extern int construction_arm_effect_duration_max_frames __asm__("D_L15_001619F8");
extern float construction_arm_effect_speed_max __asm__("D_L15_001619FC");
extern float construction_arm_wait_time_min __asm__("D_L15_00161A00");
extern float construction_arm_wait_time_max __asm__("D_L15_00161A04");
extern float helicopter_bob_period __asm__("D_L15_00161A08");
extern float helicopter_bob_amplitude __asm__("D_L15_00161A0C");
extern float ultramech_pre_hit_scale_multiplier __asm__("D_L15_00161B2C");
extern float ultramech_post_hit_scale_multiplier __asm__("D_L15_00161B30");

#endif /* LOMBYTE_RNC_OVERLAY_L15_GAMEPLAY_ENTITIES_GATE_GLOBALS_H */
