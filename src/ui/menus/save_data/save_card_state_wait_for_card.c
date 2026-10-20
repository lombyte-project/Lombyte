#include "sda.h"
#include "rnc/globals.h"
#include "rnc/storage/memory_card/memory_card_state.h"
#include "types.h"

void save_card_state_wait_for_card(void) __asm__("FUN_002089d0");

extern s32 mode_freeze_state_macro __asm__("D_0015EEB0") MACRO_ADDR;
void save_card_state_wait_for_card(void) {
    struct MemoryCardState *card_state = &memory_card_state;
    int v;
    mode_freeze_flags &= ~0x20;
    v = card_state->card[0].sync_result;
    if (v == 0) {
        mode_freeze_state_macro = 9;
        return;
    }
    if (v == -1) {
        card_state->card[0].sync_result = 0;
        mode_freeze_state_macro = 9;
        return;
    }
    if (v == -2)
        mode_freeze_state_macro = 5;
}

extern __typeof__(save_card_state_wait_for_card) func_002089D0
    __attribute__((alias("FUN_002089d0")));
