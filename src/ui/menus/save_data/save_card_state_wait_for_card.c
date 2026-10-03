#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB4;
extern int D_0015EEB0 MACRO_ADDR;

void save_card_state_wait_for_card(void) __asm__("FUN_002089d0");

void save_card_state_wait_for_card(void) {
    char *s = D_0013D290;
    int v;
    D_0015EEB4 &= ~0x20;
    v = *(int *)(s + 0x1C);
    if (v == 0) {
        D_0015EEB0 = 9;
        return;
    }
    if (v == -1) {
        *(int *)(s + 0x1C) = 0;
        D_0015EEB0 = 9;
        return;
    }
    if (v == -2) D_0015EEB0 = 5;
}

extern __typeof__(save_card_state_wait_for_card) func_002089D0 __attribute__((alias("FUN_002089d0")));
