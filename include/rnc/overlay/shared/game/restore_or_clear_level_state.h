#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAME_RESTORE_OR_CLEAR_LEVEL_STATE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAME_RESTORE_OR_CLEAR_LEVEL_STATE_H

#include "types.h"

void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");

void restore_or_clear_level_state(void) __asm__("FUN_L00_00285008");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAME_RESTORE_OR_CLEAR_LEVEL_STATE_H */
