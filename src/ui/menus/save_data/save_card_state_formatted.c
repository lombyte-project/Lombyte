#include "types.h"
#include "rnc/ui_menus_fun_00208f28_types.h"

/* D_0013D2AC is the +0x1C field of the shared D_0013D290 save-state block. */
extern struct M2c_D_0013D290 D_0013D290;
extern s32 D_0015EEB0;
extern s32 D_0015EEB4;

void save_card_state_formatted(void) __asm__("FUN_00208b88");

void save_card_state_formatted(void) {
    if (*(s32 *)((u8 *)&D_0013D290 + 0x1C)) {
        D_0015EEB0 = 3;
    } else if (D_0015EEB4 & 6) {
        D_0015EEB0 = 0xA;
    }
}

extern __typeof__(save_card_state_formatted) func_00208B88 __attribute__((alias("FUN_00208b88")));
