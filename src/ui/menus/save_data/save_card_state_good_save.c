/* Ported from rac1-decomp (src/game/menu.c, func_00209188). */
#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;
extern int mode_freeze_flags __asm__("D_0015EEB4") MACRO_ADDR;
/* Clears the 4 and 2 flag bits of mode_freeze_flags, then picks the next
   state into mode_freeze_state from the 0x80/0x100/0x200 bits and the
   D_0013D290 record. The cross-jumped `b` back into the 0x80 arm is
   one GNU as pads as a short loop and ps2eeas did not;
   tools/ps2eeas_nops.py writes it as a .word. */
void save_card_state_good_save(void) __asm__("FUN_002088d0");

void save_card_state_good_save(void) {
    int flags = mode_freeze_flags;
    struct MemoryCardState *b;
    int nf;
    mode_freeze_flags = flags & ~4;
    b = &memory_card_state;
    nf = mode_freeze_flags & ~2;
    mode_freeze_flags = nf;
    if (b->unkF4 == 0) {
        mode_freeze_state = 3;
        return;
    }
    if (flags & 0x80) {
        mode_freeze_state = 0x15;
        mode_freeze_flags = (nf ^ 0x80) | 0x40;
        return;
    }
    if (flags & 0x100) {
        mode_freeze_state = 0x14;
        mode_freeze_flags = (nf ^ 0x100) | 0x40;
        return;
    }
    if (b->card[0].sync_result != 0) {
        b->unkF4 = 0;
        mode_freeze_flags = nf | 1;
        mode_freeze_state = 2;
        return;
    }
    if (flags & 0x200) {
        mode_freeze_flags = nf ^ 0x200;
        mode_freeze_state = 0x16;
    }
}

extern __typeof__(save_card_state_good_save) func_002088D0 __attribute__((alias("FUN_002088d0")));
