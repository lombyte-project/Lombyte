#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_VENDOR_00307190_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_VENDOR_00307190_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/enemies/spawn_paratrooper_projectile.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float ground_particle_scatter_refresh_countdown __asm__("D_L08_00162370");
extern int   ground_particle_scatter_base_attempt_count __asm__("D_L08_00162374");
extern float ground_particle_scatter_spawn_vector_y_bias __asm__("D_L08_00162364");
extern float ground_particle_scatter_minimum_z_scale __asm__("D_L08_00162340");
extern float ground_particle_scatter_reference_height_offset __asm__("D_L08_00162344");
extern float ground_particle_scatter_xy_offset_random_range __asm__("D_L08_00162348");
extern float ground_particle_scatter_minimum_z_scale_override __asm__("D_L08_00162354");
extern int   ground_particle_scatter_distant_spawn_argument __asm__("D_L08_00162358");
extern float ground_particle_scatter_ground_height_floor_offset __asm__("D_L08_0016235C");
extern float smoothed_below_reference_ground_fraction __asm__("D_L08_001625E8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_VENDOR_00307190_GLOBALS_H */
