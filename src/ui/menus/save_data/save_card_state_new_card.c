#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB4 MACRO_ADDR;
extern int D_0015EEB0 MACRO_ADDR;

void save_card_state_new_card(void) __asm__("FUN_00208dd8");

void save_card_state_new_card(void) {
    char *s;
    int f = D_0015EEB4;
    int t = f & ~4;
    int g = t & ~2;
    D_0015EEB4 = g;
    if (f & 0x80) {
        D_0015EEB0 = 0x15;
        D_0015EEB4 = (g ^ 0x80) | 0x40;
        return;
    }
    if (f & 0x100) {
        D_0015EEB0 = 0x14;
        D_0015EEB4 = (g ^ 0x100) | 0x40;
        return;
    }
    s = D_0013D290;
    if (*(int *)(s + 0x1C) != 0) {
        D_0015EEB0 = 3;
        return;
    }
    if (*(int *)(s + 0xF4) != 0)
        D_0015EEB0 = 1;
}

extern __typeof__(save_card_state_new_card) func_00208DD8 __attribute__((alias("FUN_00208dd8")));
