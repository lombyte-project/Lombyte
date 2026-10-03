#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0 MACRO_ADDR;

void save_card_state_checking_save(void) __asm__("FUN_00208c00");

void save_card_state_checking_save(void) {
    char *s = D_0013D290;
    int a;
    if (*(int *)(s + 0x1C) < -1) {
        D_0015EEB0 = 3;
        return;
    }
    a = *(int *)(s + 0x14);
    if (a == -2) {
        if (*(int *)(s + 0xC) < 0x15E) {
            D_0015EEB0 = 0x13;
        } else {
            D_0015EEB0 = 0xC;
        }
        return;
    }
    if (a >= -1) D_0015EEB0 = 0x10;
}

extern __typeof__(save_card_state_checking_save) func_00208C00 __attribute__((alias("FUN_00208c00")));
