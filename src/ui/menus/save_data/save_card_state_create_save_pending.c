#include "sda.h"
#include "rnc/globals.h"
#include "rnc/storage/memory_card/memory_card_state.h"

void save_card_state_create_save_pending(void) __asm__("FUN_00208d20");

void save_card_state_create_save_pending(void) {
    struct MemoryCardState *s = &memory_card_state;
    if (s->state == 2 && s->pending_state < 0) {
        s->pending_state = 9;
        s->pending_card = 0;
        mode_freeze_state = 0xF;
    }
}

extern __typeof__(save_card_state_create_save_pending) func_00208D20
    __attribute__((alias("FUN_00208d20")));
