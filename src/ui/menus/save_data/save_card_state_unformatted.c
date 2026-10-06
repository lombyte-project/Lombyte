#include "sda.h"
extern int D_0015EEB4 __attribute__((sda));
extern int D_0015EEB0;
extern char D_0013D290[];
void save_card_state_unformatted(void) __asm__("FUN_00208a38");

void save_card_state_unformatted(void) {
    char *s = D_0013D290;
    if (*(int *)(s + 0x1C) != -2) {
        D_0015EEB0 = 3;
        return;
    }
    if (D_0015EEB4 & 2)
        D_0015EEB0 = 6;
}
extern __typeof__(save_card_state_unformatted) func_00208A38 __attribute__((alias("FUN_00208a38")));
