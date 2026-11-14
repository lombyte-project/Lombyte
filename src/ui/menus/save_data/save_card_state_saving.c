/* save_card_state_saving exact recovery: retail keeps the `mode_freeze_flags |= 0x40` result
   in v0 and the 0x15 constant in v1; without the pin the allocator swaps them.
   The register-asm pin reproduces retail's block, one instruction stream for
   all 30 instructions. */

#include "types.h"
#include "rnc/ui/menus/save_data/save_card_state.h"
#include "rnc/globals.h"

#include "rnc/save/memory_card_state.h"

extern void mode_freeze_init() __asm__("func_001FBAB8");

void save_card_state_saving(void) __asm__("FUN_00208f28");

void save_card_state_saving(void) {
    s32 updated_freeze_flags;

    if ((memory_card_state.state < MEMCARD_REQUEST_FORMAT) &&
        (memory_card_state.pending_state < 0)) {
        if (memory_card_state.err != 0) {
            mode_freeze_init(3, 0);
            save_card_state = CS_SAVE_FAILED;
            updated_freeze_flags = mode_freeze_flags | 0x40;
            mode_freeze_flags = updated_freeze_flags;
        } else {
            save_card_state = CS_GOOD_SAVE;
        }
    }
}
