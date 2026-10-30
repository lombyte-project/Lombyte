#ifndef LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMITANK_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMITANK_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l05/mobys/extermitank.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float extermitank_tracked_particle_vector_a_xy_scale __asm__("D_L05_00161B20");
extern float extermitank_tracked_particle_vector_b_xy_scale __asm__("D_L05_00161B28");
extern float extermitank_tracked_particle_vector_b_spread_degrees __asm__("D_L05_00161B38");
extern float extermitank_secondary_particle_vector_a_xy_scale __asm__("D_L05_00161B50");
extern float extermitank_secondary_particle_vector_b_xy_scale __asm__("D_L05_00161B58");
extern float extermitank_secondary_particle_vector_b_spread_degrees __asm__("D_L05_00161B68");
extern float extermitank_secondary_particle_vector_a_spread_degrees __asm__("D_L05_00161B6C");
extern float extermitank_child_animation_speed_divisor __asm__("D_L05_00161AF0");
extern float extermitank_child_animation_speed_offset_scale __asm__("D_L05_00161AF4");

#endif /* LOMBYTE_RNC_OVERLAY_L05_MOBYS_EXTERMITANK_GLOBALS_H */
