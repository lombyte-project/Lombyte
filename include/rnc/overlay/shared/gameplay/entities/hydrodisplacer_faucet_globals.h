#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_HYDRODISPLACER_FAUCET_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_HYDRODISPLACER_FAUCET_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/hydrodisplacer_faucet.c. */
extern int wobble_quad_blend_a_selector __asm__("D_L05_00161CF4") __attribute__((sda));
extern int wobble_quad_blend_b_selector __asm__("D_L05_00161CF8") __attribute__((sda));
extern int wobble_quad_blend_c_selector __asm__("D_L05_00161CFC") __attribute__((sda));
extern int wobble_quad_blend_d_selector __asm__("D_L05_00161D00") __attribute__((sda));
extern int wobble_quad_start_color __asm__("D_L05_00161D04") __attribute__((sda));
extern int wobble_quad_end_color __asm__("D_L05_00161D08") __attribute__((sda));
extern int wobble_quad_texture_index __asm__("D_L05_00161D0C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_HYDRODISPLACER_FAUCET_GLOBALS_H */
