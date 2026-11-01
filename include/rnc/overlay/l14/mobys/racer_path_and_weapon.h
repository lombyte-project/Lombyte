#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_RACER_PATH_AND_WEAPON_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_RACER_PATH_AND_WEAPON_H

#include "types.h"

struct Moby;

void initialize_racer_path_tracking(struct Moby *) __asm__("FUN_L14_002d6358");
int  interpolate_racer_to_path_start(struct Moby *) __asm__("FUN_L14_002d64d0");
void update_racer_path_and_weapon(struct Moby *) __asm__("FUN_L14_002d67e8");
void advance_moby_along_path(struct Moby *) __asm__("FUN_L14_002d7198");
void update_moby_joint_visual_effects(struct Moby *) __asm__("FUN_L14_002d7490");
void reset_racer_group_to_hero_path_position(struct Moby *) __asm__("FUN_L14_002d6570");

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_RACER_PATH_AND_WEAPON_H */
