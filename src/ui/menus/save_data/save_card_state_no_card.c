#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;

void save_card_state_no_card(void) __asm__("FUN_002089a8");

void save_card_state_no_card(void) {
    struct MemoryCardState *s = &memory_card_state;
    s->pending_state = -1;
    mode_freeze_state = 4;
    s->pending_card = -1;
}

extern __typeof__(save_card_state_no_card) func_002089A8 __attribute__((alias("FUN_002089a8")));
