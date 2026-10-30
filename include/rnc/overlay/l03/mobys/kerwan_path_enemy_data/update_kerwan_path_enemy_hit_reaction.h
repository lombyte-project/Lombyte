#ifndef LOMBYTE_RNC_OVERLAY_L03_MOBYS_KERWAN_PATH_ENEMY_DATA_UPDATE_KERWAN_PATH_ENEMY_HIT_REACTION_H
#define LOMBYTE_RNC_OVERLAY_L03_MOBYS_KERWAN_PATH_ENEMY_DATA_UPDATE_KERWAN_PATH_ENEMY_HIT_REACTION_H

#include "types.h"

/* Scalar data for src/overlays/l03/mobys/kerwan_path_enemy.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float kerwan_path_enemy_knockback_horizontal_distance __asm__("D_L03_001618E4")
    __attribute__((sda));
extern float kerwan_path_enemy_knockback_horizontal_deceleration __asm__("D_L03_001618E8")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L03_MOBYS_KERWAN_PATH_ENEMY_DATA_UPDATE_KERWAN_PATH_ENEMY_HIT_REACTION_H */
