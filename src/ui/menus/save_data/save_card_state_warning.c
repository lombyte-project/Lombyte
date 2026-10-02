#include "sda.h"
extern int D_0015EEB0 MACRO_ADDR;
extern int D_0015EEB4 MACRO_ADDR;

void save_card_state_warning(void) __asm__("FUN_00208980");

void save_card_state_warning(void) {
    if ((D_0015EEB4 ^ 1) & 1) {
        D_0015EEB0 = 3;
    }
}

extern __typeof__(save_card_state_warning) func_00208980 __attribute__((alias("FUN_00208980")));
