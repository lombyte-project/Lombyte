#ifndef LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_SHARKIGATOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_SHARKIGATOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l05/gameplay/entities/sharkigator.c. */
extern float scenery_ship_path_advance_scale __asm__("D_L05_001613D0") __attribute__((sda));
extern float scenery_ship_removal_effect_basis_scale __asm__("D_L05_001613E8") __attribute__((sda));
extern float scenery_ship_removal_effect_z_addend __asm__("D_L05_001613EC") __attribute__((sda));
extern float extermitank_tracked_particle_vector_a_xy_scale __asm__("D_L05_00161B20") __attribute__((sda));
extern float extermitank_tracked_particle_vector_b_xy_scale __asm__("D_L05_00161B28") __attribute__((sda));
extern float extermitank_tracked_particle_vector_b_spread_degrees __asm__("D_L05_00161B38") __attribute__((sda));
extern float extermitank_secondary_particle_vector_a_xy_scale __asm__("D_L05_00161B50") __attribute__((sda));
extern float extermitank_secondary_particle_vector_b_xy_scale __asm__("D_L05_00161B58") __attribute__((sda));
extern float extermitank_secondary_particle_vector_b_spread_degrees __asm__("D_L05_00161B68") __attribute__((sda));
extern float extermitank_secondary_particle_vector_a_spread_degrees __asm__("D_L05_00161B6C") __attribute__((sda));
extern float extermitank_child_animation_speed_divisor __asm__("D_L05_00161AF0") __attribute__((sda));
extern float extermitank_child_animation_speed_offset_scale __asm__("D_L05_00161AF4") __attribute__((sda));
extern float floating_platform_rotation_y_phase_rate_degrees __asm__("D_L05_00161CD4") __attribute__((sda));
extern float floating_platform_rotation_x_phase_rate_degrees __asm__("D_L05_00161CD8") __attribute__((sda));
extern float floating_platform_rotation_y_amplitude_degrees __asm__("D_L05_00161CDC") __attribute__((sda));
extern float floating_platform_rotation_x_amplitude_degrees __asm__("D_L05_00161CE0") __attribute__((sda));
extern int floating_platform_voice_interval_min __asm__("D_L05_00161CE4") __attribute__((sda));
extern int floating_platform_voice_interval_max __asm__("D_L05_00161CE8") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_SHARKIGATOR_GLOBALS_H */
