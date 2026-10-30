#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TESLA_CLAW_BEAM_DATA_DRAW_BEAM_AND_ARC_TWO_PASS_QUADS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TESLA_CLAW_BEAM_DATA_DRAW_BEAM_AND_ARC_TWO_PASS_QUADS_H

#include "types.h"

/* Scalar data for src/overlays/shared/weapons/tesla_claw_beam.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int beam_and_arc_first_pass_texture_index_gp __asm__("D_L00_00161638") __attribute__((sda));
extern int beam_and_arc_second_pass_texture_index_gp __asm__("D_L00_0016163C") __attribute__((sda));
extern int beam_and_arc_first_pass_base_vertex_color_gp __asm__("D_L00_001615F4")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_TESLA_CLAW_BEAM_DATA_DRAW_BEAM_AND_ARC_TWO_PASS_QUADS_H */
