#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_WALLOPER_DATA_DRAW_BEAM_RING_AND_RAIL_EFFECTS_2_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_WALLOPER_DATA_DRAW_BEAM_RING_AND_RAIL_EFFECTS_2_H

#include "types.h"

/* Scalar data for src/overlays/shared/weapons/walloper.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float beam_ring_texture_v_scroll_speed_gp __asm__("D_L00_00161728") __attribute__((sda));
extern int   beam_rail_first_pass_alpha_gp __asm__("D_L00_0016176C") __attribute__((sda));
extern int   beam_rail_second_pass_alpha_gp __asm__("D_L00_00161770") __attribute__((sda));
extern float beam_rail_first_pass_half_width_gp __asm__("D_L00_00161774") __attribute__((sda));
extern float beam_rail_second_pass_half_width_gp __asm__("D_L00_00161778") __attribute__((sda));
extern float beam_rail_second_pass_segment_extension_factor_gp __asm__("D_L00_0016177C")
    __attribute__((sda));
extern int beam_first_rail_second_pass_base_vertex_color_gp __asm__("D_L00_00161784")
    __attribute__((sda));
extern int beam_second_rail_second_pass_base_vertex_color_gp __asm__("D_L00_00161788")
    __attribute__((sda));
extern float beam_rail_first_pass_texture_u_offset_gp __asm__("D_L00_0016178C")
    __attribute__((sda));
extern float beam_rail_first_pass_texture_u_scroll_speed_gp __asm__("D_L00_00161790")
    __attribute__((sda));
extern int beam_ring_default_vertex_color_gp __asm__("D_L00_00161794") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_WALLOPER_DATA_DRAW_BEAM_RING_AND_RAIL_EFFECTS_2_H */
