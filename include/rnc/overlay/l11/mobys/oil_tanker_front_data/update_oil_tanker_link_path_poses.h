#ifndef LOMBYTE_RNC_OVERLAY_L11_MOBYS_OIL_TANKER_FRONT_DATA_UPDATE_OIL_TANKER_LINK_PATH_POSES_H
#define LOMBYTE_RNC_OVERLAY_L11_MOBYS_OIL_TANKER_FRONT_DATA_UPDATE_OIL_TANKER_LINK_PATH_POSES_H

#include "types.h"

/* Scalar data for src/overlays/l11/mobys/oil_tanker_front.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float oil_tanker_link_spacing_threshold_gp __asm__("D_L11_00162324") __attribute__((sda));
extern float oil_tanker_spacing_reference_offset_gp __asm__("D_L11_00162328") __attribute__((sda));
extern s32 oil_tanker_link_marker_cooldown_frames_gp __asm__("D_L11_0016232C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L11_MOBYS_OIL_TANKER_FRONT_DATA_UPDATE_OIL_TANKER_LINK_PATH_POSES_H */
