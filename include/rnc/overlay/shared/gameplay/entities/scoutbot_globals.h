#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SCOUTBOT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SCOUTBOT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/scoutbot.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float scoutbot_glow_position_offset_scale __asm__("D_L08_00161898");
extern float scoutbot_glow_quad_vertex_scale __asm__("D_L08_0016189C");
extern float moby_creation_default_timer_frames __asm__("D_L08_00161CD8");
extern float moby_creation_particle_count_limit __asm__("D_L08_00161CE0");
extern float moby_creation_particle_spread_speed_base __asm__("D_L08_00161D04");
extern float homing_projectile_trail_particle_count_limit __asm__("D_L08_00161CDC");
extern float homing_projectile_spin_rate_degrees __asm__("D_L08_00161CE4");
extern float homing_projectile_trail_spread_speed_limit __asm__("D_L08_00161D00");
extern f32 spark_emitter_default_primary_z_increment_base __asm__("D_L08_00161F7C");
extern f32 spark_emitter_default_primary_magnitude_base __asm__("D_L08_00161F80");
extern f32 spark_emitter_default_secondary_z_increment_base __asm__("D_L08_00161F84");
extern f32 spark_emitter_default_secondary_magnitude_base __asm__("D_L08_00161F88");
extern s32 spark_emitter_default_timer_reset __asm__("D_L08_00161F98");
extern f32 spark_emitter_default_primary_spread_limit __asm__("D_L08_00161FA4");
extern f32 spark_emitter_default_secondary_spread_limit __asm__("D_L08_00161FA8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SCOUTBOT_GLOBALS_H */
