#ifndef LOMBYTE_RNC_OVERLAY_L08_MOBYS_BATALIA_TEXTURED_EFFECT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L08_MOBYS_BATALIA_TEXTURED_EFFECT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l08/mobys/batalia_textured_effect.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float batalia_effect_reference_position_y __asm__("D_L08_00162384");
extern float batalia_effect_reference_position_z __asm__("D_L08_00162388");
extern float batalia_effect_culling_radius __asm__("D_L08_0016238C");
extern int   batalia_texture_3a_blend_factor __asm__("D_L08_00162390");
extern int   batalia_texture_3b_pair_one_blend_factor_base __asm__("D_L08_00162394");
extern int   batalia_texture_3b_pair_two_blend_factor_base __asm__("D_L08_00162398");
extern int   batalia_texture_3c_pair_zero_blend_factor __asm__("D_L08_001623B0");
extern int   batalia_texture_3c_pair_one_blend_factor __asm__("D_L08_001623B4");
extern int   batalia_texture_3d_blend_factor __asm__("D_L08_001623C0");

#endif /* LOMBYTE_RNC_OVERLAY_L08_MOBYS_BATALIA_TEXTURED_EFFECT_GLOBALS_H */
