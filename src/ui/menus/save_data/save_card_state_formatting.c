/* Ported from rac1-decomp (src/game/menu.c, func_00209448). */
#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"

void save_card_state_formatting(void) __asm__("FUN_00208b28");

void save_card_state_formatting(void) {
    struct MemoryCardState *card_state = &memory_card_state;
    if (card_state->state == 2 && card_state->pending_state < 0) {
        if (card_state->err != 0) {
            mode_freeze_state = 0x11;
            mode_freeze_flags |= 0x40;
            return;
        }
        mode_freeze_state = 0xE;
    }
}

extern __typeof__(save_card_state_formatting) func_00208B28 __attribute__((alias("FUN_00208b28")));
