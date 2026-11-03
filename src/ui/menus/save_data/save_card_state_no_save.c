#include "sda.h"
#include "rnc/globals.h"
#include "rnc/save/memory_card_state.h"
#include "types.h"

void save_card_state_no_save(void) __asm__("FUN_00208c70");

extern s32 mode_freeze_flags_gp __asm__("D_0015EEB4") __attribute__((sda));

void save_card_state_no_save(void) {
    struct MemoryCardState *card_state = &memory_card_state;
    if (card_state->card[0].sync_result != 0) {
        save_card_state = CS_NOCARD;
        return;
    }
    if (mode_freeze_flags_gp & 2)
        save_card_state = CS_PROMPT_CREATE_SAVE;
}

extern __typeof__(save_card_state_no_save) func_00208C70 __attribute__((alias("FUN_00208c70")));
