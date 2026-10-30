#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DOOM_BOT_MOVEMENT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DOOM_BOT_MOVEMENT_H

#include "types.h"

void save_state_fields_to_hero(char *source_state) __asm__("FUN_L00_002beb90");
void restore_state_fields_from_hero(char *destination_state) __asm__("FUN_L00_002bebc8");
void save_state_vector_and_fields_to_hero(char *source_state) __asm__("FUN_L00_002bec00");
void restore_saved_state_fields_from_hero(char *destination_state) __asm__("FUN_L00_002bec48");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_DOOM_BOT_MOVEMENT_H */
