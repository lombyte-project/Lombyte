/* Ported from rac1-decomp (src/game/menu.c, func_002094E0). */
#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;
void save_card_state_check_save(void) __asm__("FUN_00208bc0");

void save_card_state_check_save(void) {
    struct MemoryCardState *s = &memory_card_state;
    if (s->state == 2 && s->pending_state < 0) {
        s->pending_state = 7;
        s->pending_card = 0;
        mode_freeze_state = 0xB;
    }
}

extern __typeof__(save_card_state_check_save) func_00208BC0 __attribute__((alias("FUN_00208bc0")));
