#include "sda.h"
struct Menu { char pad0[0x1C]; int sel; char pad1[0xBC-0x20]; int saved; char pad2[0xDC-0xC0]; int pending; int e0; char pad3[0xF4-0xE4]; int f4; };
extern int D_0015EEB0 MACRO_ADDR;
extern struct Menu D_0013D290;
void save_card_state_init(void) __asm__("FUN_002088a8");

void save_card_state_init(void) {
    D_0015EEB0 = 3;
    D_0013D290.sel = D_0013D290.saved;
    D_0013D290.f4 = 0;
}

extern __typeof__(save_card_state_init) func_002088A8 __attribute__((alias("FUN_002088a8")));
