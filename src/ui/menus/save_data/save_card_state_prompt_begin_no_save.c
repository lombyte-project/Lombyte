#include "types.h"
#include "rnc/ui/menus/save_data/save_card_state.h"
#include "rnc/globals.h"

/* D_0013D2AC is the +0x1C field of the shared D_0013D290 save-state block. */
#include "rnc/storage/memory_card/memory_card_state.h"

void save_card_state_prompt_begin_no_save(void) __asm__("FUN_00208fe8");

void save_card_state_prompt_begin_no_save(void) {
    if (*(s32 *)((u8 *)&memory_card_state + 0x1C)) {
        mode_freeze_state = 3;
    } else if (mode_freeze_flags & 0x20) {
        mode_freeze_flags ^= 0x20;
        mode_freeze_state = 0xC;
    }
}

extern __typeof__(save_card_state_prompt_begin_no_save) func_00208FE8
    __attribute__((alias("FUN_00208fe8")));
