#include "sda.h"
extern int D_0015EEB0 MACRO_ADDR;
extern int D_0015EEB4 MACRO_ADDR;

void save_card_state_format_failed(void) __asm__("FUN_00208e68");

void save_card_state_format_failed(void) {
    if (D_0015EEB4 & 0x40) {
        return;
    }
    D_0015EEB0 = 3;
}

extern __typeof__(save_card_state_format_failed) func_00208E68
    __attribute__((alias("FUN_00208e68")));
