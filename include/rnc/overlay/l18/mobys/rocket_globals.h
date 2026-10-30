#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l18/mobys/rocket.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float rocket_target_pulse_lead_time __asm__("D_L18_00161964");
extern float rocket_spawn_scale_multiplier __asm__("D_L18_00161960");
extern float rocket_explosion_hit_priority __asm__("D_L18_0016196C");
extern int   rocket_target_pulse_color_start __asm__("D_L18_00161980");
extern int   rocket_target_pulse_color_end __asm__("D_L18_00161984");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_GLOBALS_H */
