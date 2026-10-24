#ifndef LOMBYTE_RNC_OVERLAY_L13_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L13_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l13/gameplay/entities/rocket.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int poison_vessel_blast_duration_frames __asm__("D_L13_00161E2C");
extern f32 poison_vessel_blast_start_radius __asm__("D_L13_00161E30");
extern f32 poison_vessel_blast_end_radius __asm__("D_L13_00161E34");
extern f32 poison_vessel_blast_damage __asm__("D_L13_00161E38");
extern int blarg_space_commando_face_target_wait_frames __asm__("D_L13_00161E70");
extern float gemlik_trail_spread_degrees __asm__("D_L13_00161E80");
extern float gemlik_trail_point_radial_speed __asm__("D_L13_00161E7C");
extern int gemlik_pool_tile_count __asm__("D_L13_001612D8");
extern float moby_spark_first_batch_spread_degrees __asm__("D_L13_00161F50");
extern float moby_spark_second_batch_spread_degrees __asm__("D_L13_00161F54");

#endif /* LOMBYTE_RNC_OVERLAY_L13_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H */
