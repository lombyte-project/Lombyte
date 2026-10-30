#ifndef LOMBYTE_RNC_OVERLAY_L06_MOBYS_HANGAR_DOOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L06_MOBYS_HANGAR_DOOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l06/mobys/hangar_door.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float drawbridge_min_segment_distance_base __asm__("D_L06_00161D70");
extern float drawbridge_min_segment_distance_step __asm__("D_L06_00161D74");
extern float beam_barrier_collision_z_offset __asm__("D_L06_00161E18");
extern int   beam_barrier_burst_color_from __asm__("D_L06_00161E1C");
extern int   beam_barrier_burst_color_to __asm__("D_L06_00161E20");
extern float beam_barrier_burst_scale_min __asm__("D_L06_00161E24");
extern float beam_barrier_burst_scale_max __asm__("D_L06_00161E28");
extern int   beam_barrier_burst_lifetime_min_frames __asm__("D_L06_00161E2C");
extern int   beam_barrier_burst_lifetime_max_frames __asm__("D_L06_00161E30");
extern float beam_barrier_burst_drift_min_speed __asm__("D_L06_00161E34");
extern float beam_barrier_burst_drift_max_speed __asm__("D_L06_00161E38");
extern float beam_barrier_texture_u_step __asm__("D_L06_00161DF8");
extern f32   beam_barrier_rotation_x_speed_degrees __asm__("D_L06_00161E10");
extern f32   beam_barrier_render_phase_speed __asm__("D_L06_00161E00");
extern f32   beam_barrier_class_scale_multiplier __asm__("D_L06_00161E14");
extern f32   beam_barrier_strand_jitter_max_speed __asm__("D_L06_00161E04");
extern f32   alien_queen_movement_acceleration __asm__("D_L06_00161EFC");
extern f32   alien_queen_orbit_speed __asm__("D_L06_00161EF8");
extern float moby_pair_parent_fixed_distance __asm__("D_L06_00161F1C");
extern float moby_pair_child_fixed_distance __asm__("D_L06_00161F20");
extern float moby_pair_parent_phase_target_distance __asm__("D_L06_00161F24");
extern float moby_pair_child_phase_target_distance __asm__("D_L06_00161F28");
extern float moby_pair_parent_phase_two_target_rotation_y __asm__("D_L06_00161F2C");
extern float moby_pair_child_phase_two_target_rotation_y __asm__("D_L06_00161F30");
extern float moby_pair_phase_distance_max_speed __asm__("D_L06_00161F3C");
extern float moby_pair_phase_rotation_y_max_speed_degrees __asm__("D_L06_00161F48");
extern int   moby_pair_phase_one_duration_min_frames __asm__("D_L06_00161F4C");
extern int   moby_pair_phase_one_duration_max_frames __asm__("D_L06_00161F50");
extern int   moby_pair_phase_two_duration_min_frames __asm__("D_L06_00161F54");
extern int   moby_pair_phase_two_duration_max_frames __asm__("D_L06_00161F58");
extern float moby_pair_reset_enabled __asm__("D_L06_00161F5C");
extern float height_cycle_max_speed __asm__("D_L06_00161F64");
extern float height_cycle_acceleration __asm__("D_L06_00161F68");

#endif /* LOMBYTE_RNC_OVERLAY_L06_MOBYS_HANGAR_DOOR_GLOBALS_H */
