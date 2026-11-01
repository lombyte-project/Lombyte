#ifndef LOMBYTE_RNC_GAME_H
#define LOMBYTE_RNC_GAME_H

#include "types.h"

void dispatch_game_state_update(void) __asm__("FUN_00230ee8");
void load_level_zero_preserving_progress(s32) __asm__("FUN_00226b08");
s32  process_global_state_flags(void) __asm__("FUN_00222290");
void start_level(int) __asm__("FUN_001e9488");
void update_mode_freeze(void) __asm__("FUN_001fce28");
void update_resident_gameplay_state(void) __asm__("FUN_0022f778");

#endif /* LOMBYTE_RNC_GAME_H */
