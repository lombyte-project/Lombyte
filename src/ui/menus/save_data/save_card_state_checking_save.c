#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;

void save_card_state_checking_save(void) __asm__("FUN_00208c00");

void save_card_state_checking_save(void) {
    struct MemoryCardState *s = &memory_card_state;
    int a;
    if (s->card[0].sync_result < -1) {
        mode_freeze_state = 3;
        return;
    }
    a = s->card[0].save_index;
    if (a == -2) {
        if (s->card[0].free < 0x15E) {
            mode_freeze_state = 0x13;
        } else {
            mode_freeze_state = 0xC;
        }
        return;
    }
    if (a >= -1)
        mode_freeze_state = 0x10;
}

extern __typeof__(save_card_state_checking_save) func_00208C00
    __attribute__((alias("FUN_00208c00")));
