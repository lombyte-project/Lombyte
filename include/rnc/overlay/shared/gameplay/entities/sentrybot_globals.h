#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SENTRYBOT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SENTRYBOT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/sentrybot.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float sentrybot_class_scale_multiplier __asm__("D_L15_001613EC");
extern int rocket_trail_spark_initial_configuration_word __asm__("D_L15_00161418");
extern float rocket_trail_spark_frame_count_range_start __asm__("D_L15_00161428");
extern float rocket_trail_spark_third_frame_count_range_start __asm__("D_L15_00161438");
extern float rocket_trail_spark_first_velocity_scale __asm__("D_L15_00161448");
extern int rocket_trail_spark_second_configuration_word __asm__("D_L15_0016141C");
extern float rocket_trail_spark_frame_count_range_end __asm__("D_L15_0016142C");
extern float rocket_trail_spark_second_frame_count_range_start __asm__("D_L15_00161430");
extern float rocket_trail_spark_second_frame_count_range_end __asm__("D_L15_00161434");
extern float rocket_trail_spark_third_frame_count_range_end __asm__("D_L15_0016143C");
extern float rocket_trail_spark_first_noise_radius_factor __asm__("D_L15_00161440");
extern float rocket_trail_spark_second_noise_radius_factor __asm__("D_L15_00161444");
extern float rocket_trail_spark_second_velocity_scale __asm__("D_L15_0016144C");
extern int rocket_trail_spark_lookup_index_and_flags __asm__("D_L15_00161454");
extern float rocket_detonation_glow_scale __asm__("D_L15_00161458");
extern int rocket_detonation_glow_lifetime_frames __asm__("D_L15_0016145C");
extern float group_moby_seeking_speed __asm__("D_L15_001614B4");
extern int group_moby_particle_color __asm__("D_L15_001614B8");
extern float group_moby_particle_scale __asm__("D_L15_001614BC");
extern int group_moby_particle_lifetime_frames __asm__("D_L15_001614C0");
extern float group_moby_effect_z_offset __asm__("D_L15_001614C4");
extern int group_moby_effect_first_quad_color __asm__("D_L15_001614C8");
extern int spiderbot_effect_spawn_count __asm__("D_L15_00161558");
extern float spiderbot_effect_scale __asm__("D_L15_0016155C");
extern float spiderbot_effect_lifetime_range_end __asm__("D_L15_00161560");
extern int sentrybot_alarm_voice_slot_index __asm__("D_L15_00161AC4");

/* Additional scalar data read by this unit. */
extern int sentrybot_alert_level_frame_stamp_gp __asm__("D_L15_001613E8") __attribute__((sda));
extern float sentrybot_emission_vector_speed_gp __asm__("D_L15_00161410") __attribute__((sda));
extern int sentrybot_update_disabled_gp __asm__("D_L15_00161414") __attribute__((sda));
extern float sentrybot_knockback_gravity_gp __asm__("D_L15_001613F0") __attribute__((sda));
extern float sentrybot_death_knockback_gravity_gp __asm__("D_L15_001613F4") __attribute__((sda));
extern float sentrybot_recoverable_knockback_vertical_speed_gp __asm__("D_L15_001613F8") __attribute__((sda));
extern float sentrybot_recoverable_knockback_horizontal_speed_gp __asm__("D_L15_001613FC") __attribute__((sda));
extern float sentrybot_recoverable_knockback_horizontal_speed_approach_rate_gp __asm__("D_L15_00161400") __attribute__((sda));
extern float sentrybot_death_knockback_vertical_speed_gp __asm__("D_L15_00161404") __attribute__((sda));
extern float sentrybot_death_knockback_horizontal_speed_gp __asm__("D_L15_00161408") __attribute__((sda));
extern float sentrybot_death_knockback_horizontal_speed_approach_rate_gp __asm__("D_L15_0016140C") __attribute__((sda));
extern float rocket_trail_spark_first_vector_fourth_component_gp __asm__("D_L15_00161420") __attribute__((sda));
extern float rocket_trail_spark_second_vector_fourth_component_gp __asm__("D_L15_00161424") __attribute__((sda));
extern float missile_distant_gray_glow_gate __asm__("D_L15_0015F5D0");
extern int missile_target_reticle_enabled __asm__("D_L15_0015F30C");
extern int spark_emitter_uv_phase_input __asm__("D_L15_0015F3F8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SENTRYBOT_GLOBALS_H */
