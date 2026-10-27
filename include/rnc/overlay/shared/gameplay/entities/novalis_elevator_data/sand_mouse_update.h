#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NOVALIS_ELEVATOR_DATA_SAND_MOUSE_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NOVALIS_ELEVATOR_DATA_SAND_MOUSE_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/shared/gameplay/entities/novalis_elevator.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int sand_mouse_follow_duration_frames_gp __asm__("D_L01_00161918") __attribute__((sda));
extern int sand_mouse_follow_projectile_count_limit_gp __asm__("D_L01_0016191C") __attribute__((sda));
extern int sand_mouse_fade_duration_frames_gp __asm__("D_L01_00161924") __attribute__((sda));
extern int sand_mouse_fade_particle_random_integer_min_gp __asm__("D_L01_00161928") __attribute__((sda));
extern int sand_mouse_fade_particle_random_integer_max_gp __asm__("D_L01_0016192C") __attribute__((sda));
extern float sand_mouse_fade_particle_position_jitter_gp __asm__("D_L01_00161930") __attribute__((sda));
extern float sand_mouse_fade_particle_random_float_range_start_gp __asm__("D_L01_00161934") __attribute__((sda));
extern float sand_mouse_fade_particle_random_float_range_end_gp __asm__("D_L01_00161938") __attribute__((sda));
extern int sand_mouse_fade_particle_color_alpha_gp __asm__("D_L01_0016193C") __attribute__((sda));
extern float sand_mouse_fade_particle_time_squared_coefficient_gp __asm__("D_L01_00161940") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_NOVALIS_ELEVATOR_DATA_SAND_MOUSE_UPDATE_H */
