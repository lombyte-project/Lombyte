#include "sda.h"
extern char D_0013D290[];
extern int mode_freeze_state __asm__("D_0015EEB0") __attribute__((sda));

void save_card_state_format_pending(void) __asm__("FUN_00208af8");

void save_card_state_format_pending(void) {
    char *s = D_0013D290;
    int pending = *(int *)(s + 0xDC);
    *(int *)(s + 0x1C) = 0;
    if (pending < 0) {
        *(int *)(s + 0xE0) = 0;
        *(int *)(s + 0xDC) = 3;
    }
    mode_freeze_state = 8;
}

extern __typeof__(save_card_state_format_pending) func_00208AF8
    __attribute__((alias("FUN_00208af8")));
