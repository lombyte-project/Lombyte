#ifndef LOMBYTE_RNC_OVERLAY_L15_MOBYS_RADIAL_RIPPLE_MESH_DATA_DRAW_MOBY_RADIAL_RIPPLE_MESH_BUFFERS_H
#define LOMBYTE_RNC_OVERLAY_L15_MOBYS_RADIAL_RIPPLE_MESH_DATA_DRAW_MOBY_RADIAL_RIPPLE_MESH_BUFFERS_H

#include "types.h"

/* Scalar data for src/overlays/l15/mobys/radial_ripple_mesh.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float ripple_mesh_buffers_radial_wavelength_gp __asm__("D_L15_001620F8")
    __attribute__((sda));
extern float ripple_mesh_buffers_phase_advance_divisor_gp __asm__("D_L15_001620FC")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L15_MOBYS_RADIAL_RIPPLE_MESH_DATA_DRAW_MOBY_RADIAL_RIPPLE_MESH_BUFFERS_H */
