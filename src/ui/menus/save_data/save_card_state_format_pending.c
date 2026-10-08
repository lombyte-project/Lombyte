#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_state __asm__("D_0015EEB0") __attribute__((sda));

void save_card_state_format_pending(void) __asm__("FUN_00208af8");

void save_card_state_format_pending(void) {
    struct MemoryCardState *s = &memory_card_state;
    int pending = s->pending_state;
    s->card[0].sync_result = 0;
    if (pending < 0) {
        s->pending_card = 0;
        s->pending_state = 3;
    }
    mode_freeze_state = 8;
}

extern __typeof__(save_card_state_format_pending) func_00208AF8
    __attribute__((alias("FUN_00208af8")));
