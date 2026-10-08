#include "sda.h"
#include "rnc/globals.h"
extern char D_0013D290[];

void save_card_state_create_save_pending(void) __asm__("FUN_00208d20");

void save_card_state_create_save_pending(void) {
    char *s = D_0013D290;
    if (*(int *)(s + 0xD4) == 2 && *(int *)(s + 0xDC) < 0) {
        *(int *)(s + 0xDC) = 9;
        *(int *)(s + 0xE0) = 0;
        mode_freeze_state = 0xF;
    }
}

extern __typeof__(save_card_state_create_save_pending) func_00208D20
    __attribute__((alias("FUN_00208d20")));
