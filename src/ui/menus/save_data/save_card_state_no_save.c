#include "sda.h"
#include "rnc/globals.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_flags __asm__("D_0015EEB4") __attribute__((sda));

void save_card_state_no_save(void) __asm__("FUN_00208c70");

void save_card_state_no_save(void) {
    struct MemoryCardState *s = &memory_card_state;
    if (s->card[0].sync_result != 0) {
        mode_freeze_state = 3;
        return;
    }
    if (mode_freeze_flags & 2)
        mode_freeze_state = 0xD;
}

extern __typeof__(save_card_state_no_save) func_00208C70 __attribute__((alias("FUN_00208c70")));
