#ifndef LOMBYTE_RNC_OVERLAY_L02_MOBYS_TRANSPORT_POD_DATA_TRANSPORT_POD_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_L02_MOBYS_TRANSPORT_POD_DATA_TRANSPORT_POD_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/l02/mobys/transport_pod.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float transport_pod_configured_rail_a_height_offset_gp __asm__("D_L02_00161C20")
    __attribute__((sda));
extern float transport_pod_configured_rail_b_height_offset_gp __asm__("D_L02_00161C24")
    __attribute__((sda));
extern float transport_pod_rail_cycle_enabled_gp __asm__("D_L02_00161C40") __attribute__((sda));
extern float transport_pod_path_markers_enabled_gp __asm__("D_L02_00161C44") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L02_MOBYS_TRANSPORT_POD_DATA_TRANSPORT_POD_UPDATE_H */
