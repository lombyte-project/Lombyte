#ifndef LOMBYTE_RNC_OVERLAY_L17_MOBYS_DOOR_DATA_DRAW_RIPPLE_RING_H
#define LOMBYTE_RNC_OVERLAY_L17_MOBYS_DOOR_DATA_DRAW_RIPPLE_RING_H

#include "types.h"

/* Scalar data for src/overlays/l17/mobys/door.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float ring_ripple_radial_phase_distance_scale_gp __asm__("D_L17_001622C0")
    __attribute__((sda));
extern float ring_ripple_phase_period_gp __asm__("D_L17_001622C4") __attribute__((sda));
extern float ring_ripple_z_amplitude_gp __asm__("D_L17_001622C8") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L17_MOBYS_DOOR_DATA_DRAW_RIPPLE_RING_H */
