/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00209188). */
#include "sda.h"
extern char D_0013D290[];
extern int D_0015EEB0 MACRO_ADDR;
extern int D_0015EEB4 MACRO_ADDR;
/* Clears the 4 and 2 flag bits of D_0015EEB4, then picks the next
   state into D_0015EEB0 from the 0x80/0x100/0x200 bits and the
   D_0013D290 record. The cross-jumped `b` back into the 0x80 arm is
   one GNU as pads as a short loop and ps2eeas did not;
   tools/ps2eeas_nops.py writes it as a .word. */
void save_card_state_good_save(void) __asm__("FUN_002088d0");

void save_card_state_good_save(void) {
    int flags = D_0015EEB4;
    char *b;
    int nf;
    D_0015EEB4 = flags & ~4;
    b = D_0013D290;
    nf = D_0015EEB4 & ~2;
    D_0015EEB4 = nf;
    if (*(int *)(b + 0xF4) == 0) {
        D_0015EEB0 = 3;
        return;
    }
    if (flags & 0x80) {
        D_0015EEB0 = 0x15;
        D_0015EEB4 = (nf ^ 0x80) | 0x40;
        return;
    }
    if (flags & 0x100) {
        D_0015EEB0 = 0x14;
        D_0015EEB4 = (nf ^ 0x100) | 0x40;
        return;
    }
    if (*(int *)(b + 0x1C) != 0) {
        *(int *)(b + 0xF4) = 0;
        D_0015EEB4 = nf | 1;
        D_0015EEB0 = 2;
        return;
    }
    if (flags & 0x200) {
        D_0015EEB4 = nf ^ 0x200;
        D_0015EEB0 = 0x16;
    }
}

extern __typeof__(save_card_state_good_save) func_002088D0 __attribute__((alias("FUN_002088d0")));
