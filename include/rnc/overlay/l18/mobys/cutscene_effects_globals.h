#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_CUTSCENE_EFFECTS_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_CUTSCENE_EFFECTS_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l18/mobys/cutscene_effects.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float deplanetizer_camera_quad_scale_ramp_start_value __asm__("D_L18_00162490");
extern float deplanetizer_camera_quad_scale_ramp_end_value __asm__("D_L18_00162494");
extern int   deplanetizer_camera_quad_scale_ramp_end_frame __asm__("D_L18_0016249C");
extern int   deplanetizer_camera_quad_end_frame __asm__("D_L18_001624A0");
extern int   deplanetizer_glow_alpha_ramp_end_frame __asm__("D_L18_001624C0");
extern int   deplanetizer_three_quad_scale_ramp_start_frame __asm__("D_L18_001624C4");
extern int   deplanetizer_three_quad_scale_ramp_end_frame __asm__("D_L18_001624C8");
extern int   deplanetizer_camera_quad_start_frame __asm__("D_L18_00162498");
extern int   deplanetizer_glow_alpha_ramp_start_frame __asm__("D_L18_001624BC");
extern int   deplanetizer_glow_quad_alpha __asm__("D_L18_00162524");
extern float deplanetizer_camera_quad_base_scale __asm__("D_L18_00162520");
extern float deplanetizer_three_quad_local_x_scale __asm__("D_L18_0016252C");
extern int   deplanetizer_camera_quad_first_color __asm__("D_L18_001624F8");
extern int   deplanetizer_camera_quad_smaller_color __asm__("D_L18_001624FC");
extern int   deplanetizer_glow_local_offset_x __asm__("D_L18_00162560");
extern int   deplanetizer_glow_local_offset_y __asm__("D_L18_00162564");
extern float deplanetizer_glow_local_offset_z __asm__("D_L18_00162568");
extern float deplanetizer_glow_local_offset_w __asm__("D_L18_0016256C");
extern int   deplanetizer_glow_first_quad_base_tint __asm__("D_L18_00162504");
extern float deplanetizer_glow_first_quad_scale __asm__("D_L18_00162508");
extern float deplanetizer_glow_second_quad_scale __asm__("D_L18_0016250C");
extern int   deplanetizer_glow_second_quad_base_tint __asm__("D_L18_00162510");
extern float deplanetizer_first_quad_local_z_scale __asm__("D_L18_00162514");
extern float deplanetizer_second_quad_local_z_scale __asm__("D_L18_00162518");
extern float deplanetizer_third_quad_local_z_scale __asm__("D_L18_0016251C");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_CUTSCENE_EFFECTS_GLOBALS_H */
