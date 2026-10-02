#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0 MACRO_ADDR;

void save_card_state_no_room(void) __asm__("FUN_00208eb8");

void save_card_state_no_room(void) {
    if (*(int *)(D_0013D290 + 0x1C) != 0) {
        D_0015EEB0 = 3;
    }
}

extern __typeof__(save_card_state_no_room) func_00208EB8 __attribute__((alias("FUN_00208eb8")));
