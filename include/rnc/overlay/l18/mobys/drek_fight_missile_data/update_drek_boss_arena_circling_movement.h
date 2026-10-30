#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_FIGHT_MISSILE_DATA_UPDATE_DREK_BOSS_ARENA_CIRCLING_MOVEMENT_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_FIGHT_MISSILE_DATA_UPDATE_DREK_BOSS_ARENA_CIRCLING_MOVEMENT_H

#include "types.h"

/* Scalar data for src/overlays/l18/mobys/drek_fight_missile.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 drek_boss_circling_default_near_hero_speed_gp __asm__("D_L18_00162350")
    __attribute__((sda));
extern f32 drek_boss_circling_default_far_hero_speed_gp __asm__("D_L18_00162354")
    __attribute__((sda));
extern f32 drek_boss_circling_base_far_hero_distance_gp __asm__("D_L18_00162358")
    __attribute__((sda));
extern f32 drek_boss_circling_base_near_hero_distance_gp __asm__("D_L18_0016235C")
    __attribute__((sda));
extern f32 drek_boss_circle_and_fire_radius_gp __asm__("D_L18_00162370") __attribute__((sda));
extern f32 drek_boss_default_circling_radius_gp __asm__("D_L18_00162374") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_DREK_FIGHT_MISSILE_DATA_UPDATE_DREK_BOSS_ARENA_CIRCLING_MOVEMENT_H */
