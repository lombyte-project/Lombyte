#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_REMOVE_MOBY_WHEN_NEAR_HERO_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_REMOVE_MOBY_WHEN_NEAR_HERO_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/mobys/remove_moby_when_near_hero.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   proximity_removal_frame_count __asm__("D_L11_0015F640");
extern int   projectile_trail_color_start __asm__("D_L11_00161DC0");
extern int   projectile_trail_color_end __asm__("D_L11_00161DC4");
extern int   projectile_trail_frame_count __asm__("D_L11_00161DC8");
extern float projectile_trail_velocity_scale __asm__("D_L11_00161DD0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_REMOVE_MOBY_WHEN_NEAR_HERO_GLOBALS_H */
