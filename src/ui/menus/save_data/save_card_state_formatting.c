/* Ported from rac1-decomp (src/game/menu.c, func_00209448). */
#include "sda.h"
extern char D_0013D290[];
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;
extern int mode_freeze_flags __asm__("D_0015EEB4") MACRO_ADDR;
void save_card_state_formatting(void) __asm__("FUN_00208b28");

void save_card_state_formatting(void) {
    char *s = D_0013D290;
    if (*(int *)(s + 0xD4) == 2 && *(int *)(s + 0xDC) < 0) {
        if (*(int *)(s + 0xE4) != 0) {
            mode_freeze_state = 0x11;
            mode_freeze_flags |= 0x40;
            return;
        }
        mode_freeze_state = 0xE;
    }
}

extern __typeof__(save_card_state_formatting) func_00208B28 __attribute__((alias("FUN_00208b28")));
