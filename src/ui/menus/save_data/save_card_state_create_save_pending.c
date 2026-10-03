#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0;

void save_card_state_create_save_pending(void) __asm__("FUN_00208d20");

void save_card_state_create_save_pending(void) {
    char *s = D_0013D290;
    if (*(int *)(s + 0xD4) == 2 && *(int *)(s + 0xDC) < 0) {
        *(int *)(s + 0xDC) = 9;
        *(int *)(s + 0xE0) = 0;
        D_0015EEB0 = 0xF;
    }
}

extern __typeof__(save_card_state_create_save_pending) func_00208D20 __attribute__((alias("FUN_00208d20")));
