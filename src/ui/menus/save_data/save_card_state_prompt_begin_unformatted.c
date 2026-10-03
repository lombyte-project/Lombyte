#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB4_s[] __asm__("D_0015EEB4") __attribute__((sda));
extern int D_0015EEB4;
extern int D_0015EEB0 MACRO_ADDR;

void save_card_state_prompt_begin_unformatted(void) __asm__("FUN_00208fa0");

void save_card_state_prompt_begin_unformatted(void) {
    char *s = D_0013D290;
    int f;
    if (*(int *)(s + 0x1C) != -2) {
        D_0015EEB0 = 3;
        return;
    }
    f = D_0015EEB4_s[0];
    if (f & 0x20) {
        D_0015EEB4 = f ^ 0x20;
        D_0015EEB0 = 5;
    }
}

extern __typeof__(save_card_state_prompt_begin_unformatted) func_00208FA0 __attribute__((alias("FUN_00208fa0")));
