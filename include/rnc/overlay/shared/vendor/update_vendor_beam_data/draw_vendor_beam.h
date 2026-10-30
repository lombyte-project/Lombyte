#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_VENDOR_003015D0_DATA_DRAW_VENDOR_BEAM_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_VENDOR_003015D0_DATA_DRAW_VENDOR_BEAM_H

#include "types.h"

/* Scalar data for src/overlays/shared/vendor/update_vendor_beam.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int vendor_beam_blend_minuend_selector_gp __asm__("D_L14_001620E0") __attribute__((sda));
extern int vendor_beam_blend_subtrahend_selector_gp __asm__("D_L14_001620E4") __attribute__((sda));
extern int vendor_beam_blend_alpha_factor_selector_gp __asm__("D_L14_001620E8")
    __attribute__((sda));
extern int vendor_beam_blend_addend_selector_gp __asm__("D_L14_001620EC") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_VENDOR_003015D0_DATA_DRAW_VENDOR_BEAM_H */
