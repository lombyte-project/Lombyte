#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_OSCILLATING_TARGET_LINK_DATA_DRAW_OSCILLATING_TARGET_LINK_STRIPS_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_OSCILLATING_TARGET_LINK_DATA_DRAW_OSCILLATING_TARGET_LINK_STRIPS_H

#include "types.h"

/* Scalar data for src/overlays/l12/mobys/oscillating_target_link.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int oscillating_link_strip_blend_minuend_color_selector __asm__("D_L12_00161DCC")
    __attribute__((sda));
extern int oscillating_link_strip_blend_subtrahend_color_selector __asm__("D_L12_00161DD0")
    __attribute__((sda));
extern int oscillating_link_strip_blend_alpha_factor_selector __asm__("D_L12_00161DD4")
    __attribute__((sda));
extern int oscillating_link_strip_blend_addend_color_selector __asm__("D_L12_00161DD8")
    __attribute__((sda));
extern int oscillating_link_strip_fade_source_color __asm__("D_L12_00161DEC") __attribute__((sda));
extern int oscillating_link_strip_fade_target_color __asm__("D_L12_00161DF0") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L12_MOBYS_OSCILLATING_TARGET_LINK_DATA_DRAW_OSCILLATING_TARGET_LINK_STRIPS_H */
