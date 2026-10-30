#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_VELDIN_SHIP_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_VELDIN_SHIP_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/vehicles/veldin_ship.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   metal_detector_trail_initial_quad_color __asm__("D_L00_00161914");
extern int   metal_detector_trail_texture_selector __asm__("D_L00_00161918");
extern float horny_toad_target_approach_distance_threshold __asm__("D_L00_00161980");
extern float horny_toad_target_animation_start_index __asm__("D_L00_00161984");
extern int   smoke_fire_emission_attempts_per_update __asm__("D_L00_001619BC");
extern int   smoke_fire_particle_steady_alpha __asm__("D_L00_00161998");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_VELDIN_SHIP_GLOBALS_H */
