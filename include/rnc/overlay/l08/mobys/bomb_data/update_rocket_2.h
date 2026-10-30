#ifndef LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMB_DATA_UPDATE_ROCKET_2_H
#define LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMB_DATA_UPDATE_ROCKET_2_H

#include "types.h"

/* Scalar data for src/overlays/l08/mobys/bomb.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float rocket_guidance_effect_velocity_w_gp __asm__("D_L08_00161A24") __attribute__((sda));
extern float rocket_guidance_effect_auxiliary_vector_w_gp __asm__("D_L08_00161A28")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMB_DATA_UPDATE_ROCKET_2_H */
