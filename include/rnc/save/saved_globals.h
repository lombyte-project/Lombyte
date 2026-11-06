#ifndef LOMBYTE_RNC_SAVE_SAVED_GLOBALS_H
#define LOMBYTE_RNC_SAVE_SAVED_GLOBALS_H

#include "types.h"
#include "rnc/save/memory_card_state.h"

/* Globals the save block list mc_gamedata (rnc/save/mc_data.h) stores and no
   other header declares yet; each type follows the size its block saves. */

/* The sceCdReadClock time taken when the game is saved. */
extern struct MemoryCardSaveClock D_0015EE98 __asm__("D_0015EE98");

extern s32 D_0015ED90 __asm__("D_0015ED90");
extern s32 D_0015ED94 __asm__("D_0015ED94");
extern s32 D_0015ED9C __asm__("D_0015ED9C");
/* Indexed by a configured text label's selector (rebuild_configured_text_label_list). */
extern u8  selector_available[12] __asm__("D_0015EDC0");
extern s32 D_0015EE28 __asm__("D_0015EE28");
extern s32 D_0015EE2C __asm__("D_0015EE2C");
extern s32 D_0015EEAC __asm__("D_0015EEAC");
/* One time per help record (148), in units of 600 frames: level 5's
   help_hints subtracts it times 600 from scale_game_frames. */
extern u16 D_00141720[148] __asm__("D_00141720");

#endif /* LOMBYTE_RNC_SAVE_SAVED_GLOBALS_H */
