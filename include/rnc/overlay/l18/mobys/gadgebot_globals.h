#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_GADGEBOT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_GADGEBOT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l18/mobys/gadgebot.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   gadgebot_joint_glows_last_queued_frame __asm__("D_L18_00162604");
extern int   gadgebot_glow_tint_start __asm__("D_L18_0016260C");
extern int   gadgebot_glow_tint_end __asm__("D_L18_00162610");
extern float gadgebot_glow_default_phase_speed_degrees_per_second __asm__("D_L18_00162614");
extern float gadgebot_glow_state_4_or_5_phase_speed_degrees_per_second __asm__("D_L18_00162618");
extern int   gadgebot_launch_glow_blend_minuend_selector __asm__("D_L18_0016262C");
extern int   gadgebot_launch_glow_blend_subtrahend_selector __asm__("D_L18_00162630");
extern int   gadgebot_launch_glow_blend_alpha_factor_selector __asm__("D_L18_00162634");
extern int   gadgebot_launch_glow_blend_addend_selector __asm__("D_L18_00162638");
extern int   gadgebot_launch_glow_tint_start __asm__("D_L18_0016263C");
extern int   gadgebot_launch_glow_tint_end __asm__("D_L18_00162640");
extern int   gadgebot_launch_glow_texture_index __asm__("D_L18_00162644");
extern float gadgebot_launch_glow_z_offset __asm__("D_L18_00162648");
extern float gadgebot_launch_glow_vertical_scale_adjustment __asm__("D_L18_0016264C");
extern float gadgebot_launch_glow_horizontal_scale_adjustment __asm__("D_L18_00162650");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_GADGEBOT_GLOBALS_H */
