/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00209448). */
#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0 MACRO_ADDR;
extern int D_0015EEB4 MACRO_ADDR;
void save_card_state_formatting(void) __asm__("FUN_00208b28");

void save_card_state_formatting(void) {
    char *s = D_0013D290;
    if (*(int *)(s + 0xD4) == 2 && *(int *)(s + 0xDC) < 0) {
        if (*(int *)(s + 0xE4) != 0) {
            D_0015EEB0 = 0x11;
            D_0015EEB4 |= 0x40;
            return;
        }
        D_0015EEB0 = 0xE;
    }
}

extern __typeof__(save_card_state_formatting) func_00208B28 __attribute__((alias("FUN_00208b28")));
