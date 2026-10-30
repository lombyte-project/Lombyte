#ifndef LOMBYTE_RNC_OVERLAY_L09_MOBYS_ASTEROID_DATA_PLATFORM_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_L09_MOBYS_ASTEROID_DATA_PLATFORM_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/l09/mobys/asteroid.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float platform_active_tilt_error_gain_gp __asm__("D_L09_00161A04") __attribute__((sda));
extern float platform_active_tilt_velocity_damping_gp __asm__("D_L09_00161A08")
    __attribute__((sda));
extern float platform_active_tilt_velocity_limit_gp __asm__("D_L09_00161A0C") __attribute__((sda));
extern float platform_return_tilt_error_gain_gp __asm__("D_L09_00161A10") __attribute__((sda));
extern float platform_return_tilt_velocity_damping_gp __asm__("D_L09_00161A14")
    __attribute__((sda));
extern float platform_return_tilt_velocity_limit_gp __asm__("D_L09_00161A18") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L09_MOBYS_ASTEROID_DATA_PLATFORM_UPDATE_H */
