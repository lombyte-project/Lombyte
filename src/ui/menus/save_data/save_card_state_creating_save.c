/* Ported from rac1-decomp (src/game/menu.c, func_002096D8). */
#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;
extern int mode_freeze_flags __asm__("D_0015EEB4") MACRO_ADDR;
void save_card_state_creating_save(void) __asm__("FUN_00208d60");

void save_card_state_creating_save(void) {
    struct MemoryCardState *s = &memory_card_state;
    if (s->state == 2 && s->pending_state < 0) {
        if (s->err != 0) {
            mode_freeze_state = 0x12;
            mode_freeze_flags |= 0x40;
            return;
        }
        s->pending_state = 7;
        s->active_card = 0;
        s->card[0].save_index = 0;
        s->pending_card = 0;
        mode_freeze_state = 0x10;
    }
}

extern __typeof__(save_card_state_creating_save) func_00208D60
    __attribute__((alias("FUN_00208d60")));
