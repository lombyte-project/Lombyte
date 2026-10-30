/* Ported from rac1-decomp (src/game/menu.c, func_002096D8). */
#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"

void save_card_state_creating_save(void) __asm__("FUN_00208d60");

void save_card_state_creating_save(void) {
    struct MemoryCardState *card_state = &memory_card_state;
    if (card_state->state == 2 && card_state->pending_state < 0) {
        if (card_state->err != 0) {
            mode_freeze_state = 0x12;
            mode_freeze_flags |= 0x40;
            return;
        }
        card_state->pending_state = 7;
        card_state->active_card = 0;
        card_state->card[0].save_index = 0;
        card_state->pending_card = 0;
        mode_freeze_state = 0x10;
    }
}

extern __typeof__(save_card_state_creating_save) func_00208D60
    __attribute__((alias("FUN_00208d60")));
