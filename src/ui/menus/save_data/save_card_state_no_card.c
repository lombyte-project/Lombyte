#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0 MACRO_ADDR;

void save_card_state_no_card(void) __asm__("FUN_002089a8");

void save_card_state_no_card(void) {
    char *s = D_0013D290;
    *(int *)(s + 0xDC) = -1;
    D_0015EEB0 = 4;
    *(int *)(s + 0xE0) = -1;
}

extern __typeof__(save_card_state_no_card) func_002089A8 __attribute__((alias("FUN_002089a8")));
