#include "sda.h"
#include "rnc/globals.h"
#include "rnc/save/memory_card_state.h"
#include "types.h"

void save_card_state_wait_for_card(void) __asm__("FUN_002089d0");

extern s32 mode_freeze_state_macro __asm__("D_0015EEB0") MACRO_ADDR;

void save_card_state_wait_for_card(void) {
    struct MemoryCardState *card_state = &memory_card_state;
    int                     sync_result;
    mode_freeze_flags &= ~0x20;
    sync_result = card_state->card[0].sync_result;
    if (sync_result == 0) {
        mode_freeze_state_macro = 9;
        return;
    }
    if (sync_result == -1) {
        card_state->card[0].sync_result = 0;
        mode_freeze_state_macro = 9;
        return;
    }
    if (sync_result == -2)
        mode_freeze_state_macro = 5;
}

extern __typeof__(save_card_state_wait_for_card) func_002089D0
    __attribute__((alias("FUN_002089d0")));
