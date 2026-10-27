#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_ARMED_TRANSPORT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_ARMED_TRANSPORT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/armed_transport.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float armed_transport_path_acceleration_coefficient __asm__("D_L16_00161D84");

/* Additional scalar data read by this unit. */
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

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_ARMED_TRANSPORT_GLOBALS_H */
