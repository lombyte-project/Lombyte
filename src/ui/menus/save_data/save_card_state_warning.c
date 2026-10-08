#include "sda.h"
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;
extern int mode_freeze_flags __asm__("D_0015EEB4") MACRO_ADDR;

void save_card_state_warning(void) __asm__("FUN_00208980");

void save_card_state_warning(void) {
    if ((mode_freeze_flags ^ 1) & 1) {
        mode_freeze_state = 3;
    }
}

extern __typeof__(save_card_state_warning) func_00208980 __attribute__((alias("FUN_00208980")));
