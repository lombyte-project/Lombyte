#ifndef LOMBYTE_RNC_OVERLAY_L09_MOBYS_FLOATING_SHIP_PIECE_DATA_UPDATE_FLOATING_SHIP_PIECE_H
#define LOMBYTE_RNC_OVERLAY_L09_MOBYS_FLOATING_SHIP_PIECE_DATA_UPDATE_FLOATING_SHIP_PIECE_H

#include "types.h"

/* Scalar data for src/overlays/l09/mobys/floating_ship_piece.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float floating_ship_piece_loaded_tilt_time_squared_gain_gp __asm__("D_L09_00161DD4")
    __attribute__((sda));
extern float floating_ship_piece_loaded_tilt_frame_scale_squared_gain_gp __asm__("D_L09_00161DD8")
    __attribute__((sda));
extern float floating_ship_piece_loaded_tilt_snap_tolerance_scale_gp __asm__("D_L09_00161DDC")
    __attribute__((sda));
extern float floating_ship_piece_return_tilt_time_squared_gain_gp __asm__("D_L09_00161DE0")
    __attribute__((sda));
extern float floating_ship_piece_return_tilt_frame_scale_squared_gain_gp __asm__("D_L09_00161DE4")
    __attribute__((sda));
extern float floating_ship_piece_return_tilt_snap_tolerance_scale_gp __asm__("D_L09_00161DE8")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L09_MOBYS_FLOATING_SHIP_PIECE_DATA_UPDATE_FLOATING_SHIP_PIECE_H */
