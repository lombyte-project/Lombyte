#ifndef LOMBYTE_RNC_OVERLAY_L13_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L13_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l13/gameplay/entities/rocket.c. */
extern int poison_vessel_blast_duration_frames __asm__("D_L13_00161E2C") __attribute__((sda));
extern f32 poison_vessel_blast_start_radius __asm__("D_L13_00161E30") __attribute__((sda));
extern f32 poison_vessel_blast_end_radius __asm__("D_L13_00161E34") __attribute__((sda));
extern f32 poison_vessel_blast_damage __asm__("D_L13_00161E38") __attribute__((sda));
extern int blarg_space_commando_face_target_wait_frames __asm__("D_L13_00161E70");
extern float gemlik_trail_spread_degrees __asm__("D_L13_00161E80") __attribute__((sda));
extern float gemlik_trail_point_radial_speed __asm__("D_L13_00161E7C") __attribute__((sda));
extern int gemlik_pool_tile_count __asm__("D_L13_001612D8");
extern float moby_spark_first_batch_spread_degrees __asm__("D_L13_00161F50") __attribute__((sda));
extern float moby_spark_second_batch_spread_degrees __asm__("D_L13_00161F54") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L13_GAMEPLAY_ENTITIES_ROCKET_GLOBALS_H */
