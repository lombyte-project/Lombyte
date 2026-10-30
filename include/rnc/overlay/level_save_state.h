#ifndef LOMBYTE_RNC_OVERLAY_LEVEL_SAVE_STATE_H
#define LOMBYTE_RNC_OVERLAY_LEVEL_SAVE_STATE_H

#include "types.h"

/* The level's saved moby state. Pickups, gates and enemies index cleared by
   their Moby.save_id: 0 while the moby is still there, its mission slot + 2
   once it is collected or killed. */
typedef struct {
    u8 pad0[0x454];
    u8 cleared[1]; /* 0x454 */
} LevelSaveState;

extern LevelSaveState l01_save_state __asm__("D_L01_001BB6B0");
extern LevelSaveState l03_save_state __asm__("D_L03_001BB330");
extern LevelSaveState l10_save_state __asm__("D_L10_001BB6B0");
extern LevelSaveState l15_save_state __asm__("D_L15_001BB830");

#endif /* LOMBYTE_RNC_OVERLAY_LEVEL_SAVE_STATE_H */
