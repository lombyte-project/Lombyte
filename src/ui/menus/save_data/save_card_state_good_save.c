/* Ported from rac1-decomp (src/game/menu.c, func_00209188). */
#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"

/* Clears the 4 and 2 flag bits of mode_freeze_flags, then picks the next
   state into save_card_state from the 0x80/0x100/0x200 bits and the
   D_0013D290 record. The cross-jumped `b` back into the 0x80 arm is
   one GNU as pads as a short loop and ps2eeas did not;
   tools/ps2eeas_nops.py writes it as a .word. */
void save_card_state_good_save(void) __asm__("FUN_002088d0");

void save_card_state_good_save(void) {
    int                     flags = mode_freeze_flags;
    struct MemoryCardState *card_state;
    int                     nf;
    mode_freeze_flags = flags & ~4;
    card_state = &memory_card_state;
    nf = mode_freeze_flags & ~2;
    mode_freeze_flags = nf;
    if (card_state->card_save_active == 0) {
        save_card_state = CS_NOCARD;
        return;
    }
    if (flags & 0x80) {
        save_card_state = CS_SAVE_FAILED;
        mode_freeze_flags = (nf ^ 0x80) | 0x40;
        return;
    }
    if (flags & 0x100) {
        save_card_state = CS_LOAD_FAILED;
        mode_freeze_flags = (nf ^ 0x100) | 0x40;
        return;
    }
    if (card_state->card[0].sync_result != 0) {
        card_state->card_save_active = 0;
        mode_freeze_flags = nf | 1;
        save_card_state = CS_WARNING;
        return;
    }
    if (flags & 0x200) {
        mode_freeze_flags = nf ^ 0x200;
        save_card_state = CS_SAVING;
    }
}

extern __typeof__(save_card_state_good_save) func_002088D0 __attribute__((alias("FUN_002088d0")));
