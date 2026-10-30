#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_BOSS_RING_PROJECTILE_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_BOSS_RING_PROJECTILE_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l18/mobys/drek_boss_ring_projectile.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   ballistic_ring_glow_color_end __asm__("D_L18_00161C00");
extern float ballistic_ring_trail_spark_gravity __asm__("D_L18_00161C04");
extern float ballistic_ring_trail_movement_step_scale __asm__("D_L18_00161C08");
extern int   ballistic_ring_trail_initial_color_start __asm__("D_L18_00161C30");
extern int   ballistic_ring_trail_initial_color_end __asm__("D_L18_00161C34");
extern int   ballistic_ring_trail_second_color_start __asm__("D_L18_00161C38");
extern int   ballistic_ring_trail_second_color_end __asm__("D_L18_00161C3C");
extern int   ballistic_ring_trail_sparks_per_update __asm__("D_L18_00161C40");
extern int   expanding_projectile_ring_color_start __asm__("D_L18_00161C5C");
extern int   expanding_projectile_ring_color_end __asm__("D_L18_00161C60");
extern float expanding_projectile_ring_segment_angle_degrees __asm__("D_L18_00161C64");
extern float expanding_projectile_ring_draw_height_offset __asm__("D_L18_00161C68");
extern float expanding_projectile_ring_layer_radius_offset __asm__("D_L18_00161C70");
extern float ring_segment_particle_cull_depth_limit __asm__("D_L18_00161BF4");
extern int   ring_segment_particle_color_start __asm__("D_L18_00161C80");
extern int   ring_segment_particle_color_end __asm__("D_L18_00161C84");
extern float ring_segment_particle_scale __asm__("D_L18_00161C88");
extern int   ring_segment_radial_particle_lifetime_frames __asm__("D_L18_00161C8C");
extern int   ring_segment_horizontal_particle_lifetime_frames __asm__("D_L18_00161C90");
extern float ring_segment_particle_spawn_probability __asm__("D_L18_00161C98");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_BOSS_RING_PROJECTILE_GLOBALS_H */
