/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_002094E0). */
#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0 MACRO_ADDR;
void save_card_state_check_save(void) __asm__("FUN_00208bc0");

void save_card_state_check_save(void) {
    char *s = D_0013D290;
    if (*(int *)(s + 0xD4) == 2 && *(int *)(s + 0xDC) < 0) {
        *(int *)(s + 0xDC) = 7;
        *(int *)(s + 0xE0) = 0;
        D_0015EEB0 = 0xB;
    }
}

extern __typeof__(save_card_state_check_save) func_00208BC0 __attribute__((alias("FUN_00208bc0")));
