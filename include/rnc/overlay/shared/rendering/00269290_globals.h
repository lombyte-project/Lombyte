#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_00269290_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_00269290_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/rendering/00269290.c. */
extern int rotating_grayscale_particle_fade_reference_duration __asm__("D_L00_001601A4") __attribute__((sda));
extern int spinning_particle_default_initial_color_blend_source __asm__("D_L00_001601A8") __attribute__((sda));
extern int spinning_particle_default_initial_color_blend_target __asm__("D_L00_001601AC") __attribute__((sda));
extern int spinning_particle_default_final_color_blend_source __asm__("D_L00_001601B0") __attribute__((sda));
extern int spinning_particle_default_final_color_blend_target __asm__("D_L00_001601B4") __attribute__((sda));
extern int spinning_particle_default_lifetime_lower_bound __asm__("D_L00_001601C0") __attribute__((sda));
extern int spinning_particle_default_lifetime_upper_bound __asm__("D_L00_001601C4") __attribute__((sda));
extern int spinning_particle_alternate_initial_color_blend_source __asm__("D_L00_001601C8") __attribute__((sda));
extern int spinning_particle_alternate_initial_color_blend_target __asm__("D_L00_001601CC") __attribute__((sda));
extern int spinning_particle_alternate_final_color_blend_source __asm__("D_L00_001601D0") __attribute__((sda));
extern int spinning_particle_alternate_final_color_blend_target __asm__("D_L00_001601D4") __attribute__((sda));
extern int spinning_particle_alternate_lifetime_lower_bound __asm__("D_L00_001601E0") __attribute__((sda));
extern int spinning_particle_alternate_lifetime_upper_bound __asm__("D_L00_001601E4") __attribute__((sda));
extern float spinning_particle_default_final_size __asm__("D_L00_001601BC") __attribute__((sda));
extern float spinning_particle_alternate_final_size __asm__("D_L00_001601DC") __attribute__((sda));
extern float spinning_particle_vertical_step_scale __asm__("D_L00_001601EC") __attribute__((sda));
extern float two_point_particle_first_point_target_offset_scale __asm__("D_L00_001601F4") __attribute__((sda));
extern float two_point_particle_second_point_target_offset_scale __asm__("D_L00_001601F8") __attribute__((sda));
extern f32 animated_offset_particle_scalar __asm__("D_L00_00160200") __attribute__((sda));
extern s32 animated_offset_particle_packed_color __asm__("D_L00_00160204") __attribute__((sda));
extern f32 animated_offset_particle_offset_distance __asm__("D_L00_00160208") __attribute__((sda));
extern u8 animated_offset_particle_rotation_step __asm__("D_L00_0016020C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_00269290_GLOBALS_H */
