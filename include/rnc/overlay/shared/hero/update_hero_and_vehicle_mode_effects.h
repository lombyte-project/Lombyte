#ifndef LOMBYTE_RNC_OVERLAY_SHARED_HERO_UPDATE_HERO_AND_VEHICLE_MODE_EFFECTS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_HERO_UPDATE_HERO_AND_VEHICLE_MODE_EFFECTS_H

#include "types.h"

void set_hero_state_for_vehicle_mode(void) __asm__("FUN_L15_0021ab60");
void restore_saved_hero_moby_and_clear_vehicle_mode(void) __asm__("FUN_L15_00208ca8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_HERO_UPDATE_HERO_AND_VEHICLE_MODE_EFFECTS_H */
