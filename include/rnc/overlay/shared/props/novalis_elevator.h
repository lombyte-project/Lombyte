#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_NOVALIS_ELEVATOR_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_NOVALIS_ELEVATOR_H

#include "types.h"

struct Moby;

void update_nearest_area_moby_selection(struct Moby *) __asm__("FUN_L01_002f2eb8");
void sandmouse_house_update(u8 *) __asm__("FUN_L01_002f2b68");
void update_moby_motion_collision_and_lifetime(s8 *) __asm__("FUN_L01_002c7530");
void render_water_grids(void *, s32) __asm__("FUN_L01_002b91c8");
void update_moby_motion_collision_and_burst(s8 *) __asm__("FUN_L01_002eda00");
void gunship_follow_path(void *) __asm__("FUN_L01_002f5168");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_NOVALIS_ELEVATOR_H */
