#ifndef LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_SHARKIGATOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_SHARKIGATOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l05/gameplay/entities/sharkigator.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float scenery_ship_path_advance_scale __asm__("D_L05_001613D0");
extern float scenery_ship_removal_effect_basis_scale __asm__("D_L05_001613E8");
extern float scenery_ship_removal_effect_z_addend __asm__("D_L05_001613EC");
extern float extermitank_tracked_particle_vector_a_xy_scale __asm__("D_L05_00161B20");
extern float extermitank_tracked_particle_vector_b_xy_scale __asm__("D_L05_00161B28");
extern float extermitank_tracked_particle_vector_b_spread_degrees __asm__("D_L05_00161B38");
extern float extermitank_secondary_particle_vector_a_xy_scale __asm__("D_L05_00161B50");
extern float extermitank_secondary_particle_vector_b_xy_scale __asm__("D_L05_00161B58");
extern float extermitank_secondary_particle_vector_b_spread_degrees __asm__("D_L05_00161B68");
extern float extermitank_secondary_particle_vector_a_spread_degrees __asm__("D_L05_00161B6C");
extern float extermitank_child_animation_speed_divisor __asm__("D_L05_00161AF0");
extern float extermitank_child_animation_speed_offset_scale __asm__("D_L05_00161AF4");
extern float floating_platform_rotation_y_phase_rate_degrees __asm__("D_L05_00161CD4");
extern float floating_platform_rotation_x_phase_rate_degrees __asm__("D_L05_00161CD8");
extern float floating_platform_rotation_y_amplitude_degrees __asm__("D_L05_00161CDC");
extern float floating_platform_rotation_x_amplitude_degrees __asm__("D_L05_00161CE0");
extern int floating_platform_voice_interval_min __asm__("D_L05_00161CE4");
extern int floating_platform_voice_interval_max __asm__("D_L05_00161CE8");

#endif /* LOMBYTE_RNC_OVERLAY_L05_GAMEPLAY_ENTITIES_SHARKIGATOR_GLOBALS_H */
