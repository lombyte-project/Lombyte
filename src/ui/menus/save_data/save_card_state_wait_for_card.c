#include "sda.h"
#include "rnc/globals.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;

void save_card_state_wait_for_card(void) __asm__("FUN_002089d0");

void save_card_state_wait_for_card(void) {
    struct MemoryCardState *s = &memory_card_state;
    int v;
    mode_freeze_flags &= ~0x20;
    v = s->card[0].sync_result;
    if (v == 0) {
        mode_freeze_state = 9;
        return;
    }
    if (v == -1) {
        s->card[0].sync_result = 0;
        mode_freeze_state = 9;
        return;
    }
    if (v == -2)
        mode_freeze_state = 5;
}

extern __typeof__(save_card_state_wait_for_card) func_002089D0
    __attribute__((alias("FUN_002089d0")));
