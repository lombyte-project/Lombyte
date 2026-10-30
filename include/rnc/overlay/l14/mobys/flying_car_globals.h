#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_FLYING_CAR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_FLYING_CAR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/mobys/flying_car.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int   flying_car_path_reset_delay_frames __asm__("D_L14_0016227C");
extern float flying_car_rotation_oscillation_amplitude_gp __asm__("D_L14_00162290")
    __attribute__((sda));
extern float flying_car_rotation_oscillation_common_phase_update_gp __asm__("D_L14_00162294")
    __attribute__((sda));
extern float flying_car_rotation_oscillation_axis_phase_update_gp __asm__("D_L14_00162298")
    __attribute__((sda));
extern float flying_car_tracking_spring_gain_gp __asm__("D_L14_0016226C") __attribute__((sda));
extern float flying_car_motion_spring_damping_gp __asm__("D_L14_00162270") __attribute__((sda));
extern float flying_car_full_bank_yaw_error_gp __asm__("D_L14_00162278") __attribute__((sda));
extern float flying_car_max_sink_offset_rate_gp __asm__("D_L14_00162280") __attribute__((sda));
extern float flying_car_sink_offset_recovery_rate_gp __asm__("D_L14_00162288") __attribute__((sda));
extern float flying_car_burst_height_threshold_gp __asm__("D_L14_0016228C") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_FLYING_CAR_GLOBALS_H */
