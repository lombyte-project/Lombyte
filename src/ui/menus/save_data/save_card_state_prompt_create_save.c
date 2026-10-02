#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0 MACRO_ADDR;
extern int D_0015EEB4 MACRO_ADDR;
extern int D_0015F5E8 MACRO_ADDR;

void save_card_state_prompt_create_save(void) __asm__("FUN_00208ca8");

void save_card_state_prompt_create_save(void) {
    int flags;

    if (*(int *)(D_0013D290 + 0x1C) != 0) {
        D_0015EEB0 = 3;
        return;
    }
    flags = D_0015EEB4;
    if (flags & 0x20) {
        D_0015EEB4 = flags ^ 0x20;
        if (D_0015F5E8 != 0) {
            D_0015EEB0 = 0x18;
        } else {
            D_0015EEB0 = 0xC;
        }
        return;
    }
    if (flags & 0x10) {
        D_0015EEB4 = flags ^ 0x10;
        D_0015EEB0 = 0xE;
    }
}

extern __typeof__(save_card_state_prompt_create_save) func_00208CA8 __attribute__((alias("FUN_00208ca8")));
