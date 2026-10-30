#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"

void save_card_state_no_card(void) __asm__("FUN_002089a8");

void save_card_state_no_card(void) {
    struct MemoryCardState *card_state = &memory_card_state;
    card_state->pending_state = -1;
    mode_freeze_state = 4;
    card_state->pending_card = -1;
}

extern __typeof__(save_card_state_no_card) func_002089A8 __attribute__((alias("FUN_002089a8")));
