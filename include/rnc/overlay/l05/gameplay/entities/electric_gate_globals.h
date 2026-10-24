#ifndef LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_ELECTRIC_GATE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_ELECTRIC_GATE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l05/gameplay/entities/electric_gate.c. */
extern float drain_cover_tilt_acceleration __asm__("D_L05_00161DDC") __attribute__((sda));
extern float drain_cover_tilt_deceleration __asm__("D_L05_00161DE0") __attribute__((sda));
extern float drain_cover_tilt_max_speed __asm__("D_L05_00161DE4") __attribute__((sda));
extern float gate_rise_acceleration __asm__("D_L05_00161E80") __attribute__((sda));
extern float gate_rise_deceleration __asm__("D_L05_00161E84") __attribute__((sda));
extern float gate_rise_max_speed __asm__("D_L05_00161E88") __attribute__((sda));
extern float gate_swing_acceleration __asm__("D_L05_00161E8C") __attribute__((sda));
extern float gate_swing_deceleration __asm__("D_L05_00161E90") __attribute__((sda));
extern float gate_swing_max_speed __asm__("D_L05_00161E94") __attribute__((sda));
extern f32 elevator_camera_transition_fade __asm__("D_L05_0015F3FC");
extern int hero_inside_clip_volume __asm__("D_L05_001612DC");
extern float hero_clip_volume_output_w __asm__("D_L05_001612EC");

#endif /* LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_ELECTRIC_GATE_GLOBALS_H */
