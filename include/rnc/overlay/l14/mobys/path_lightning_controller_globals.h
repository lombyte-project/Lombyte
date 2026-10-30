#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_PATH_LIGHTNING_CONTROLLER_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_PATH_LIGHTNING_CONTROLLER_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/mobys/path_lightning_controller.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   lightning_arc_remaining_frames __asm__("D_L14_00161CB4");
extern float lightning_displaced_endpoint_y __asm__("D_L14_00161CD4");
extern float lightning_displaced_endpoint_z __asm__("D_L14_00161CD8");
extern float lightning_path_endpoint_w __asm__("D_L14_00161CCC");
extern f32   lightning_arc_alpha_pulse_angle_step __asm__("D_L14_00161C68");
extern f32   lightning_arc_alpha_pulse_sweep __asm__("D_L14_00161C6C");
extern f32   lightning_main_arc_point_spacing __asm__("D_L14_00161C74");
extern f32   lightning_main_arc_z_displacement_limit __asm__("D_L14_00161C94");
extern s32   lightning_arc_lifetime_range_start __asm__("D_L14_00161C98");
extern s32   lightning_arc_lifetime_range_end __asm__("D_L14_00161C9C");
extern s16   lightning_arc_alpha_pulse_offset __asm__("D_L14_00161CE2");
extern f32   lightning_arc_alpha_pulse_phase __asm__("D_L14_00161CE4");
extern s32   lightning_arc_initial_lifetime_frames __asm__("D_L14_00161CEC");
extern f32   lightning_arc_inverse_fade_duration __asm__("D_L14_00161CF0");
extern s32   lightning_arc_sound_pending __asm__("D_L14_00161CF4");

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_PATH_LIGHTNING_CONTROLLER_GLOBALS_H */
