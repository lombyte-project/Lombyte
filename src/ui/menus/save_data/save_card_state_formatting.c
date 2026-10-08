/* Ported from rac1-decomp (src/game/menu.c, func_00209448). */
#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;
extern int mode_freeze_flags __asm__("D_0015EEB4") MACRO_ADDR;
void save_card_state_formatting(void) __asm__("FUN_00208b28");

void save_card_state_formatting(void) {
    struct MemoryCardState *s = &memory_card_state;
    if (s->state == 2 && s->pending_state < 0) {
        if (s->err != 0) {
            mode_freeze_state = 0x11;
            mode_freeze_flags |= 0x40;
            return;
        }
        mode_freeze_state = 0xE;
    }
}

extern __typeof__(save_card_state_formatting) func_00208B28 __attribute__((alias("FUN_00208b28")));
