/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_002096D8). */
#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0 MACRO_ADDR;
extern int D_0015EEB4 MACRO_ADDR;
void save_card_state_creating_save(void) __asm__("FUN_00208d60");

void save_card_state_creating_save(void) {
    char *s = D_0013D290;
    if (*(int *)(s + 0xD4) == 2 && *(int *)(s + 0xDC) < 0) {
        if (*(int *)(s + 0xE4) != 0) {
            D_0015EEB0 = 0x12;
            D_0015EEB4 |= 0x40;
            return;
        }
        *(int *)(s + 0xDC) = 7;
        *(int *)(s + 0xC0) = 0;
        *(int *)(s + 0x14) = 0;
        *(int *)(s + 0xE0) = 0;
        D_0015EEB0 = 0x10;
    }
}

extern __typeof__(save_card_state_creating_save) func_00208D60 __attribute__((alias("FUN_00208d60")));
