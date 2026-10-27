#ifndef LOMBYTE_RNC_OVERLAY_L08_GAMEPLAY_ENTITIES_BOMB_DATA_UPDATE_ROCKET_H
#define LOMBYTE_RNC_OVERLAY_L08_GAMEPLAY_ENTITIES_BOMB_DATA_UPDATE_ROCKET_H

#include "types.h"

/* Scalar data for src/overlays/l08/gameplay/entities/bomb.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int rocket_guidance_effect_frame_count_gp __asm__("D_L08_00161A20") __attribute__((sda));
extern int rocket_guidance_effect_initial_color_gp __asm__("D_L08_00161A2C") __attribute__((sda));
extern int rocket_guidance_effect_second_color_gp __asm__("D_L08_00161A30") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L08_GAMEPLAY_ENTITIES_BOMB_DATA_UPDATE_ROCKET_H */
