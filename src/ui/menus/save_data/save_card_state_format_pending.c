#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0 __attribute__((sda));

void save_card_state_format_pending(void) __asm__("FUN_00208af8");

void save_card_state_format_pending(void) {
    char *s = D_0013D290;
    int pending = *(int *)(s + 0xDC);
    *(int *)(s + 0x1C) = 0;
    if (pending < 0) {
        *(int *)(s + 0xE0) = 0;
        *(int *)(s + 0xDC) = 3;
    }
    D_0015EEB0 = 8;
}

extern __typeof__(save_card_state_format_pending) func_00208AF8 __attribute__((alias("FUN_00208af8")));
