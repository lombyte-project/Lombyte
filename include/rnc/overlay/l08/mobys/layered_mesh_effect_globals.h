#ifndef LOMBYTE_RNC_OVERLAY_L08_MOBYS_LAYERED_MESH_EFFECT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L08_MOBYS_LAYERED_MESH_EFFECT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l08/mobys/layered_mesh_effect.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   scrolled_mesh_unscaled_fixed_alpha __asm__("D_L08_00161DD0");
extern int   scrolled_mesh_subtractive_base_fixed_alpha __asm__("D_L08_00161DD4");
extern int   scrolled_mesh_additive_base_fixed_alpha __asm__("D_L08_00161DD8");
extern int   offset_mesh_first_pass_fixed_alpha __asm__("D_L08_00161DF0");
extern int   offset_mesh_second_pass_fixed_alpha __asm__("D_L08_00161DF4");
extern int   pair_offset_mesh_fixed_alpha __asm__("D_L08_00161E00");
extern float layered_mesh_effect_center_y __asm__("D_L08_00161DC4");
extern float layered_mesh_effect_center_z __asm__("D_L08_00161DC8");
extern float layered_mesh_effect_culling_radius __asm__("D_L08_00161DCC");

#endif /* LOMBYTE_RNC_OVERLAY_L08_MOBYS_LAYERED_MESH_EFFECT_GLOBALS_H */
