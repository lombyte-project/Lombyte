#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_DATA_DRAW_ROCKET_TARGET_PULSE_QUAD_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_DATA_DRAW_ROCKET_TARGET_PULSE_QUAD_H

#include "types.h"

/* Scalar data for src/overlays/l18/mobys/rocket.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int rocket_target_pulse_blend_minuend_selector_gp __asm__("D_L18_00161970")
    __attribute__((sda));
extern int rocket_target_pulse_blend_subtrahend_selector_gp __asm__("D_L18_00161974")
    __attribute__((sda));
extern int rocket_target_pulse_blend_alpha_factor_selector_gp __asm__("D_L18_00161978")
    __attribute__((sda));
extern int rocket_target_pulse_blend_addend_selector_gp __asm__("D_L18_0016197C")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_DATA_DRAW_ROCKET_TARGET_PULSE_QUAD_H */
