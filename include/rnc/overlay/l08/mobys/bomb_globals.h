#ifndef LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMB_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMB_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l08/mobys/bomb.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float bolt_pickup_spin_increment_range_start __asm__("D_L08_00161468");
extern float bolt_pickup_spin_increment_range_end __asm__("D_L08_0016146C");
extern float bolt_pickup_collision_stop_speed_threshold __asm__("D_L08_00161458");
extern float bolt_pickup_collision_speed_reduction __asm__("D_L08_0016145C");
extern int   bolt_pickup_bob_period_ticks __asm__("D_L08_00161460");
extern float bolt_pickup_bob_amplitude __asm__("D_L08_00161464");
extern float bomb_downward_acceleration __asm__("D_L08_00161860");
extern float bomber_tank_projectile_speed __asm__("D_L08_001618A8");
extern int   bomber_tank_part_3_destroyed_by_class_661 __asm__("D_L08_0015F5B8");
extern float bomber_tank_part_reference_speed __asm__("D_L08_001618A0");
extern float bomber_tank_joint_particle_jitter_magnitude_base __asm__("D_L08_001618DC");
extern float bomber_tank_joint_particle_vertical_spread __asm__("D_L08_001618E0");
extern float bomber_tank_joint_particle_horizontal_spread_range_end __asm__("D_L08_001618E8");
extern int   available_moby_slot_count __asm__("D_L08_0015FFBC");
extern int   water_entry_plane_initialized __asm__("D_L08_00161970");
extern float line_puff_initial_z_velocity_range_end __asm__("D_L08_00161998");
extern float line_puff_position_jitter_range_end __asm__("D_L08_001619A0");
extern u16   line_puff_duration_frames __asm__("D_L08_001619A8");
extern float rocket_guidance_velocity_correction_limit __asm__("D_L08_00161A04");
extern float rocket_route_speed __asm__("D_L08_00161A08");
extern float rocket_target_hit_distance __asm__("D_L08_00161A0C");
extern float rocket_effect_radial_speed_range_end __asm__("D_L08_00161A14");
extern int   class_1b3_projectile_color_red __asm__("D_L08_00161A34");
extern int   class_1b3_projectile_color_green __asm__("D_L08_00161A38");
extern int   class_1b3_projectile_color_blue __asm__("D_L08_00161A3C");
extern float class_1b3_projectile_scale_multiplier __asm__("D_L08_00161A40");
extern float moby_motion_spark_first_displacement_scale __asm__("D_L08_00161A48");
extern float moby_motion_spark_second_displacement_scale __asm__("D_L08_00161A4C");
extern float moby_motion_spark_jitter_magnitude_range_end __asm__("D_L08_00161A50");
extern int   moby_motion_spark_frame_count_random_span __asm__("D_L08_00161A54");
extern int   moby_motion_spark_second_frame_count_base __asm__("D_L08_00161A58");
extern int   moby_motion_spark_third_frame_count_base __asm__("D_L08_00161A5C");
extern float moby_motion_spark_frame_count_variation_extent __asm__("D_L08_00161A60");
extern float moby_motion_spark_first_vector_w_range_start __asm__("D_L08_00161A64");
extern float moby_motion_spark_first_vector_w_range_end __asm__("D_L08_00161A68");
extern float moby_motion_spark_second_vector_w_range_start __asm__("D_L08_00161A6C");
extern float moby_motion_spark_second_vector_w_range_end __asm__("D_L08_00161A70");
extern int   moby_motion_spark_initial_color_start __asm__("D_L08_00161A74");
extern int   moby_motion_spark_initial_color_end __asm__("D_L08_00161A78");
extern int   moby_motion_spark_second_color_start __asm__("D_L08_00161A7C");
extern int   moby_motion_spark_second_color_end __asm__("D_L08_00161A80");

#endif /* LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMB_GLOBALS_H */
