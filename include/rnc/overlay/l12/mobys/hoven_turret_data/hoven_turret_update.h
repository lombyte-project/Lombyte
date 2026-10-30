#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_HOVEN_TURRET_DATA_HOVEN_TURRET_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_HOVEN_TURRET_DATA_HOVEN_TURRET_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/l12/mobys/hoven_turret.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float hoven_turret_destruction_view_offset_distance_min __asm__("D_L12_00161E60")
    __attribute__((sda));
extern float hoven_turret_destruction_view_offset_distance_max __asm__("D_L12_00161E64")
    __attribute__((sda));
extern float hoven_turret_pitch_min __asm__("D_L12_00161EA8") __attribute__((sda));
extern float hoven_turret_pitch_max __asm__("D_L12_00161EAC") __attribute__((sda));
extern float hoven_turret_yaw_input_rate __asm__("D_L12_00161ED0") __attribute__((sda));
extern float hoven_turret_pitch_input_rate __asm__("D_L12_00161ED4") __attribute__((sda));
extern float hoven_turret_yaw_acceleration_rate __asm__("D_L12_00161ED8") __attribute__((sda));
extern float hoven_turret_pitch_acceleration_rate __asm__("D_L12_00161EDC") __attribute__((sda));
extern float hoven_turret_yaw_braking_rate __asm__("D_L12_00161EE0") __attribute__((sda));
extern float hoven_turret_pitch_braking_rate __asm__("D_L12_00161EE4") __attribute__((sda));
extern float hoven_turret_firing_offset_rate __asm__("D_L12_00161EE8") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L12_MOBYS_HOVEN_TURRET_DATA_HOVEN_TURRET_UPDATE_H */
