#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_SKY_UPDATE_SKY_SHELLS_ANIMATED_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_SKY_UPDATE_SKY_SHELLS_ANIMATED_H

#include "types.h"

void update_sky_shells_static(void) __asm__("FUN_L00_002892d0");
void sky_shells_rotate_two(void) __asm__("FUN_L00_002899a8");

void update_sky_shells_animated(void) __asm__("FUN_L00_00289420");
void draw_sky_shells_with_fast_and_slow_rotation(void) __asm__("FUN_L00_00289a90");
void draw_sky_shells_with_particles_and_modulo_50000_rotation(void) __asm__("FUN_L00_00289bd8");
void draw_three_sky_shells_with_particles_and_modulo_50000_rotation(void) __asm__("FUN_L00_0028a208");
void draw_sky_shells_with_modulo_40000_rotation(void) __asm__("FUN_L00_0028a2f0");
void draw_sky_shells_with_modulo_25000_rotation(void) __asm__("FUN_L00_0028a3a8");
void draw_sky_shells_with_scaled_phase_rotation(void) __asm__("FUN_L00_0028a480");
void draw_sky_shells_with_slow_medium_and_fast_rotation(void) __asm__("FUN_L00_0028a620");
void draw_three_sky_shells_with_particles(void) __asm__("FUN_L00_0028a920");
void draw_three_sky_shells_with_modulo_50000_and_100000_rotation(void) __asm__("FUN_L00_0028a998");
void draw_two_sky_shells_with_particles(void) __asm__("FUN_L00_0028aa98");
void draw_seven_sky_shells_with_modulo_40000_and_60000_rotation(void) __asm__("FUN_L00_0028b080");
void draw_sky_shells_with_rotation_and_scaling(void) __asm__("FUN_L00_0028b1a0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_SKY_UPDATE_SKY_SHELLS_ANIMATED_H */
