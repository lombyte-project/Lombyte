/* Ported from rac1-decomp (src/game/menu.c, func_002094E0). */
#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"

void save_card_state_check_save(void) __asm__("FUN_00208bc0");

void save_card_state_check_save(void) {
    struct MemoryCardState *card_state = &memory_card_state;
    if (card_state->state == 2 && card_state->pending_state < 0) {
        card_state->pending_state = 7;
        card_state->pending_card = 0;
        mode_freeze_state = 0xB;
    }
}

extern __typeof__(save_card_state_check_save) func_00208BC0 __attribute__((alias("FUN_00208bc0")));
