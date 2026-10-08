#include "sda.h"
#include "rnc/globals.h"
extern int mode_freeze_flags __asm__("D_0015EEB4") __attribute__((sda));
#include "rnc/storage/memory_card/memory_card_state.h"
void save_card_state_unformatted(void) __asm__("FUN_00208a38");

void save_card_state_unformatted(void) {
    struct MemoryCardState *s = &memory_card_state;
    if (s->card[0].sync_result != -2) {
        mode_freeze_state = 3;
        return;
    }
    if (mode_freeze_flags & 2)
        mode_freeze_state = 6;
}
extern __typeof__(save_card_state_unformatted) func_00208A38 __attribute__((alias("FUN_00208a38")));
