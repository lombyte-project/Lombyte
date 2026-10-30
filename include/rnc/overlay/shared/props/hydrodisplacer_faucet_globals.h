#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_HYDRODISPLACER_FAUCET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_HYDRODISPLACER_FAUCET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/props/hydrodisplacer_faucet.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int wobble_quad_blend_a_selector __asm__("D_L05_00161CF4");
extern int wobble_quad_blend_b_selector __asm__("D_L05_00161CF8");
extern int wobble_quad_blend_c_selector __asm__("D_L05_00161CFC");
extern int wobble_quad_blend_d_selector __asm__("D_L05_00161D00");
extern int wobble_quad_start_color __asm__("D_L05_00161D04");
extern int wobble_quad_end_color __asm__("D_L05_00161D08");
extern int wobble_quad_texture_index __asm__("D_L05_00161D0C");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_HYDRODISPLACER_FAUCET_GLOBALS_H */
