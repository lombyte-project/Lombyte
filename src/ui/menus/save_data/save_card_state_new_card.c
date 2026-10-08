#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_flags __asm__("D_0015EEB4") MACRO_ADDR;
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;

void save_card_state_new_card(void) __asm__("FUN_00208dd8");

void save_card_state_new_card(void) {
    struct MemoryCardState *s;
    int f = mode_freeze_flags;
    int t = f & ~4;
    int g = t & ~2;
    mode_freeze_flags = g;
    if (f & 0x80) {
        mode_freeze_state = 0x15;
        mode_freeze_flags = (g ^ 0x80) | 0x40;
        return;
    }
    if (f & 0x100) {
        mode_freeze_state = 0x14;
        mode_freeze_flags = (g ^ 0x100) | 0x40;
        return;
    }
    s = &memory_card_state;
    if (s->card[0].sync_result != 0) {
        mode_freeze_state = 3;
        return;
    }
    if (s->unkF4 != 0)
        mode_freeze_state = 1;
}

extern __typeof__(save_card_state_new_card) func_00208DD8 __attribute__((alias("FUN_00208dd8")));
