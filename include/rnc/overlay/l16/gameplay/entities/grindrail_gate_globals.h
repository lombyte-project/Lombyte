#ifndef LOMBYTE_RNC_OVERLAY_L16_GAMEPLAY_ENTITIES_GRINDRAIL_GATE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L16_GAMEPLAY_ENTITIES_GRINDRAIL_GATE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l16/gameplay/entities/grindrail_gate.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float bolt_thief_path_speed __asm__("D_L16_00161D90");
extern float chomper_knockback_horizontal_speed_approach_rate __asm__("D_L16_00161E58");
extern float chomper_knockback_gravity __asm__("D_L16_00161E54");
extern float chomper_knockback_vertical_speed __asm__("D_L16_00161E5C");
extern float chomper_knockback_horizontal_speed __asm__("D_L16_00161E60");
extern float path_walker_gap_range_start __asm__("D_L16_00161E68");
extern float path_walker_gap_range_end __asm__("D_L16_00161E6C");
extern float path_walker_speed_range_start __asm__("D_L16_00161E70");
extern float path_walker_speed_range_end __asm__("D_L16_00161E74");
extern float path_walker_speed_approach_acceleration __asm__("D_L16_00161E78");
extern float path_walker_height_range_start __asm__("D_L16_00161E7C");
extern float path_walker_height_range_end __asm__("D_L16_00161E80");
extern float path_walker_reposition_advance_rate __asm__("D_L16_00161E8C");
extern int path_walker_draw_distance __asm__("D_L16_00161E90");
extern float path_walker_particle_movement_velocity_scale __asm__("D_L16_00161EA0");
extern float path_walker_particle_random_velocity_scale __asm__("D_L16_00161EA4");
extern float path_walker_particle_target_scalar_scale __asm__("D_L16_00161EA8");
extern float elevator_ring_outer_radius __asm__("D_L16_00161EC4");
extern float elevator_ring_half_height __asm__("D_L16_00161EC8");
extern float elevator_ring_segment_angle_degrees __asm__("D_L16_00161EE8");

#endif /* LOMBYTE_RNC_OVERLAY_L16_GAMEPLAY_ENTITIES_GRINDRAIL_GATE_GLOBALS_H */
