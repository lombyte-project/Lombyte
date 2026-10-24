#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SENTRYBOT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SENTRYBOT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/sentrybot.c. */
extern float sentrybot_class_scale_multiplier __asm__("D_L15_001613EC") __attribute__((sda));
extern int rocket_trail_spark_initial_configuration_word __asm__("D_L15_00161418") __attribute__((sda));
extern float rocket_trail_spark_frame_count_range_start __asm__("D_L15_00161428") __attribute__((sda));
extern float rocket_trail_spark_third_frame_count_range_start __asm__("D_L15_00161438") __attribute__((sda));
extern float rocket_trail_spark_first_velocity_scale __asm__("D_L15_00161448") __attribute__((sda));
extern int rocket_trail_spark_second_configuration_word __asm__("D_L15_0016141C") __attribute__((sda));
extern float rocket_trail_spark_frame_count_range_end __asm__("D_L15_0016142C") __attribute__((sda));
extern float rocket_trail_spark_second_frame_count_range_start __asm__("D_L15_00161430") __attribute__((sda));
extern float rocket_trail_spark_second_frame_count_range_end __asm__("D_L15_00161434") __attribute__((sda));
extern float rocket_trail_spark_third_frame_count_range_end __asm__("D_L15_0016143C") __attribute__((sda));
extern float rocket_trail_spark_first_noise_radius_factor __asm__("D_L15_00161440") __attribute__((sda));
extern float rocket_trail_spark_second_noise_radius_factor __asm__("D_L15_00161444") __attribute__((sda));
extern float rocket_trail_spark_second_velocity_scale __asm__("D_L15_0016144C") __attribute__((sda));
extern int rocket_trail_spark_lookup_index_and_flags __asm__("D_L15_00161454") __attribute__((sda));
extern float rocket_detonation_glow_scale __asm__("D_L15_00161458") __attribute__((sda));
extern int rocket_detonation_glow_lifetime_frames __asm__("D_L15_0016145C") __attribute__((sda));
extern float group_moby_seeking_speed __asm__("D_L15_001614B4") __attribute__((sda));
extern int group_moby_particle_color __asm__("D_L15_001614B8") __attribute__((sda));
extern float group_moby_particle_scale __asm__("D_L15_001614BC") __attribute__((sda));
extern int group_moby_particle_lifetime_frames __asm__("D_L15_001614C0") __attribute__((sda));
extern float group_moby_effect_z_offset __asm__("D_L15_001614C4") __attribute__((sda));
extern int group_moby_effect_first_quad_color __asm__("D_L15_001614C8") __attribute__((sda));
extern int spiderbot_effect_spawn_count __asm__("D_L15_00161558") __attribute__((sda));
extern float spiderbot_effect_scale __asm__("D_L15_0016155C") __attribute__((sda));
extern float spiderbot_effect_lifetime_range_end __asm__("D_L15_00161560") __attribute__((sda));
extern int sentrybot_alarm_voice_slot_index __asm__("D_L15_00161AC4") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_SENTRYBOT_GLOBALS_H */
