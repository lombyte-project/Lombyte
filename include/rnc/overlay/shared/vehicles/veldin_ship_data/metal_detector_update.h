#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_VELDIN_SHIP_DATA_METAL_DETECTOR_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_VELDIN_SHIP_DATA_METAL_DETECTOR_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/shared/vehicles/veldin_ship.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 metal_detector_aim_yaw_limit_degrees_gp __asm__("D_L00_00161950") __attribute__((sda));
extern f32 metal_detector_aim_pitch_limit_degrees_gp __asm__("D_L00_00161954") __attribute__((sda));
extern s32 metal_detector_aim_perpendicular_cross_direction_row_index_gp __asm__("D_L00_0016191C")
    __attribute__((sda));
extern s32 metal_detector_aim_direction_row_index_gp __asm__("D_L00_00161920") __attribute__((sda));
extern s32 metal_detector_aim_perpendicular_row_index_gp __asm__("D_L00_00161924")
    __attribute__((sda));
extern s32 metal_detector_aim_direction_normalization_length_gp __asm__("D_L00_0016192C")
    __attribute__((sda));
extern s32 metal_detector_aim_perpendicular_normalization_length_gp __asm__("D_L00_00161930")
    __attribute__((sda));
extern f32 metal_detector_aim_cross_reference_x_gp __asm__("D_L00_00161940") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_VELDIN_SHIP_DATA_METAL_DETECTOR_UPDATE_H */
