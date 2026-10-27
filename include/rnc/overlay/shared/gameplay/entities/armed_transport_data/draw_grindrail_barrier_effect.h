#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_ARMED_TRANSPORT_DATA_DRAW_GRINDRAIL_BARRIER_EFFECT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_ARMED_TRANSPORT_DATA_DRAW_GRINDRAIL_BARRIER_EFFECT_H

#include "types.h"

/* Scalar data for src/overlays/shared/gameplay/entities/armed_transport.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int grindrail_barrier_ring_blend_start_color __asm__("D_L16_00161E18") __attribute__((sda));
extern int grindrail_barrier_ring_base_color __asm__("D_L16_00161E1C") __attribute__((sda));
extern int grindrail_barrier_accent_color __asm__("D_L16_00161E20") __attribute__((sda));
extern int grindrail_barrier_blend_minuend_selector __asm__("D_L16_00161E28") __attribute__((sda));
extern int grindrail_barrier_blend_subtrahend_selector __asm__("D_L16_00161E2C") __attribute__((sda));
extern int grindrail_barrier_blend_multiplier_selector __asm__("D_L16_00161E30") __attribute__((sda));
extern int grindrail_barrier_blend_addend_selector __asm__("D_L16_00161E34") __attribute__((sda));
extern int grindrail_barrier_ring_texture_id __asm__("D_L16_00161E38") __attribute__((sda));
extern int grindrail_barrier_accent_texture_id __asm__("D_L16_00161E3C") __attribute__((sda));
extern int grindrail_barrier_ring_quad_count __asm__("D_L16_00161E40") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_ARMED_TRANSPORT_DATA_DRAW_GRINDRAIL_BARRIER_EFFECT_H */
