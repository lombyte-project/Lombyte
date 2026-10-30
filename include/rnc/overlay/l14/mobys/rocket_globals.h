#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_ROCKET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_ROCKET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/mobys/rocket.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float rocket_puff_initial_right_offset __asm__("D_L14_00161F90");
extern float rocket_puff_forward_offset __asm__("D_L14_00161F94");
extern float rocket_puff_up_offset __asm__("D_L14_00161F98");
extern float rocket_puff_middle_right_offset_delta __asm__("D_L14_00161F9C");
extern float rocket_puff_final_right_offset __asm__("D_L14_00161FA0");
extern s32   rocket_initial_timer_frames __asm__("D_L14_00161FB0");
extern f32   rocket_travel_speed __asm__("D_L14_00161FA8");
extern f32   rocket_steering_blend_fraction __asm__("D_L14_00161FAC");

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_ROCKET_GLOBALS_H */
