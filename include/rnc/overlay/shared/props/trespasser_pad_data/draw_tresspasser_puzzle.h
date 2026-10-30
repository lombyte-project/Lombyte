#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_TRESPASSER_PAD_DATA_DRAW_TRESSPASSER_PUZZLE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_TRESPASSER_PAD_DATA_DRAW_TRESSPASSER_PUZZLE_H

#include "types.h"

/* Scalar data for src/overlays/shared/props/trespasser_pad.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int tresspasser_third_ring_arc_second_boundary_radius_gp __asm__("D_L02_00161B30")
    __attribute__((sda));
extern int tresspasser_third_ring_arc_first_boundary_radius_gp __asm__("D_L02_00161B34")
    __attribute__((sda));
extern int tresspasser_third_ring_arrow_distance_gp __asm__("D_L02_00161B38") __attribute__((sda));
extern int tresspasser_second_ring_arc_second_boundary_radius_gp __asm__("D_L02_00161B3C")
    __attribute__((sda));
extern int tresspasser_second_ring_arc_first_boundary_radius_gp __asm__("D_L02_00161B40")
    __attribute__((sda));
extern int tresspasser_second_ring_arrow_distance_gp __asm__("D_L02_00161B44") __attribute__((sda));
extern int tresspasser_first_ring_arc_second_boundary_radius_gp __asm__("D_L02_00161B48")
    __attribute__((sda));
extern int tresspasser_first_ring_arc_first_boundary_radius_gp __asm__("D_L02_00161B4C")
    __attribute__((sda));
extern int tresspasser_first_ring_arrow_distance_gp __asm__("D_L02_00161B50") __attribute__((sda));
extern int tresspasser_marker_arc_second_boundary_radius_gp __asm__("D_L02_00161B54")
    __attribute__((sda));
extern int tresspasser_marker_arc_first_boundary_radius_gp __asm__("D_L02_00161B58")
    __attribute__((sda));
extern int tresspasser_unintercepted_strip_endpoint_distance_gp __asm__("D_L02_00161B5C")
    __attribute__((sda));
extern int tresspasser_selected_ring_base_color_gp __asm__("D_L02_00161BA0") __attribute__((sda));
extern int tresspasser_unselected_ring_color_gp __asm__("D_L02_00161BA4") __attribute__((sda));
extern int tresspasser_arrow_unused_argument_gp __asm__("D_L02_00161BA8") __attribute__((sda));
extern int tresspasser_default_radial_strip_color_gp __asm__("D_L02_00161BAC") __attribute__((sda));
extern int tresspasser_reached_marker_strip_color_gp __asm__("D_L02_00161BB0") __attribute__((sda));
extern int tresspasser_reverse_radial_strip_color_gp __asm__("D_L02_00161BB4") __attribute__((sda));
extern int tresspasser_unreached_marker_arc_flag_clear_color_gp __asm__("D_L02_00161BBC")
    __attribute__((sda));
extern int tresspasser_unreached_marker_arc_flag_set_color_gp __asm__("D_L02_00161BC0")
    __attribute__((sda));
extern int tresspasser_reached_marker_arc_color_gp __asm__("D_L02_00161BC4") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_TRESPASSER_PAD_DATA_DRAW_TRESSPASSER_PUZZLE_H */
