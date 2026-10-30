#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_PATH_WALKER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_PATH_WALKER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l16/mobys/path_walker.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float path_walker_gap_range_start __asm__("D_L16_00161E68");
extern float path_walker_gap_range_end __asm__("D_L16_00161E6C");
extern float path_walker_speed_range_start __asm__("D_L16_00161E70");
extern float path_walker_speed_range_end __asm__("D_L16_00161E74");
extern float path_walker_speed_approach_acceleration __asm__("D_L16_00161E78");
extern float path_walker_height_range_start __asm__("D_L16_00161E7C");
extern float path_walker_height_range_end __asm__("D_L16_00161E80");
extern float path_walker_reposition_advance_rate __asm__("D_L16_00161E8C");
extern int   path_walker_draw_distance __asm__("D_L16_00161E90");
extern float path_walker_particle_movement_velocity_scale __asm__("D_L16_00161EA0");
extern float path_walker_particle_random_velocity_scale __asm__("D_L16_00161EA4");
extern float path_walker_particle_target_scalar_scale __asm__("D_L16_00161EA8");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_PATH_WALKER_GLOBALS_H */
