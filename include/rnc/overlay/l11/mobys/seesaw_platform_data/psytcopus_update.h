#ifndef LOMBYTE_RNC_OVERLAY_L11_MOBYS_SEESAW_PLATFORM_DATA_PSYTCOPUS_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_L11_MOBYS_SEESAW_PLATFORM_DATA_PSYTCOPUS_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/l11/mobys/seesaw_platform.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float psytcopus_knockback_base_gravity_gp __asm__("D_L11_00161F88") __attribute__((sda));
extern float psytcopus_hurt_knockback_base_vertical_speed_gp __asm__("D_L11_00161F8C")
    __attribute__((sda));
extern float psytcopus_hurt_knockback_base_horizontal_speed_gp __asm__("D_L11_00161F90")
    __attribute__((sda));
extern float psytcopus_death_knockback_base_vertical_speed_gp __asm__("D_L11_00161F94")
    __attribute__((sda));
extern float psytcopus_death_knockback_base_horizontal_speed_gp __asm__("D_L11_00161F98")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L11_MOBYS_SEESAW_PLATFORM_DATA_PSYTCOPUS_UPDATE_H */
