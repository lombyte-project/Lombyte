#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAME_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAME_H

#include "types.h"

void init_level_runtime_state(void) __asm__("FUN_L00_0023d028");
void deactivate_recorded_moby(void) __asm__("FUN_L00_0028e990");
void initialize_moby_state_record(char *) __asm__("FUN_L00_0028e8a0");
void prepare_level_entry_transition(void) __asm__("FUN_L00_0028eed0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAME_H */
