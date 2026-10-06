#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB4_s[] __asm__("D_0015EEB4") __attribute__((sda));
extern int D_0015EEB4;
extern int D_0015EEB0 MACRO_ADDR;
extern int D_0015F5E8;

void save_card_state_prompt_format(void) __asm__("FUN_00208a78");

void save_card_state_prompt_format(void) {
    char *s = D_0013D290;
    int f;
    if (*(int *)(s + 0x1C) != -2) {
        D_0015EEB0 = 3;
        return;
    }
    f = D_0015EEB4_s[0];
    if (f & 0x20) {
        D_0015EEB4_s[0] = f ^ 0x20;
        if (D_0015F5E8 != 0) {
            D_0015EEB0 = 0x17;
        } else {
            D_0015EEB0 = 5;
        }
        return;
    }
    if (f & 8) {
        D_0015EEB4 = f ^ 8;
        D_0015EEB0 = 7;
    }
}

extern __typeof__(save_card_state_prompt_format) func_00208A78
    __attribute__((alias("FUN_00208a78")));
