#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VELDIN_SHIP_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VELDIN_SHIP_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/veldin_ship.c. */
extern int metal_detector_trail_initial_quad_color __asm__("D_L00_00161914") __attribute__((sda));
extern int metal_detector_trail_texture_selector __asm__("D_L00_00161918") __attribute__((sda));
extern float horny_toad_target_approach_distance_threshold __asm__("D_L00_00161980") __attribute__((sda));
extern float horny_toad_target_animation_start_index __asm__("D_L00_00161984") __attribute__((sda));
extern int smoke_fire_emission_attempts_per_update __asm__("D_L00_001619BC") __attribute__((sda));
extern int smoke_fire_particle_steady_alpha __asm__("D_L00_00161998") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_VELDIN_SHIP_GLOBALS_H */
