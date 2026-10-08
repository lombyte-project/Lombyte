#include "sda.h"
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;
extern int mode_freeze_flags __asm__("D_0015EEB4") MACRO_ADDR;

void save_card_state_load_failed(void) __asm__("FUN_00208ed8");

void save_card_state_load_failed(void) {
    if (mode_freeze_flags & 0x40) {
        return;
    }
    mode_freeze_state = 3;
}

extern __typeof__(save_card_state_load_failed) func_00208ED8 __attribute__((alias("FUN_00208ed8")));
