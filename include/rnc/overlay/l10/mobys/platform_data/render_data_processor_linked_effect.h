#ifndef LOMBYTE_RNC_OVERLAY_L10_MOBYS_PLATFORM_DATA_RENDER_DATA_PROCESSOR_LINKED_EFFECT_H
#define LOMBYTE_RNC_OVERLAY_L10_MOBYS_PLATFORM_DATA_RENDER_DATA_PROCESSOR_LINKED_EFFECT_H

#include "types.h"

/* Scalar data for src/overlays/l10/mobys/platform.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int data_processor_linked_effect_blend_minuend_selector_gp __asm__("D_L10_00161C70")
    __attribute__((sda));
extern int data_processor_linked_effect_blend_subtrahend_selector_gp __asm__("D_L10_00161C74")
    __attribute__((sda));
extern int data_processor_linked_effect_blend_alpha_factor_selector_gp __asm__("D_L10_00161C78")
    __attribute__((sda));
extern int data_processor_linked_effect_blend_addend_selector_gp __asm__("D_L10_00161C7C")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L10_MOBYS_PLATFORM_DATA_RENDER_DATA_PROCESSOR_LINKED_EFFECT_H */
